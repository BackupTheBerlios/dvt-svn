/** @file
 * 
 * Module - Part of the Qt4-GUI of the portable Dictionary and 
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

#ifndef MODULE_H_
#define MODULE_H_

#include "dvtCore.h"

#include <QWidget>
#include <QToolBar>

class MainWindow;

class Module : public QWidget
{
	Q_OBJECT
	
protected:
	MainWindow* mainWindow;
	Dvt::Core* core;
	
public:
	Module(MainWindow* mainWindow, QWidget* parent = 0);
	virtual ~Module();
	
	virtual void setLessonFile(Dvt::LessonFile* lessonFile);
	virtual bool mayClose();
	
	virtual QToolBar* toolBar();
};

#endif /*MODULE_H_*/
