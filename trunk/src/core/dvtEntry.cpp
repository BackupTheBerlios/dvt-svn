/** @file
 * 
 * Dvt::Entry - Part of the portable Dictionary and Vocabulary Trainer.
 * 
 * Copyright (C) 2006  Denis Martin (http://www.delta-my.de/)
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * 
 */

#include "dvtEntry.h"

#include "dvtCore.h"
#include "dvtLanguageProfile.h"
#include "dvtLanguage.h"

#include "debug.h"

#include <stdlib.h>
#include <assert.h>

namespace Dvt
{

using namespace std;

Entry::Entry(LanguageProfile* lp, std::string baseName, WordClass::Type wc)
{
	core = Core::getInstance();
	this->lp = lp;
	p_baseName = baseName;
	p_wc = wc;
	
	p_gender = Gender::Undefined;
}

Entry::~Entry()
{
}

LanguageProfile* Entry::languageProfile() {return lp;}

string Entry::baseName() {return p_baseName;}
void Entry::setBaseName(string s) {p_baseName = s;}

WordClass::Type Entry::wordClass() {return p_wc;}
void Entry::setWordClass(WordClass::Type wct) {
	if (wct >= 0 && wct < WordClass::High) p_wc = wct;
}

Gender::Type Entry::gender() {return p_gender;} 
void Entry::setGender(Gender::Type gt) {
	if (gt >= 0 && gt < Gender::High) p_gender = gt;
}

map<string, string>& Entry::decls() {return p_decls;} 

/**
 * Return the conjugations for a given tense. If tense is omitted, the default
 * tense is used (eg. none or the first tense with the lowest ID defined in the
 * XML file).
 * 
 * @param tense	Pointer to tense
 * 
 * @return STL map with conjugations
 */
map<PersonalPronoun, string>& Entry::conjs(Tense* tense)
{
	if (tense == NULL)
		tense = lp->getDefaultTense();
	
	return p_conjs[tense];
}

string Entry::longName() {return lp->lang()->getLongName(this);}
string Entry::longNameHtml() {return lp->lang()->getLongNameHtml(this);}

string Entry::getDecl(string id) {return p_decls[id];}
void  Entry::setDecl(string id, std::string s) {p_decls[id] = s;}

/**
 * Get a conjugated form of a verb. If tense is omitted the default tense is
 * used (eg. none or the first tense with the lowest ID defined in the
 * XML file).
 * 
 * @param pp	Personal pronoun
 * @param tense	Tense
 * 
 * @return Conjugated form of the entry
 */
string Entry::getConj(PersonalPronoun pp, Tense* tense)
{
	if (tense == NULL)
		tense = lp->getDefaultTense();
		
	return p_conjs[tense][pp];
}

/**
 * Set a conjugated form of a verb. If tense is omitted the default tense is
 * used (eg. none or the first tense with the lowest ID defined in the
 * XML file).
 * 
 * @param pp	Personal pronoun
 * @param s		Conjugated form of the entry
 * @param tense	Tense
 */
void Entry::setConj(PersonalPronoun pp, string s, Tense* tense)
{
	if (tense == NULL)
		tense = lp->getDefaultTense();
	
	p_conjs[tense][pp] = s;
}

Conjugation* Entry::conjClass() {return lp->getConjugation(p_conjClass);}

void Entry::setConjClass(Conjugation* cc) 
{
	if (cc != NULL) p_conjClass = cc->id();
	else p_conjClass = string();
}

void Entry::setFromXmlNode(sxml::XmlNode* node)
{	
	sxml::XmlNode* n;
	
	n = node->findFirst("g");
	if (n != NULL)
		p_gender = Gender::getType(n->attributes["t"]);
	
	n = node->findFirst("dcls");
	if (n != NULL) {
		sxml::XmlNode* d;
		sxml::NodeSearch* ns = n->findInit("dcl");
		while ((d = n->findNext(ns))) {
			string t = d->attributes["t"];
			if (!t.empty() && d->children.size() == 1 && 
				d->children[0]->type == sxml::ntTextNode)
			{
				p_decls[t] = d->children[0]->name; 
			}
		}
		n->findFree(ns);
	}
	
	sxml::NodeSearch* ns_cj = node->findInit("cj");
	while ((n = node->findNext(ns_cj))) {
		Tense* tense = lp->getTense(n->attributes["ts"]);
		if (tense == NULL)
			tense = lp->getDefaultTense();
		p_conjClass = n->attributes["t"];
		sxml::XmlNode* pn;
		sxml::NodeSearch* ns = n->findInit("p");
		while ((pn = n->findNext(ns))) {
			PersonalPronoun pp = (PersonalPronoun) atoi(pn->attributes["t"].c_str());
			if (pp > ppUndefined && pp < ppHigh && pn->children.size() == 1 && 
				pn->children[0]->type == sxml::ntTextNode)
			{
				p_conjs[tense][pp] = pn->children[0]->name;
			}
		}
		n->findFree(ns);
	}
	node->findFree(ns_cj);
	
}

void Entry::setToXmlNode(sxml::XmlNode* node)
{
	if (p_gender > Gender::Undefined && p_gender < Gender::High) {
		sxml::XmlNode* n = new sxml::XmlNode(sxml::ntElementNode, "g");
		n->attributes["t"] = Gender::getXmlAttr(p_gender);
		node->children.push_back(n);
	}
	
	if (p_decls.size() > 0) {
		sxml::XmlNode* n = new sxml::XmlNode(sxml::ntElementNode, "dcls");
		map<string, string>::const_iterator it;
		for (it = p_decls.begin(); it != p_decls.end(); it++) {
			if (!it->first.empty() && !it->second.empty()) {
				sxml::XmlNode* dn = new sxml::XmlNode(sxml::ntElementNode, "dcl");
				dn->attributes["t"] = it->first;
				sxml::XmlNode* tn = new sxml::XmlNode(sxml::ntTextNode, it->second);
				dn->children.push_back(tn);
				n->children.push_back(dn);
			}
		}
		if (n->children.size() > 0)
			node->children.push_back(n);
		else
			delete n;
	}
	
	map<Tense*, map<PersonalPronoun, std::string> >::iterator it_tense;
	for (it_tense = p_conjs.begin(); it_tense != p_conjs.end(); it_tense++) {
		Tense* tense = it_tense->first;
		map<PersonalPronoun, std::string>& cjs = it_tense->second;
		
		sxml::XmlNode* n = new sxml::XmlNode(sxml::ntElementNode, "cj");
		if (!p_conjClass.empty())	
			n->attributes["t"] = p_conjClass;
		if (tense != NULL)
			n->attributes["ts"] = tense->id();
			
		map<PersonalPronoun, string>::const_iterator it;
		for (it = cjs.begin(); it != cjs.end(); it++) {
			if (it->first > ppUndefined && it->first < ppHigh && !it->second.empty()) {
				char pps[33];
				snprintf(pps, 33, "%d", it->first);
				sxml::XmlNode* pn = new sxml::XmlNode(sxml::ntElementNode, "p");
				pn->attributes["t"] = pps;
				sxml::XmlNode* tn = new sxml::XmlNode(sxml::ntTextNode, it->second);
				pn->children.push_back(tn);
				n->children.push_back(pn);
			}
		}
		if (n->children.size() > 0)
			node->children.push_back(n);
		else
			delete n;
	}
}

}
