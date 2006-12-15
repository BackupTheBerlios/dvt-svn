/** @file
 * 
 * Dvt::Language_fi - Part of the portable Dictionary and Vocabulary Trainer.
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

#include "dvtLanguage_fi.h"

#include "dvtEntry.h"
#include "dvtLanguageProfile.h"

namespace Dvt
{

using namespace std;

#define GEN_ID	"2"
#define PART_ID	"3"

Language_fi::Language_fi(LanguageProfile* lp):Language(lp)
{
}

Language_fi::~Language_fi()
{
}

string Language_fi::getLongName(Entry* e)
{
	assert(e != NULL);
	
	string s;
	
	switch (e->wordClass()) {
		case WordClass::Verb: {
			s = e->baseName();
			string fps = e->getConj(ppSingular1);
			if (!fps.empty()) {
				fps = p_langProfile->getPersonalPronoun(ppSingular1) + " " + fps;
				s = s + " (" + fps + ")";
			}
			break;
		}
		default: {
			s = e->baseName();
			
			string gen = e->getDecl(GEN_ID);
			string part = e->getDecl(PART_ID);
			if (!gen.empty()) {
				s += " (" + gen;
				if (!part.empty())
					s += ", " + part;
				s += ")";
			}
			
			if (e->wordClass() != WordClass::Undefined)
				s += "  " + WordClass::getShortString(e->wordClass()) + ".";
			
			break;
		}
		
	}
	
	return s;
}

string Language_fi::getLongNameHtml(Entry* e)
{
	assert(e != NULL);
	
	string s;
	
	switch (e->wordClass()) {
		case WordClass::Verb: {
			s = e->baseName();
			string fps = e->getConj(ppSingular1);
			if (!fps.empty()) {
				fps = p_langProfile->getPersonalPronoun(ppSingular1) + " " + fps;
				s = s + " <i>(" + fps + ")</i>";
			} else {
				s += "  <i>" + WordClass::getShortString(e->wordClass()) + ".</i>";
			}
			break;
		}
		default: {
			s = e->baseName();
			
			string gen = e->getDecl(GEN_ID);
			string part = e->getDecl(PART_ID);
			if (!gen.empty()) {
				s += " <i>(" + gen;
				if (!part.empty())
					s += ", " + part;
				s += ")</i>";
			}
			
			if (e->wordClass() != WordClass::Undefined)
				s += "  <i>" + WordClass::getShortString(e->wordClass()) + ".</i>";
			
			break;
		}
		
	}
	
	return s;
}

} // namespace
