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

#include "dvtCore.h"

#include "dvtExceptions.h"
#include "dvtLanguageProfile.h"
#include "dvtLesson.h"

#include "debug.h"

#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

namespace Dvt {

using namespace std;

Core* Core::p_instance = NULL;
list<string> Core::p_languagePrefOrder;
bool Core::p_prettyXmlOutput = true;

Core::Core()
{
}

Core::~Core()
{
}

Core* Core::getInstance()
{
	if (p_instance == NULL)
		p_instance = new Core();
	
	return p_instance;
}

void Core::init()
{
	loadLanguageProfiles("lang");
	loadLessons("lessons");
}

bool Core::prettyXmlOutput() {return p_prettyXmlOutput;}
void Core::setPrettyXmlOutput(bool b) {p_prettyXmlOutput = b;}
list<string>& Core::languagePrefOrder() {return p_languagePrefOrder;}

std::string Core::systemLocale() {return p_systemLocale;}
map<string, LanguageProfile*> Core::languageProfiles() {return p_languageProfiles;}
std::vector<Lesson*> Core::lessons() {return p_lessons;}

void Core::setSystemLocale(std::string name)
{
	p_systemLocale = name;
	if (p_languagePrefOrder.size() == 0) {
		p_languagePrefOrder.push_back(name);
		if (name.size() > 2) {
			// assume sth like fr_FR
			name = name.substr(0, 2);
			p_languagePrefOrder.push_back(name);
			
		}
		
	}
}

LanguageProfile* Core::getLanguageProfile(std::string langCode)
{
	map<string, LanguageProfile*>::iterator it;
	
	it = p_languageProfiles.find(langCode);
	if (it != p_languageProfiles.end())
		return it->second;
	else
		return NULL;
}

/**
 * Read in language profiles
 */
void Core::loadLanguageProfiles(const string& dirName) {
	DIR* dir = NULL;
	struct dirent* dirEntry = NULL;
	
//	DEBUG(DBG_GENERAL, "Dvt::Core::loadLanguageProfiles(): reading %s\n", dirName.c_str());
	
	errno = 0;
	dir = opendir(dirName.c_str());
	if (dir == NULL) {
		DEBUG(DBG_GENERAL, "Dvt::Core::loadLanguageProfiles(): Error %i opening directory %s\n", errno, dirName.c_str());
		throw EFileError(dirName.c_str());
	}
	
	try {
		while ((dirEntry = readdir(dir))) {
			string s = dirEntry->d_name;
			
			if (s != "." && s != "..") {
				// check, whether it is a file
				struct stat entry_stat;
				memset(&entry_stat, 0, sizeof(entry_stat));
				string fullName = dirName + DVT_DIR_SEPARATOR + s;
				if (stat(fullName.c_str(), &entry_stat) != 0) {
					DEBUG(DBG_GENERAL, "Dvt::Core::loadLanguageProfiles(): stat errno %i: %s\n", 
						errno, strerror(errno));
				}
				if (!S_ISDIR(entry_stat.st_mode)) {
					// a file
					if (s.length() > 4 && (strcasecmp(s.substr(s.length() - 4, 4).c_str(), DVT_LANGUAGE_PROFILE_FILE_SUFFIX) == 0)) {
						LanguageProfile* lp = new LanguageProfile();
						try {
							lp->readFromFile(dirName + DVT_DIR_SEPARATOR + s);
						} catch (Exception e) {
							delete lp;
							throw;
						}
						if (lp->langCode() != "") { 
							p_languageProfiles[lp->langCode()] = lp;
//							DEBUG(DBG_GENERAL, 
//								"Dvt::Core::loadLanguageProfiles(): found %s (%s)\n", 
//								lp->langCode().c_str(), lp->name().c_str());
							
						} else {
							delete lp;
							
						}
						
					}
				}
			}
		}
	} catch (Exception& e) {
		DEBUG(DBG_GENERAL, "Dvt::Core::loadLanguageProfiles(): Caught exception. ec = %i, msg = \"%s\"\n", e.code, e.msg.c_str());
		closedir(dir);
		throw;
	}
	
	closedir(dir);
}

/**
 * Read in training lessons
 */
void Core::loadLessons(const string& dirName) {
	DIR* dir = NULL;
	struct dirent* dirEntry = NULL;
	
//	DEBUG(DBG_GENERAL, "Dvt::Core::loadTrainingLessons(): reading %s\n", 
//		dirName.c_str());
	
	errno = 0;
	dir = opendir(dirName.c_str());
	if (dir == NULL) {
		DEBUG(DBG_GENERAL, "Dvt::Core::loadTrainingLessons(): Error %i opening directory %s\n", 
			errno, dirName.c_str());
		throw EFileError(dirName.c_str());
	}
	
	try {
		while ((dirEntry = readdir(dir))) {
			string s = dirEntry->d_name;
			
			if (s != "." && s != "..") {
				// check, whether it is a file
				struct stat entry_stat;
				memset(&entry_stat, 0, sizeof(entry_stat));
				string fullName = dirName + DVT_DIR_SEPARATOR + s;
				if (stat(fullName.c_str(), &entry_stat) != 0) {
					DEBUG(DBG_GENERAL, "Dvt::Core::loadTrainingLessons(): stat errno %i\n", errno);
				}
				if (!S_ISDIR(entry_stat.st_mode)) {
					// a file
					if (s.length() > 4 && (strcasecmp(s.substr(s.length() - 4, 4).c_str(), DVT_TRAINING_LESSON_FILE_SUFFIX) == 0)) {
						Lesson* tl = new Lesson();
						try {
							tl->readFromFile(dirName + DVT_DIR_SEPARATOR + s, rfReadMetaDataOnly);
						} catch (Exception e) {
							delete tl;
							throw;
						}
//						DEBUG(DBG_GENERAL, 
//							"Dvt::Core::loadTrainingLessons(): found lesson %s\n",
//							tl->title().c_str());
						p_lessons.push_back(tl);
					}
				}
			}
		}
	} catch (Exception& e) {
		DEBUG(DBG_GENERAL, 
			"Dvt::Core::loadTrainingLessons(): Caught exception. ec = %i, msg = \"%s\"\n", 
			e.code, e.msg.c_str());
		closedir(dir);
		throw;
	}
	
	closedir(dir);
}

/**
 * Create a new lesson
 */
Dvt::Lesson* Core::createLesson()
{
	Dvt::Lesson* lesson = new Dvt::Lesson(true);
	p_lessons.push_back(lesson);
	return lesson;
}

} // namespace
