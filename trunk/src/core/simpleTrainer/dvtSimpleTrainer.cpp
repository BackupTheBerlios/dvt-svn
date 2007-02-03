/** @file
 * 
 * Dvt::SimpleTrainer - Part of the portable Dictionary and Vocabulary Trainer.
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

#include "dvtSimpleTrainer.h"

#include "dvtCore.h"
#include "dvtLesson.h"
#include "dvtWordClass.h"
#include "dvtLanguageProfile.h"

#include "debug.h"

namespace Dvt
{

using namespace std;

SimpleTrainer::SimpleTrainer():Trainer()
{
}

SimpleTrainer::~SimpleTrainer()
{
}

void SimpleTrainer::generateQuery()
{
	p_query.clear();
	
	LanguageProfile* lp = p_lesson->langProfile_t();
	assert(lp != NULL);
	
	string s, base, t, s2, wc;
	vector<TrainingEntry*>::const_iterator it;
	for (it = p_lesson->entries().begin(); it != p_lesson->entries().end(); it++) {
		TrainingEntry* te = *it;
		base = te->trans()->baseName();
		s = te->orig()->baseName();
		wc = "";
		if (te->wordClass() != WordClass::Undefined) {
			wc = WordClass::getShortString(te->wordClass()) + ".";
			if (p_useHtmlOrig) {
				wc = "  <i>" + wc + "</i>";
				s = s + wc;
			} else {
				wc = "  " + wc;
				s = s + wc;
			}
		}
		
		p_query.push_back(QueryPair(s, base));
			
		if (p_useDecl) {
			map<string, string>& dcls = te->trans()->decls();
			map<string, string>::const_iterator dcls_it;
			for (dcls_it = dcls.begin(); dcls_it != dcls.end(); dcls_it++) {
				t = dcls_it->second;
				if (t != "") {
					if (t[0] == '-') t.replace(0, 1, base);
					string d = lp->getCase(dcls_it->first)->name().str();
					if (p_useHtmlOrig)
						s2 = s + " <i>(" + d + ")</i>";
					else
						s2 = s + " (" + d + ")";
					p_query.push_back(QueryPair(s2, t));
				}
				
			}
			
		}
		
		if (p_useConj) {
			for (int pp = (int) ppSingular1; pp < (int) ppHigh; pp++) {
				t = te->trans()->getConj((PersonalPronoun) pp);
				if (!t.empty()) {
					string p = lp->getPersonalPronoun((PersonalPronoun) pp);
					if (p_useHtmlOrig)
						s2 = s + " <i>(" + p + ")</i>";
					else
						s2 = s + " (" + p + ")";
					p_query.push_back(QueryPair(s2, t));
				}
				
			}
			
		}
		
	}
	
	randomizeQuery();
	
	p_currentQueryPair = p_query.begin();
}

} // namespace
