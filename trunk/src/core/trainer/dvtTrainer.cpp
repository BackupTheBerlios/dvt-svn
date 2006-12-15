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

QueryPair::QueryPair(std::string o, std::string t)
{
	p_orig = o;
	p_trans = t;
}

QueryPair::~QueryPair() {}

std::string QueryPair::orig() {return p_orig;}
std::string QueryPair::trans() {return p_trans;}

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
	p_lesson = NULL;
	p_useConj = false;
	p_useDecl = false;
}

Trainer::~Trainer()
{
}

Lesson* Trainer::lesson() {return p_lesson;}
void Trainer::setLesson(Lesson* lesson) {p_lesson = lesson;}
	
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

} // namespace
