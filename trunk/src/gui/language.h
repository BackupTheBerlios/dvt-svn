/** @file
 * 
 * Language - Part of the Qt4-GUI of the portable Dictionary and 
 * Vocabulary Trainer.
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

#ifndef LANGUAGE_H_
#define LANGUAGE_H_

#include "dvtWordClass.h"
#include "dvtLanguageProfile.h"
#include "dvtEntry.h"

#include <string>

class LessonEdit;
class EditExt;
class EditDetail;

class Language
{
private:
	LessonEdit* lessonEdit;
	Dvt::LanguageProfile* lp;
	
	EditExt* p_editExt;
	EditDetail* p_editDetailVerb;
	
public:
	Language(LessonEdit* ledit, Dvt::LanguageProfile* lp);
	virtual ~Language();
	
	EditExt* editExt();
	EditDetail* getEditDetail(Dvt::WordClass::Type wc);
	
};

#endif /*LANGUAGE_H_*/
