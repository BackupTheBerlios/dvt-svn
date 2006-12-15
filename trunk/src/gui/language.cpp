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

#include "language.h"

#include "lessonEdit.h"

#include "editExt.h"
#include "editExt_fi.h"

#include "editDetail.h"
#include "editDetailVerb.h"

using namespace std;

Language::Language(LessonEdit* ledit, Dvt::LanguageProfile* lp)
{
	this->lp = lp;
	this->lessonEdit = ledit;
	
	p_editExt = NULL;
	p_editDetailVerb = NULL;
	
	if (lp->langCode() == "fi") {
		p_editExt = new EditExt_fi(lessonEdit, lp);
		p_editDetailVerb = new EditDetailVerb(lessonEdit, lp);
	
	} else {
		p_editDetailVerb = new EditDetailVerb(lessonEdit, lp);
		
	}
}

Language::~Language()
{
	if (p_editExt != NULL) delete p_editExt;
	if (p_editDetailVerb != NULL) delete p_editDetailVerb;
}

EditExt* Language::editExt()
{
	return p_editExt;
}

EditDetail* Language::getEditDetail(Dvt::WordClass::Type wc)
{
	switch (wc) {
		case Dvt::WordClass::Verb: return p_editDetailVerb;
		default: return NULL;
	}
}
