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

#ifndef DVTLANGUAGE_DE_H_
#define DVTLANGUAGE_DE_H_

#include "dvtLanguage.h"

namespace Dvt
{

class Language_de : public Language
{
public:
	Language_de(LanguageProfile* lp);
	virtual ~Language_de();
	
	virtual std::string getLongName(Entry* e);
	virtual std::string getLongNameHtml(Entry* e);
	
};

}

#endif /*DVTLANGUAGE_DE_H_*/
