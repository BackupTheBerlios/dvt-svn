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

namespace Dvt
{

class Lesson;
class Core;

class QueryPair
{
protected:
	std::string	p_orig;
	std::string p_trans;
	
public:
	QueryPair(std::string o, std::string t);
	virtual ~QueryPair();
	
	virtual std::string orig();
	virtual std::string trans();
	
	virtual bool compare(std::string answer);
	
	virtual bool operator==(std::string answer);
	
};

typedef std::vector<QueryPair>	Query;

class Trainer
{
protected:
	Core* core;
	Lesson* p_lesson;
	
	bool p_useConj;
	bool p_useDecl;
	bool p_useHtmlOrig;
	
	Query p_query;
	
public:
	Trainer();
	virtual ~Trainer();
	
	virtual Lesson* lesson();
	virtual void setLesson(Lesson* lesson);
	
	virtual bool useConj();
	virtual void setUseConj(bool use);
	
	virtual bool useDecl();
	virtual void setUseDecl(bool use);
	
	virtual bool useHtmlOrig();
	virtual void setUseHtmlOrig(bool use);
	
	virtual Query& query();
	
	virtual void generateQuery();
	virtual void randomizeQuery();
	 
};

}

#endif /*DVTTRAINER_H_*/
