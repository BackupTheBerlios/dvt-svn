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

#ifndef DVTENTRY_H_
#define DVTENTRY_H_

#include "dvtWordClass.h"
#include "dvtLanguageProfile.h"

#include "xmlNode.h"

#include <string>
#include <map>

namespace Dvt
{

class Core;
class LanguageProfile;

class Entry
{
private:
	Core* core;
	LanguageProfile* lp;
	std::string p_baseName;
	WordClass::Type p_wc;
	
	Gender::Type p_gender;
	std::map<std::string, std::string> p_decls;
	
	std::string p_conjClass;
	std::map<PersonalPronoun, std::string> p_conjs;
	
public:
	Entry(LanguageProfile* lp, std::string baseName = std::string(), 
		WordClass::Type wc = WordClass::Undefined);
	virtual ~Entry();
	
	LanguageProfile* languageProfile();
	
	std::string baseName();
	void setBaseName(std::string s);
	
	WordClass::Type wordClass();
	void setWordClass(WordClass::Type wct);
	
	Gender::Type gender();
	void setGender(Gender::Type gt);
	
	std::map<std::string, std::string>& decls();
	std::map<PersonalPronoun, std::string>& conjs();
	
	std::string longName();
	std::string longNameHtml();
	
	std::string getDecl(std::string id);
	void setDecl(std::string id, std::string s);
	
	std::string getConj(PersonalPronoun pp);
	void setConj(PersonalPronoun pp, std::string s);
	
	Conjugation* conjClass();
	void setConjClass(Conjugation* cc);
	
	void setFromXmlNode(sxml::XmlNode* node);
	void setToXmlNode(sxml::XmlNode* node);
};

}

#endif /*DVTENTRY_H_*/
