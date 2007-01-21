/** @file
 * 
 * Dvt::MlString - Part of the portable Dictionary and Vocabulary Trainer.
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

#include "dvtMlString.h"

#include "dvtCore.h"
#include "dvtExceptions.h"

#include "debug.h"

namespace Dvt
{

using namespace std;

MlString::MlString()
{
}

MlString::~MlString()
{
}

void MlString::setString(const string& langId, const string& str) {
	if (str != "") translations[langId] = str;
}

string MlString::getString(const string& langId) {
	return translations[langId];
}

void MlString::setFromXmlNode(sxml::XmlNode* node)
{
	translations.clear();
	updateFromXmlNode(node);
}

void MlString::setToXmlNode(sxml::XmlNode* node) {
	assert(node != NULL);
	
	sxml::XmlNode* cnode;
	map<string, string>::iterator it;
	for (it = translations.begin(); it != translations.end(); it++) {
		if (it->second != "") {
			cnode = new sxml::XmlNode(sxml::ntElementNode, "lg");
			cnode->attributes["c"] = it->first;
			cnode->children.push_back(new sxml::XmlNode(sxml::ntTextNode, it->second));
			node->children.push_back(cnode);
		}
	}
}

void MlString::updateFromXmlNode(sxml::XmlNode* node)
{
	sxml::XmlNode* cnode;
	sxml::NodeSearch* ns = node->findInit("lg");
	
	try {
		for (cnode = node->findNext(ns); cnode != NULL; cnode = node->findNext(ns)) {
			string code;
			
			sxml::NodeAttributesIterator it = cnode->attributes.find("c");
			if (it != cnode->attributes.end()) {
				code = it->second;
			} else {
				code = "ia";
			}
			
			if (cnode->children.size() != 1 ||
				(cnode->children.size() == 1 && cnode->children[0]->type != sxml::ntTextNode))
			{
				throw EXmlTreeInvalid("", node->name);
			}
			
//			DEBUG(DBG_GENERAL, DBGL_INFO, "MlString::setFromXmlNode(): found %s:%s\n", code.c_str(), cnode->children[0]->name.c_str());
			
			translations[code] = cnode->children[0]->name;
		}	
	} catch (Exception e) {
		node->findFree(ns);
		throw;
	}
}

void MlString::setFrom(const MlString& mls)
{
	translations = mls.translations;
}

void MlString::clear()
{
	translations.clear();
}

string MlString::str() {
	if (translations.size() == 0) {
		return string();
	}
	
	map<string, string>::iterator tr_it;
	
	list<string>::iterator it;
	for (it = Core::languagePrefOrder().begin(); it != Core::languagePrefOrder().end(); it++) {
//		DEBUG(DBG_GENERAL, "MlString::str(): languagePrefOrder: %s\n", it->c_str());
		tr_it = translations.find(*it);
		if (tr_it != translations.end() && !tr_it->second.empty()) {
			return tr_it->second;
		}
	}
	
	assert(it == Core::languagePrefOrder().end());
	
	// try interlingua
	tr_it = translations.find("ia");
	if (tr_it != translations.end() && !tr_it->second.empty()) {
		return tr_it->second;
	}
	
	// try english
	tr_it = translations.find("en");
	if (tr_it != translations.end() && !tr_it->second.empty()) {
		return tr_it->second;
	}
	
	// get first non-empty element
	for (tr_it = translations.begin(); tr_it != translations.end(); tr_it++) {
		if (!tr_it->second.empty())
			return tr_it->second;
	}
	
	// nothing found
	return string();
	
//	DEBUG(DBG_GENERAL, DBGL_INFO, "MlString::str(): %s\n", res.c_str());
}

const char* MlString::c_str() {
	return str().c_str();
}

bool MlString::empty() {
	map<string, string>::iterator it;
	for (it = translations.begin(); it != translations.end(); it++) {
		if (!it->second.empty()) {
			return false;
		}
	}
	return true;
}

MlString::operator string () {
	return str();
}

string MlString::operator[](const string& langId) {
	return translations[langId];
}

void MlString::operator= (const MlString& mls) {
	return setFrom(mls);
}

} // namespace
