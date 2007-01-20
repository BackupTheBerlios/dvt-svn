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

#include "lessonSelect.h"

#include "mainWindow.h"
#include "utils.h"
#include "dlgLessonMetaEdit.h"

#include "dvtLanguageProfile.h"

using namespace std;

LessonSelect::LessonSelect(MainWindow* mainWindow, QWidget* parent)
	:Module(mainWindow, parent)
{
	setupUi(this);
	
	lbFlagOrig->setText("");
	lbFlagTrans->setText("");
	
	setLesson(NULL);
	pbTrain->setEnabled(false);
}

LessonSelect::~LessonSelect()
{
}

void LessonSelect::loadLessons()
{
	lwLessons->clear();
	
	vector<Dvt::Lesson*> lessons = core->lessons();
	vector<Dvt::Lesson*>::iterator it;
	for (it = lessons.begin(); it != lessons.end(); it++) {
		LwiLesson* lwil = new LwiLesson(*it, lwLessons);
		if (*it == lesson)
			lwLessons->setCurrentItem(lwil);
	}
}

void LessonSelect::setLesson(Dvt::Lesson* lesson)
{
	this->lesson = lesson;
	
	if (lesson != NULL) {
		QIcon* icon;
		
		lbLangOrig->setText(STR2QSTR(lesson->langProfile_o()->name()));
		if ((icon = mainWindow->flags[lesson->langProfile_o()->langCode().c_str()]))
			lbFlagOrig->setPixmap(icon->pixmap(icon->actualSize(QSize(23,14))));
		else
			lbFlagOrig->setPixmap(QPixmap());
			
		lbLangTrans->setText(STR2QSTR(lesson->langProfile_t()->name()));
		if ((icon = mainWindow->flags[lesson->langProfile_t()->langCode().c_str()]))
			lbFlagTrans->setPixmap(icon->pixmap(icon->actualSize(QSize(23,14))));
		else
			lbFlagTrans->setPixmap(QPixmap());
		
		lbDesc->setText(STR2QSTR(lesson->description()));
		
	} else {
		lbLangOrig->setText("");
		lbLangTrans->setText("");
		lbDesc->setText("");
		
	}
	
	pbEdit->setEnabled(lesson != NULL);
	
}

void LessonSelect::showEvent(QShowEvent* e)
{
	Q_UNUSED(e);
}

void LessonSelect::on_lwLessons_currentItemChanged(QListWidgetItem* item, QListWidgetItem* prev)
{
	Q_UNUSED(prev);
	
	LwiLesson* lwil = (LwiLesson*) item;
	
	if (lwil != NULL)
		setLesson(lwil->lesson);
	else
		setLesson(NULL);
}

void LessonSelect::on_pbEdit_clicked(bool pressed)
{
	Q_UNUSED(pressed);
	
	if (lesson != NULL)
		mainWindow->actionEdit->trigger();
}

void LessonSelect::on_pbNew_clicked(bool pressed)
{
	Q_UNUSED(pressed);
	QDialog::DialogCode res = 
		(QDialog::DialogCode) mainWindow->dlgLessonMetaEdit->exec();
}

void LessonSelect::on_pbTrain_clicked(bool pressed)
{
	Q_UNUSED(pressed);
	
	if (lesson != NULL)
		mainWindow->actionTrain->trigger();
}
