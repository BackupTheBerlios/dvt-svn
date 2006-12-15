/** @file
 * 
 * EditExt - Part of the Qt4-GUI of the portable Dictionary and 
 * Vocabulary Trainer.
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

#ifndef EDITEXT_H_
#define EDITEXT_H_

#include <QWidget>

#include "twiEntry.h"

#include "dvtCore.h"
#include "dvtEntry.h"
#include "dvtWordClass.h"

class LessonEdit;

class EditExt : public QWidget
{
	Q_OBJECT
	
protected:
	LessonEdit* lessonEdit;
	Dvt::LanguageProfile* lp;
	Dvt::Core* core; 
	
public:
	EditExt(LessonEdit* ledit, Dvt::LanguageProfile* lp, QWidget* parent = 0);
	virtual ~EditExt();
	
	virtual void setNextTabWidget(QWidget* w);
	
public slots:
	virtual void changeCurrentEntry(TwiEntry* entry);
	virtual void postCurrentEntry(TwiEntry* entry);
	
};

#endif /*EDITEXT_H_*/
