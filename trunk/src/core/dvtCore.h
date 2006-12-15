/** @file
 * 
 * Dvt::Core - Part of the portable Dictionary and Vocabulary Trainer.
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

#ifndef DVTCORE_H_
#define DVTCORE_H_

#include <list>
#include <map>
#include <vector>
#include <string>

#define DVT_XML_NAMESPACE					"http://www.delta-my.de/divotra"
#define DVT_XSD_LOCATION					"http://www.delta-my.de/divotra/dtl.xsd"
#define DVT_TRAINING_LESSON_FILE_SUFFIX		".dtl"
#define DVT_LANGUAGE_PROFILE_FILE_SUFFIX	".dlp"

#define DVT_DIR_SEPARATOR	"/"

#define DVT_DEBUG

namespace Dvt {

class LanguageProfile;
class Lesson;

/**
 * Dvt::Core
 */
class Core
{
private:
	static Core* p_instance;
	static bool p_prettyXmlOutput;
	static std::list<std::string> p_languagePrefOrder;
	
	std::map<std::string, LanguageProfile*> p_languageProfiles;
	std::vector<Lesson*> p_lessons;
	
	Core();
	
public:
	virtual ~Core();
	static Core* getInstance();
	
	void init();
	
	static bool prettyXmlOutput();
	static void setPrettyXmlOutput(bool);
	
	static std::list<std::string>& languagePrefOrder();
	
	std::map<std::string, LanguageProfile*> languageProfiles();
	std::vector<Lesson*> lessons();
	
	LanguageProfile* getLanguageProfile(std::string langCode);
	
	void loadLanguageProfiles(const std::string& dirName);
	void loadLessons(const std::string& dirName);
};

}

#endif /*DVTCORE_H_*/
