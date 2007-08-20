/** @file
 * 
 * Dvt::Language_en - Part of the portable Dictionary and Vocabulary Trainer.
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

#include "dvtLanguage_en.h"

#include "dvtEntry.h"
#include "dvtLanguageProfile.h"

#include "assert.h"

namespace Dvt
{

using namespace std;

Language_en::Language_en(LanguageProfile* lp):Language(lp)
{
}

Language_en::~Language_en()
{
}

string Language_en::getLongName(Entry* e)
{
	assert(e != NULL);
	
	string s;
	
	switch (e->wordClass()) {
		case WordClass::Verb: {
			s = e->baseName();
			if (e->baseName().substr(0, 3) != "to ")
				s = "to " + s;
			break;
		}
		default: {
			s = Language::getLongName(e);
			break;
		}
		
	}
	
	return s;
}

string Language_en::getLongNameHtml(Entry* e)
{
	assert(e != NULL);
	
	string s;
	
	switch (e->wordClass()) {
		case WordClass::Verb: {
			s = e->baseName();
			if (e->baseName().substr(0, 3) != "to ")
				s = "to " + s;
			break;
		}
		default: {
			s = Language::getLongNameHtml(e);
			break;
		}
		
	}
	
	return s;
}

}
