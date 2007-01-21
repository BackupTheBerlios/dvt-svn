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

#ifndef DVTWORDCLASS_H_
#define DVTWORDCLASS_H_

#include "dvtMlString.h"

#include "xmlNode.h"

#include <map>
#include <string>

namespace Dvt
{

class Gender
{
public:
	typedef enum {
		Undefined,
		Male,
		Female,
		Neuter,
		High
	} Type;

private:
	static bool initialized;
	
	static std::map<Type, MlString> p_gtToMlString;
	static std::map<Type, MlString> p_gtToMlStringShort;
	static std::map<Gender::Type, std::string> p_typeToXmlAttr;
	static std::map<std::string, Gender::Type> p_xmlAttrToType;
	
	static void init();
	
public:
	static std::string getString(Type type);
	static std::string getShortString(Type type);
	static std::string getXmlAttr(Type type);
	static Type getType(std::string xmlAttr);
	
	static void updateFromXmlNode(sxml::XmlNode* node);
	static void updateShortNamesFromXmlNode(sxml::XmlNode* node);
	
};

/**
 * Word classes
 */
class WordClass
{
public:
	typedef enum {
		Undefined,		///< default
		Noun,				
		Verb,				
		Adjective,		
		Adverb,
		Pronoun,
		Preposition,		
		Conjunction,
		Interjection,
		Numeral,
		Phrase,
		NonNounNonVerb,	///< if unsure
		High			///< last element of enumeration
	} Type;
	
private:
	static bool initialized;

	static std::map<Type, MlString> p_wctToMlString;
	static std::map<Type, MlString> p_wctToMlStringShort;
	static const char* p_wctToXmlAttr[];
	static std::map<std::string, Type> p_xmlAttrToWct;
	
	static void init();
	
public:
	static std::string getString(Type wc);
	static std::string getShortString(Type wc);
	static std::string getXmlAttr(Type wc);
	static Type getType(const std::string xmlAttr);
	
	static void updateFromXmlNode(sxml::XmlNode* node);
	static void updateShortNamesFromXmlNode(sxml::XmlNode* node);
	
};

}

#endif /*DVTWORDCLASS_H_*/
