/** @file
 * 
 * Dvt::MlString - Part of the portable Dictionary and Vocabulary Trainer.
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

#ifndef DVTMLSTRING_H_
#define DVTMLSTRING_H_

#include "xmlNode.h"

#include <string>
#include <map>

namespace Dvt
{

/**
 * Multi-language string
 */
class MlString
{
private:
	std::map<std::string, std::string> translations;
	
public:
	MlString();
	virtual ~MlString();
	
	void setString(const std::string& langId, const std::string& str);
	std::string getString(const std::string& langId);
	
	void setFromXmlNode(sxml::XmlNode* node);
	void setToXmlNode(sxml::XmlNode* node);
	
	std::string str();
	const char* c_str();
	
	bool empty();
	
	operator std::string ();
	std::string operator [] (const std::string& langId);
};

}

#endif /*DVTMLSTRING_H_*/
