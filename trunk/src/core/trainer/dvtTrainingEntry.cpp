/** @file
 * 
 * Dvt::TrainingEntry - Part of the portable Dictionary and Vocabulary Trainer.
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

#include "dvtTrainingEntry.h"

#include <assert.h>

namespace Dvt
{

TrainingEntry::TrainingEntry(Entry* o, Entry* t)
{
	p_orig = o;
	p_trans = t;
	
	assert(o != NULL && t != NULL);
}

TrainingEntry::~TrainingEntry()
{
	delete p_orig;
	delete p_trans;
}
	
Entry* TrainingEntry::orig() {return p_orig;}
Entry* TrainingEntry::trans() {return p_trans;}
MlString& TrainingEntry::meaning() {return p_meaning;}

WordClass::Type TrainingEntry::wordClass()
{
	return p_orig->wordClass();
}

void TrainingEntry::setWordClass(WordClass::Type wct)
{
	p_orig->setWordClass(wct);
	p_trans->setWordClass(wct);
}

} // namespace
