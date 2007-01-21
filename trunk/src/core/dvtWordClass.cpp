/** @file
 * 
 * Dvt::WordClass - Part of the portable Dictionary and Vocabulary Trainer.
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

#include "dvtWordClass.h"

#include "debug.h"

namespace Dvt
{

using namespace std;

/* Gender *********************************************************************/

bool Gender::initialized = false;
map<Gender::Type, MlString> Gender::p_gtToMlString;
map<Gender::Type, MlString> Gender::p_gtToMlStringShort;
map<Gender::Type, string> Gender::p_typeToXmlAttr;
map<string, Gender::Type> Gender::p_xmlAttrToType;

void Gender::init()
{
	p_gtToMlString[Undefined].setString("en", "undefined");
	p_gtToMlString[Male].setString("en", "masculine");
	p_gtToMlString[Female].setString("en", "feminine");
	p_gtToMlString[Neuter].setString("en", "neuter");
	
	p_gtToMlStringShort[Undefined].setString("en", "u");
	p_gtToMlStringShort[Male].setString("en", "m");
	p_gtToMlStringShort[Female].setString("en", "f");
	p_gtToMlStringShort[Neuter].setString("en", "n");
	
	p_typeToXmlAttr[Undefined] = "u";
	p_typeToXmlAttr[Male] = "m";
	p_typeToXmlAttr[Female] = "f";
	p_typeToXmlAttr[Neuter] = "n";
	
	p_xmlAttrToType["u"] = Undefined;
	p_xmlAttrToType["m"] = Male;
	p_xmlAttrToType["f"] = Female;
	p_xmlAttrToType["n"] = Neuter;
	
	initialized = true;
}

string Gender::getString(Gender::Type type)
{
	if (!initialized) init();
	
	map<Type, MlString>::iterator it;
	it = p_gtToMlString.find(type);
	if (it != p_gtToMlString.end())
		return it->second.str();
	else
		return "UNKNOWN";
}
	
string Gender::getShortString(Gender::Type type)
{
	if (!initialized) init();
	
	map<Type, MlString>::iterator it;
	it = p_gtToMlStringShort.find(type);
	if (it != p_gtToMlStringShort.end())
		return it->second.str();
	else
		return "UNKNOWN";
}

string Gender::getXmlAttr(Gender::Type type)
{
	if (!initialized) init();
	
	map<Type, MlString>::iterator it;
	it = p_gtToMlStringShort.find(type);
	if (it != p_gtToMlStringShort.end())
		return it->second.str();
	else
		return "UNKNOWN";
}

Gender::Type Gender::getType(std::string xmlAttr)
{
	if (!initialized) init();
	
	map<string, Gender::Type>::iterator it;
	it = p_xmlAttrToType.find(xmlAttr);
	if (it != p_xmlAttrToType.end())
		return it->second;
	else
		return Undefined;
}

void Gender::updateFromXmlNode(sxml::XmlNode* node)
{
	if (!initialized) init();
	
	sxml::NodeSearch* ns = node->findInit("g");
	sxml::XmlNode* node2 = node->findNext(ns);
	while (node2 != NULL) {
		string s = node2->attributes["t"];
		if (!s.empty()) {
			Type t = getType(s);
			if (t != Undefined || s == "u")
				p_gtToMlString[t].updateFromXmlNode(node2);
				
		}
		
		node2 = node->findNext(ns);
		
	}
	
	node->findFree(ns);
	
}

void Gender::updateShortNamesFromXmlNode(sxml::XmlNode* node)
{
	if (!initialized) init();
	
	sxml::NodeSearch* ns = node->findInit("g");
	sxml::XmlNode* node2 = node->findNext(ns);
	while (node2 != NULL) {
		string s = node2->attributes["t"];
		if (!s.empty()) {
			Type t = getType(s);
			if (t != Undefined || s == "u")
				p_gtToMlStringShort[t].updateFromXmlNode(node2);
				
		}
		
		node2 = node->findNext(ns);
		
	}
	
	node->findFree(ns);
	
}

/* WordClass ******************************************************************/

#define DVT_WC_XMLATTR_UNDEFINED		"u"
#define DVT_WC_XMLATTR_NOUN				"n"
#define DVT_WC_XMLATTR_VERB				"v"
#define DVT_WC_XMLATTR_ADJECTIVE		"ad"
#define DVT_WC_XMLATTR_ADVERB			"av"
#define DVT_WC_XMLATTR_PRONOUN			"pn"
#define DVT_WC_XMLATTR_PREPOSITION		"pp"
#define DVT_WC_XMLATTR_CONJUNCTION		"cj"
#define DVT_WC_XMLATTR_INTERJECTION		"ij"
#define DVT_WC_XMLATTR_NUMERAL			"nr"
#define DVT_WC_XMLATTR_PHRASE			"p"
#define DVT_WC_XMLATTR_NONNOUNNONVERB	"nnv"
#define DVT_WC_XMLATTR_HIGH				"INVALID"

bool WordClass::initialized = false;

const char* WordClass::p_wctToXmlAttr[WordClass::High] = {
	DVT_WC_XMLATTR_UNDEFINED, 
	DVT_WC_XMLATTR_NOUN, 
	DVT_WC_XMLATTR_VERB,
	DVT_WC_XMLATTR_ADJECTIVE,
	DVT_WC_XMLATTR_ADVERB,
	DVT_WC_XMLATTR_PRONOUN,
	DVT_WC_XMLATTR_PREPOSITION,
	DVT_WC_XMLATTR_CONJUNCTION,
	DVT_WC_XMLATTR_INTERJECTION,
	DVT_WC_XMLATTR_NUMERAL,
	DVT_WC_XMLATTR_PHRASE,
	DVT_WC_XMLATTR_NONNOUNNONVERB
};
map<string, WordClass::Type> WordClass::p_xmlAttrToWct;
map<WordClass::Type, MlString> WordClass::p_wctToMlString;
map<WordClass::Type, MlString> WordClass::p_wctToMlStringShort;

/**
 * Static
 */
void WordClass::init()
{
	p_wctToMlString.clear();
	p_wctToMlString[Undefined].setString("en", "undefined");
	p_wctToMlString[Noun].setString("en", "noun");
	p_wctToMlString[Verb].setString("en", "verb");
	p_wctToMlString[Adjective].setString("en", "adjective");
	p_wctToMlString[Adverb].setString("en", "adverb");
	p_wctToMlString[Pronoun].setString("en", "pronoun");
	p_wctToMlString[Preposition].setString("en", "preposition");
	p_wctToMlString[Conjunction].setString("en", "conjunction");
	p_wctToMlString[Interjection].setString("en", "interjection");
	p_wctToMlString[Numeral].setString("en", "numeral");
	p_wctToMlString[Phrase].setString("en", "phrase");
	p_wctToMlString[NonNounNonVerb].setString("en", "not noun, not verb");
	
	p_wctToMlStringShort.clear();
	p_wctToMlStringShort[Undefined].setString("en", "u");
	p_wctToMlStringShort[Noun].setString("en", "n");
	p_wctToMlStringShort[Verb].setString("en", "v");
	p_wctToMlStringShort[Adjective].setString("en", "adj");
	p_wctToMlStringShort[Adverb].setString("en", "adv");
	p_wctToMlStringShort[Pronoun].setString("en", "pron");
	p_wctToMlStringShort[Preposition].setString("en", "prep");
	p_wctToMlStringShort[Conjunction].setString("en", "cj");
	p_wctToMlStringShort[Interjection].setString("en", "interj");
	p_wctToMlStringShort[Numeral].setString("en", "num");
	p_wctToMlStringShort[Phrase].setString("en", "phr");
	p_wctToMlStringShort[NonNounNonVerb].setString("en", "nn/nv");
	
	p_xmlAttrToWct.clear();
	p_xmlAttrToWct[DVT_WC_XMLATTR_UNDEFINED] = Undefined;
	p_xmlAttrToWct[DVT_WC_XMLATTR_NOUN] = Noun;
	p_xmlAttrToWct[DVT_WC_XMLATTR_VERB] = Verb;
	p_xmlAttrToWct[DVT_WC_XMLATTR_ADJECTIVE] = Adjective;
	p_xmlAttrToWct[DVT_WC_XMLATTR_ADVERB] = Adverb;
	p_xmlAttrToWct[DVT_WC_XMLATTR_PRONOUN] = Pronoun;
	p_xmlAttrToWct[DVT_WC_XMLATTR_PREPOSITION] = Preposition;
	p_xmlAttrToWct[DVT_WC_XMLATTR_CONJUNCTION] = Conjunction;
	p_xmlAttrToWct[DVT_WC_XMLATTR_INTERJECTION] = Interjection;
	p_xmlAttrToWct[DVT_WC_XMLATTR_NUMERAL] = Numeral;
	p_xmlAttrToWct[DVT_WC_XMLATTR_PHRASE] = Phrase;
	p_xmlAttrToWct[DVT_WC_XMLATTR_NONNOUNNONVERB] = NonNounNonVerb;
	
	initialized = true;
}

/**
 * Static
 */
string WordClass::getString(Type wc)
{
	if (!initialized) init();
	
	map<Type, MlString>::iterator it;
	
	it = p_wctToMlString.find(wc);
	if (it != p_wctToMlString.end())
		return it->second;
	else
		return "UNKNOWN";
}

/**
 * Static
 */
string WordClass::getShortString(Type wc)
{
	if (!initialized) init();
	
	map<Type, MlString>::iterator it;
	
	it = p_wctToMlStringShort.find(wc);
	if (it != p_wctToMlStringShort.end())
		return it->second;
	else
		return "UNKNOWN";
}

/**
 * Static
 */
string WordClass::getXmlAttr(Type wc)
{
	if (!initialized) init();
	
	if (wc >= Undefined && wc < High-1)
		return p_wctToXmlAttr[wc];
	else
		return p_wctToXmlAttr[Undefined];
}

/**
 * Static
 */
WordClass::Type WordClass::getType(const string xmlAttr)
{
	if (!initialized) init();
	
	map<string, Type>::iterator it;
	
	it = p_xmlAttrToWct.find(xmlAttr);
	if (it != p_xmlAttrToWct.end()) {
		return it->second;
		
	} else {
		DEBUG(DBG_GENERAL, "WordClass::getType(): %s not recognized\n", xmlAttr.c_str()); 
		return Undefined;
		
	}
}

/**
 * Static
 */
void WordClass::updateFromXmlNode(sxml::XmlNode* node)
{
	if (!initialized) init();
	
	sxml::NodeSearch* ns = node->findInit("wc");
	sxml::XmlNode* node2 = node->findNext(ns);
	while (node2 != NULL) {
		string s = node2->attributes["t"];
		if (!s.empty()) {
			Type t = getType(s);
			if (t != Undefined || s == "u")
				p_wctToMlString[t].updateFromXmlNode(node2);
				
		}
		
		node2 = node->findNext(ns);
		
	}
	
	node->findFree(ns);
	
}

/**
 * Static
 */
void WordClass::updateShortNamesFromXmlNode(sxml::XmlNode* node)
{
	if (!initialized) init();
	
	sxml::NodeSearch* ns = node->findInit("wc");
	sxml::XmlNode* node2 = node->findNext(ns);
	while (node2 != NULL) {
		string s = node2->attributes["t"];
		if (!s.empty()) {
			Type t = getType(s);
			if (t != Undefined || s == "u")
				p_wctToMlStringShort[t].updateFromXmlNode(node2);
				
		}
		
		node2 = node->findNext(ns);
		
	}
	
	node->findFree(ns);
	
}

} // namespace
