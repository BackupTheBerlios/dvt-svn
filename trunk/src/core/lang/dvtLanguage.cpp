/** @file
 * 
 * Dvt::Language - Part of the portable Dictionary and Vocabulary Trainer.
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

#include "dvtLanguage.h"

#include "dvtExceptions.h"
#include "dvtEntry.h"
#include "dvtLanguageProfile.h"

#include "debug.h"

#include "assert.h"

namespace Dvt
{

using namespace std;

/* Language *******************************************************************/

Language::Language(LanguageProfile* lp)
{
	assert(lp != NULL);
	p_langProfile = lp;
}

Language::~Language()
{
}

LanguageProfile* Language::langProfile() {return p_langProfile;}

string Language::getLongName(Entry* e)
{
	assert(e != NULL);
	
	string s;
	
	switch (e->wordClass()) {
		case WordClass::Noun: {
			s = e->baseName();
			if (e->gender() != Gender::Undefined)
				s += " (" + Gender::getShortString(e->gender()) + ")";
			else
				s += "  " + WordClass::getShortString(e->wordClass()) + ".";
			break;
		}
		default: {
			s = e->baseName();
			if (e->wordClass() != WordClass::Undefined)
				s += "  " + WordClass::getShortString(e->wordClass()) + ".";
			break;
		}
		
	}
	
	return s;
}

string Language::getLongNameHtml(Entry* e)
{
	assert(e != NULL);
	
	string s;
	
	switch (e->wordClass()) {
		case WordClass::Noun: {
			s = e->baseName();
			if (e->gender() != Gender::Undefined)
				s += " (" + Gender::getShortString(e->gender()) + ")";
			else
				s += "  <i>" + WordClass::getShortString(e->wordClass()) + ".</i>";
			break;
		}
		default: {
			s = e->baseName();
			if (e->wordClass() != WordClass::Undefined)
				s += "  <i>" + WordClass::getShortString(e->wordClass()) + ".</i>";
			break;
		}
		
	}
	
	return s;
}

} // namespace
