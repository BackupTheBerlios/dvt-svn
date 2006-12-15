/** @file
 * 
 * Dvt::Language_fr - Part of the portable Dictionary and Vocabulary Trainer.
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

#include "dvtLanguage_fr.h"

#include "dvtEntry.h"
#include "dvtLanguageProfile.h"

namespace Dvt
{

using namespace std;

Language_fr::Language_fr(LanguageProfile* lp):Language(lp)
{
	p_vocals.push_back("a");
	p_vocals.push_back("e");
	p_vocals.push_back("i");
	p_vocals.push_back("o");
	p_vocals.push_back("u");
	p_vocals.push_back("h");
	
	p_vocals.push_back("à");
	p_vocals.push_back("è");
	p_vocals.push_back("á");
	p_vocals.push_back("é");
	
	p_vocals.push_back("â");
	p_vocals.push_back("ê");
	p_vocals.push_back("î");
	p_vocals.push_back("ô");
	p_vocals.push_back("û");
	
}

Language_fr::~Language_fr()
{
}

string Language_fr::getLongName(Entry* e)
{
	assert(e != NULL);
	
	string s;
	
	switch (e->wordClass()) {
		case WordClass::Noun: {
			s = e->baseName();
			if (e->gender() != Gender::Undefined) {
				if (beginsWithVocal(e)) {
					if (s.substr(0, 2) != "l'")
						s = "l'" + s; 
					s += " (" + Gender::getShortString(e->gender()) + ")";
					
				} else {
					string a = p_langProfile->getDefArticle(e->gender());
					if (!a.empty() && s.substr(0, a.length()+1) != a+" ")
						s = a + " " + s;
					
				}
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

string Language_fr::getLongNameHtml(Entry* e)
{
	assert(e != NULL);
	
	string s;
	
	switch (e->wordClass()) {
		case WordClass::Noun: {
			s = e->baseName();
			if (e->gender() != Gender::Undefined) {
				if (beginsWithVocal(e)) {
					if (s.substr(0, 2) != "l'")
						s = "l'" + s; 
					s += " (" + Gender::getShortString(e->gender()) + ")";
					
				} else {
					string a = p_langProfile->getDefArticle(e->gender());
					if (!a.empty() && s.substr(0, a.length()+1) != a+" ")
						s = a + " " + s;
					
				}
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

bool Language_fr::beginsWithVocal(Entry* e)
{
	vector<string>::const_iterator it;
	string s = e->baseName();
	
	for (it = p_vocals.begin(); it != p_vocals.end(); it++) {
		if (*it == s.substr(0, it->length()))
			return true;
	}
	
	return false;
}

}
