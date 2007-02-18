/** @file
 * 
 * Dvt::Trainer - Part of the portable Dictionary and Vocabulary Trainer.
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

#include "dvtTrainer.h"

#include "dvtCore.h"
#include "dvtLesson.h"

#include <algorithm>

namespace Dvt
{

using namespace std;

/* QueryPair ******************************************************************/

QueryPair::QueryPair(std::string o, std::string t, int number)
{
	p_orig = o;
	p_trans = t;
	p_number = number;
}

QueryPair::~QueryPair() {}

std::string QueryPair::orig() {return p_orig;}
std::string QueryPair::trans() {return p_trans;}

int QueryPair::number() {return p_number;}
bool QueryPair::isDone() {return p_done;}
void QueryPair::setDone(bool done) {p_done = done;}
bool QueryPair::wasCorrect() {return p_correct;}
void QueryPair::setCorrect(bool correct) {p_correct = correct;}

bool QueryPair::empty()
{
	return p_orig.empty() && p_trans.empty();
}

bool QueryPair::compare(std::string answer)
{
	return p_trans == answer;
}

bool QueryPair::operator==(std::string answer)
{
	return compare(answer);
}

/* Trainer ********************************************************************/

Trainer::Trainer()
{
	core = Core::getInstance();
	p_lessonFile = NULL;
	p_useConj = false;
	p_useDecl = false;
	
	p_done = 0;
	p_correct = 0;
	p_wrong = 0;
}

Trainer::~Trainer()
{
}

LessonFile* Trainer::lessonFile() { return p_lessonFile; }
void Trainer::setLessonFile(LessonFile* lessonFile) { p_lessonFile = lessonFile; } 

LessonList& Trainer::lessons() { return p_lessons; }
void Trainer::setLessons(LessonList& lessonList) 
{
	p_lessons = lessonList;
	p_done = 0;
	p_correct = 0;
	p_wrong = 0;
}
	
bool Trainer::useConj() {return p_useConj;}
void Trainer::setUseConj(bool use) {p_useConj = use;}

bool Trainer::useDecl() {return p_useDecl;}
void Trainer::setUseDecl(bool use) {p_useDecl = use;}

bool Trainer::useHtmlOrig() {return p_useHtmlOrig;}
void Trainer::setUseHtmlOrig(bool use) {p_useHtmlOrig = use;}

Query& Trainer::query() {return p_query;}
	
void Trainer::generateQuery() {};

void Trainer::randomizeQuery()
{
	random_shuffle(p_query.begin(), p_query.end());
}

QueryPair Trainer::currentQueryPair()
{
	if (p_currentQueryPair != p_query.end()) {
		return *p_currentQueryPair;
		
	} else {
		return QueryPair("", "");
		
	}
	
}

QueryPair Trainer::nextQueryPair()
{
	if (p_currentQueryPair != p_query.end()) {
		QueryPair& qp = *p_currentQueryPair;
		qp.setDone(true);
		if (qp.wasCorrect())
			p_correct++;
		else
			p_wrong++;
		p_done++;
		
		p_currentQueryPair++;
		if (p_currentQueryPair == p_query.end())
			return QueryPair("", "");
		else
			return *p_currentQueryPair;
		
	} else {
		return QueryPair("", "");
		
	}
	
}

void Trainer::setCurrentAsWrong()
{
	if (p_currentQueryPair != p_query.end()) {
		QueryPair& qp = *p_currentQueryPair;
		qp.setCorrect(false);
	}
}

void Trainer::setCurrentAsCorrect()
{
	if (p_currentQueryPair != p_query.end()) {
		QueryPair& qp = *p_currentQueryPair;
		qp.setCorrect(true);
	}
}

int Trainer::nbRemaining() {return p_query.size() - p_done;}
int Trainer::nbDone() {return p_done;}
int Trainer::nbCorrect() {return p_correct;}
int Trainer::nbWrong() {return p_wrong;}

} // namespace
