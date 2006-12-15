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
map<PersonalPronoun, string>& Entry::conjs() {return p_conjs;}

string Entry::longName() {return lp->lang()->getLongName(this);}
string Entry::longNameHtml() {return lp->lang()->getLongNameHtml(this);}

string Entry::getDecl(string id) {return p_decls[id];}
void  Entry::setDecl(string id, std::string s) {p_decls[id] = s;}

string Entry::getConj(PersonalPronoun pp) {return p_conjs[pp];}
void Entry::setConj(PersonalPronoun pp, string s) {p_conjs[pp] = s;}

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
	
	n = node->findFirst("cj");
	if (n != NULL) {
		p_conjClass = n->attributes["t"];
		sxml::XmlNode* pn;
		sxml::NodeSearch* ns = n->findInit("p");
		while ((pn = n->findNext(ns))) {
			PersonalPronoun pp = (PersonalPronoun) atoi(pn->attributes["t"].c_str());
			if (pp > ppUndefined && pp < ppHigh && pn->children.size() == 1 && 
				pn->children[0]->type == sxml::ntTextNode)
			{
				p_conjs[pp] = pn->children[0]->name;
			}
		}
		n->findFree(ns);
	}
	
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
	}
	
	if (p_conjs.size() > 0) {
		sxml::XmlNode* n = new sxml::XmlNode(sxml::ntElementNode, "cj");
		if (!p_conjClass.empty())	
			n->attributes["t"] = p_conjClass;
		map<PersonalPronoun, string>::const_iterator it;
		for (it = p_conjs.begin(); it != p_conjs.end(); it++) {
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
	}
}

}
