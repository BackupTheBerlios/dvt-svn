/** @file
 * 
 * TrainerSelect - Part of the Qt4-GUI of the portable Dictionary and 
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

#include "dlgTrainerSelect.h"

#include "mainWindow.h"
#include "utils.h"

#include "dvtLessonFile.h"
#include "dvtLesson.h"

#include "assert.h"

#include <QMessageBox>

#include <map>

using namespace std;

DlgTrainerSelect::DlgTrainerSelect(MainWindow* parent)
	:QDialog(parent)
{
	setupUi(this);
	
	mainWindow = parent;
	useDecls = false;
	useConjs = false;
	
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(validateAccept()));
	
	trainer = (Module*) mainWindow->flashCardTrainer;
	cboxTrainer->addItem(trUtf8("Flash Cards"), mainWindow->flashCardTrainer);
	cboxTrainer->setCurrentIndex(0);
}

DlgTrainerSelect::~DlgTrainerSelect()
{
	
}

void DlgTrainerSelect::setLessonFile(Dvt::LessonFile* lessonFile)
{
	this->lessonFile = lessonFile;
	lwLessons->clear();
	
	if (lessonFile != NULL) {
		if (!lessonFile->dataRead()) 
			lessonFile->readData();
				
		int i = 1;
		map<int, Dvt::Lesson*>::const_iterator lit;
		for (lit = lessonFile->lessons().begin(); lit != lessonFile->lessons().end(); lit++, i++) {
			QListWidgetItem* lwi = new QListWidgetItem(QString("%1. %2")
				.arg(i).arg(STR2QSTR(lit->second->title())), lwLessons);
			lwi->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
			lwi->setCheckState(Qt::Unchecked);
			lwi->setData(Qt::UserRole, (int) lit->second);
			
		}
		
	}
	
}

void DlgTrainerSelect::validateAccept()
{
	if (false) {
		// TODO
		QMessageBox::information(this, APPNAME, trUtf8("Please enter a title for this lesson file."));
		
	} else {
		accept();
		
	}
}

void DlgTrainerSelect::accept()
{
	useConjs = chkBoxConjs->isChecked();
	useDecls = chkBoxConjs->isChecked();
	
	lessons.clear();
	for (int i = 0; i < lwLessons->count(); i++) {
		QListWidgetItem* lwi = lwLessons->item(i);
		if (lwi->checkState())
			lessons.push_back((Dvt::Lesson*) lwi->data(Qt::UserRole).toInt());
		
	}
	
	QDialog::accept();
}
