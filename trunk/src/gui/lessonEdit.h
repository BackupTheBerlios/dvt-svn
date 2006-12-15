/** @file
 * 
 * LessonEdit - Part of the Qt4-GUI of the portable Dictionary and 
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

#ifndef LESSONEDIT_H_
#define LESSONEDIT_H_

#include "ui_lessonEdit.h"

#include "module.h"
#include "twiEntry.h"

#include "dvtLesson.h"
#include "dvtWordClass.h"

#include <QLabel>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QFocusEvent>
#include <QTableWidgetItem>
#include <QScrollArea>

class Language;
class EditExt;
class TwiCellEntry;
class TwiEntry;

class LessonEdit : public Module, public Ui::LessonEdit
{
	Q_OBJECT
	
private:
	Dvt::Lesson* lesson;
	Language* foreignLang;
	Language* nativeLang;
	
	TwiEntry* currentEntry;
	
	QScrollArea* saDetails;
	
	bool edited;
	
	void setEntry(Dvt::TrainingEntry* te);
	void setTwiEntry(TwiEntry* te);
	
	void postCurrentEntry(bool recursive = true);
	
	void resetTabOrder();
	
public:
	LessonEdit(MainWindow* mainWindow, QWidget* parent = 0);
	virtual ~LessonEdit();
	
	virtual void setLesson(Dvt::Lesson* lesson);
	virtual bool mayClose();
	
	void setDetail(Dvt::WordClass::Type wc);
	
	void setEdited(bool edited);
	
	void emitCurrentEntryChanged();
	void emitCurrentEntryPosted();
	
protected:
	virtual void showEvent(QShowEvent* e);
	
public slots:
	void on_twEntries_currentItemChanged(QTableWidgetItem* current, QTableWidgetItem* previous);
	
	void on_cboxWordClass_currentIndexChanged(int index);
	
	void on_actionNew_triggered(bool checked = false);
	void on_actionDelete_triggered(bool checked = false);
	void on_actionSave_triggered(bool checked = false);
	
	void on_pbNew_clicked(bool checked = false);
	void on_pbDelete_clicked(bool checked = false);
	void on_pbSave_clicked(bool checked = false);
	
	void on_tabWidget_currentChanged(int index);
	
signals:
	void currentEntryChanged(TwiEntry* entry);
	void currentEntryPosted(TwiEntry* entry);
	
};

#endif /*LESSONEDIT_H_*/
