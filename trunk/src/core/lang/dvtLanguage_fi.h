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

#ifndef DVTLANGUAGE_FI_H_
#define DVTLANGUAGE_FI_H_

#include "dvtLanguage.h"

namespace Dvt
{

class Language_fi : public Language
{
public:
	Language_fi(LanguageProfile* lp);
	virtual ~Language_fi();
	
	virtual std::string getLongName(Entry* e);
	virtual std::string getLongNameHtml(Entry* e);
};

}

#endif /*DVTLANGUAGE_FI_H_*/
