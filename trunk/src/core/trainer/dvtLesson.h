/** @file
 * 
 * Dvt::Lesson - Part of the portable Dictionary and Vocabulary Trainer.
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

#ifndef DVTLESSON_H_
#define DVTLESSON_H_

#include "dvtMlString.h"
#include "dvtTrainingEntry.h"

#include <string>
#include <vector>

namespace Dvt
{

class Core;
class LanguageProfile;

class Lesson
{
private:
	Core* core;
	
	LanguageProfile* p_langProfile_o;
	LanguageProfile* p_langProfile_t;

	int p_number;
	MlString p_title;
	
	std::vector<TrainingEntry*> p_entries;
	
public:
	Lesson(int number, LanguageProfile* orig = NULL, LanguageProfile* trans = NULL);
	virtual ~Lesson();
	
	LanguageProfile* langProfile_o();
	void setLangProfile_o(LanguageProfile*);
	LanguageProfile* langProfile_t();
	void setLangProfile_t(LanguageProfile*);
	
	int number();
	MlString& title();
	
	std::vector<TrainingEntry*>& entries();
	
	// methods
	
	TrainingEntry* addEntry();
	void deleteEntry(TrainingEntry* te);
	
	void setFromXmlNode(sxml::XmlNode* lessonNode);
	void setToXmlNode(sxml::XmlNode* lessonNode);
	
	void unloadData();
};

}

#endif /*DVTLESSON_H_*/
