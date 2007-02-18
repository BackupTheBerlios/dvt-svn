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

#ifndef DVTTRAINER_H_
#define DVTTRAINER_H_

#include <string>
#include <vector>
#include <list>

namespace Dvt
{

class LessonFile;
class Lesson;
class Core;

class QueryPair
{
protected:
	std::string	p_orig;
	std::string p_trans;
	
	int p_number;
	bool p_done;
	bool p_correct;
	
public:
	QueryPair(std::string o, std::string t, int number = 0);
	virtual ~QueryPair();
	
	virtual std::string orig();
	virtual std::string trans();
	
	virtual int number();
	
	virtual bool isDone();
	virtual void setDone(bool done);
	virtual bool wasCorrect();
	virtual void setCorrect(bool correct);
	
	virtual bool empty();
	
	virtual bool compare(std::string answer);
	
	virtual bool operator==(std::string answer);
	
};

typedef std::vector<QueryPair>	Query;
typedef std::list<Lesson*> LessonList;

class Trainer
{
protected:
	Core* core;
	LessonFile* p_lessonFile;
	LessonList p_lessons;
	
	bool p_useConj;
	bool p_useDecl;
	bool p_useHtmlOrig;
	
	int p_done;
	int p_correct;
	int p_wrong;
	
	Query p_query;
	Query::iterator p_currentQueryPair;
	
public:
	Trainer();
	virtual ~Trainer();
	
	virtual LessonFile* lessonFile();
	virtual void setLessonFile(LessonFile* lessonFile);
	
	virtual LessonList& lessons();
	virtual void setLessons(LessonList& lessonList);
	
	virtual bool useConj();
	virtual void setUseConj(bool use);
	
	virtual bool useDecl();
	virtual void setUseDecl(bool use);
	
	virtual bool useHtmlOrig();
	virtual void setUseHtmlOrig(bool use);
	
	virtual Query& query();
	
	virtual void generateQuery();
	virtual void randomizeQuery();
	
	virtual QueryPair currentQueryPair();
	virtual QueryPair nextQueryPair();
	
	virtual void setCurrentAsWrong();
	virtual void setCurrentAsCorrect();
	
	virtual int nbRemaining();
	virtual int nbDone();
	virtual int nbCorrect();
	virtual int nbWrong();
	 
};

}

#endif /*DVTTRAINER_H_*/
