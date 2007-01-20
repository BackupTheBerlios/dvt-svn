/** @file
 * 
 * LessonSelect - Part of the Qt4-GUI of the portable Dictionary and 
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

#ifndef LESSONSELECT_H_
#define LESSONSELECT_H_

#include "ui_lessonSelect.h"

#include "module.h"

#include "dvtCore.h"
#include "dvtLesson.h"
#include "dvtLanguageProfile.h"

#include <QWidget>
#include <QListWidgetItem>

class LwiLesson : public QListWidgetItem
{
public:
	Dvt::Lesson* lesson;
	
	LwiLesson(Dvt::Lesson* lesson, QListWidget* parent = 0, int type = Type)
		:QListWidgetItem(parent, type)
	{
		this->lesson = lesson;
		setText(QString("%1 (%2)")
			.arg(QString::fromUtf8(lesson->title().c_str()))
			.arg(QString::fromUtf8(lesson->langProfile_t()->name().c_str()))
		);
	};
	
};

class LessonSelect : public Module, public Ui::LessonSelect
{
	Q_OBJECT
	
public:
	Dvt::Lesson* lesson;

	LessonSelect(MainWindow* mainWindow, QWidget* parent = 0);
	virtual ~LessonSelect();
	
	void loadLessons();
	
	void setLesson(Dvt::Lesson* lesson);
	
protected:
	virtual void showEvent(QShowEvent* e);
	
public slots:
	void on_lwLessons_currentItemChanged(QListWidgetItem* item, QListWidgetItem* prev);
	void on_pbEdit_clicked(bool pressed = false);
	void on_pbNew_clicked(bool pressed = false);
	void on_pbTrain_clicked(bool pressed = false);
	
};

#endif /*LESSONSELECT_H_*/
