/** @file
 * 
 * Dvt::Language_de - Part of the portable Dictionary and Vocabulary Trainer.
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

#include "dvtLanguage_de.h"

#include "dvtEntry.h"
#include "dvtLanguageProfile.h"

#include "assert.h"

namespace Dvt
{

using namespace std;

Language_de::Language_de(LanguageProfile* lp):Language(lp)
{
}

Language_de::~Language_de()
{
}

string Language_de::getLongName(Entry* e)
{
	assert(e != NULL);
	
	string s;
	
	switch (e->wordClass()) {
		case WordClass::Noun: {
			s = e->baseName();
			if (e->gender() != Gender::Undefined) {
				string a = p_langProfile->getDefArticle(e->gender());
				if (!a.empty() && s.substr(0, a.length()+1) != a+" ")
					s = a + " " + s;
			}
			break;
		}
		default: {
			s = Language::getLongName(e);
			break;
		}
		
	}
	
	return s;
}

string Language_de::getLongNameHtml(Entry* e)
{
	assert(e != NULL);
	
	string s;
	
	switch (e->wordClass()) {
		case WordClass::Noun: {
			s = e->baseName();
			if (e->gender() != Gender::Undefined) {
				string a = p_langProfile->getDefArticle(e->gender());
				if (!a.empty() && s.substr(0, a.length()+1) != a+" ")
					s = a + " " + s;
			}
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
