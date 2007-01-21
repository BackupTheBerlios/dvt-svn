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
#include <iostream>

namespace Dvt
{

#define DVT_DTL_XML_FORMATVERSION	"1.0"

typedef enum {
	rfReadAll,
	rfReadMetaDataOnly,
	rfReadData
} ReadFlag;

class Core;
class LanguageProfile;

class Lesson
{
private:
	Core* core;
	
	bool p_metaRead;
	bool p_dataRead;
	
	std::string p_fileName;
	
	LanguageProfile* p_langProfile_o;
	LanguageProfile* p_langProfile_t;
	std::string p_version;

	MlString p_title;
	MlString p_description;
	MlString p_license;
	
	std::vector<TrainingEntry*> p_entries;
	
public:
	Lesson(bool newLesson = false);
	virtual ~Lesson();
	
	// tag attributes
	LanguageProfile* langProfile_o();
	void setLangProfile_o(LanguageProfile*);
	LanguageProfile* langProfile_t();
	void setLangProfile_t(LanguageProfile*);
	
	std::string version();
	
	bool metaRead();
	bool dataRead();
	
	// meta data
	MlString& title();
	MlString& description();
	MlString& license();
	
	std::vector<TrainingEntry*>& entries();
	
	TrainingEntry* addEntry();
	void deleteEntry(TrainingEntry* te);
	
	// methods
	
	void readFromStream(std::istream& readFrom, const ReadFlag readFlag = rfReadAll);
	void readFromFile(const std::string& fileName, const ReadFlag readFlag = rfReadAll);
	
	void readData();
	void unloadData();
	
	void write();
	void writeToStream(std::ostream& writeTo);
	void writeToFile(const std::string& fileName);
};

}

#endif /*DVTLESSON_H_*/
