/** @file
 * 
 * FlashCardTrainer - Part of the Qt4-GUI of the portable Dictionary and 
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

#ifndef FLASHCARDTRAINER_H_
#define FLASHCARDTRAINER_H_

#include "ui_flashCardTrainer.h"
#include "module.h"

#include "dvtFlashCardTrainer.h"
#include "dvtLessonFile.h"
#include "dvtLesson.h"

class FlashCardTrainer : public Module, public Ui::FlashCardTrainer
{
	Q_OBJECT
	
private:
	Dvt::FlashCardTrainer* trainer;
	Dvt::LessonFile* lessonFile;
	
	bool cardIsFront;
	
private slots:
	void setCard(bool front, QString text);
	void setNext();
	
	void on_actionTurn_triggered(bool checked = false);
	void on_actionKnown_triggered(bool checked = false);
	void on_actionNotKnown_triggered(bool checked = false);
	
public:
	FlashCardTrainer(MainWindow* mainWindow, QWidget* parent = 0);
	virtual ~FlashCardTrainer();
	
	virtual void setLessonFile(Dvt::LessonFile* lesson);
	virtual bool mayClose();
	
};

#endif /*FLASHCARDTRAINER_H_*/
