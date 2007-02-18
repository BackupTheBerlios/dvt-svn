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
#include "dvtExceptions.h"

#include <QMessageBox>
#include <QInputDialog>
#include <QFileInfo>

using namespace std;

LessonSelect::LessonSelect(MainWindow* mainWindow, QWidget* parent)
	:Module(mainWindow, parent)
{
	setupUi(this);
	
	lbFlagOrig->setText("");
	lbFlagTrans->setText("");
	
	setLessonFile(NULL);
}

LessonSelect::~LessonSelect()
{
}

void LessonSelect::loadLessonFiles()
{
	lwLessons->clear();
	
	vector<Dvt::LessonFile*> lessonFiles = core->lessonFiles();
	vector<Dvt::LessonFile*>::iterator it;
	for (it = lessonFiles.begin(); it != lessonFiles.end(); it++) {
		LwiLessonFile* lwil = new LwiLessonFile(*it, lwLessons);
		if (*it == lessonFile)
			lwLessons->setCurrentItem(lwil);
	}
}

void LessonSelect::setLessonFile(Dvt::LessonFile* lessonFile)
{
	this->lessonFile = lessonFile;
	
	if (lessonFile != NULL) {
		QIcon* icon;
		
		lbLangOrig->setText(STR2QSTR(lessonFile->langProfile_o()->name()));
		if ((icon = mainWindow->flags[lessonFile->langProfile_o()->langCode().c_str()]))
			lbFlagOrig->setPixmap(icon->pixmap(icon->actualSize(QSize(23,14))));
		else
			lbFlagOrig->setPixmap(QPixmap());
			
		lbLangTrans->setText(STR2QSTR(lessonFile->langProfile_t()->name()));
		if ((icon = mainWindow->flags[lessonFile->langProfile_t()->langCode().c_str()]))
			lbFlagTrans->setPixmap(icon->pixmap(icon->actualSize(QSize(23,14))));
		else
			lbFlagTrans->setPixmap(QPixmap());
		
		teExtendedInfo->clear();
		QString desc = STR2QSTR(lessonFile->description());
		QString license = STR2QSTR(lessonFile->license());
		if (!desc.isEmpty()) {
			teExtendedInfo->append("<b>" + trUtf8("Description:") + "</b>");
			teExtendedInfo->append(desc);
			teExtendedInfo->append("");
			
		}
		if (!license.isEmpty()) {
			teExtendedInfo->append("<b>" + trUtf8("License:") + "</b>");
			teExtendedInfo->append(license);
			
		}
		
	} else {
		lbLangOrig->setText("");
		lbLangTrans->setText("");
		teExtendedInfo->clear();
		
	}
	
	pbEdit->setEnabled(lessonFile != NULL);
	
}

void LessonSelect::showEvent(QShowEvent* e)
{
	Q_UNUSED(e);
}

void LessonSelect::on_lwLessons_currentItemChanged(QListWidgetItem* item, QListWidgetItem* prev)
{
	Q_UNUSED(prev);
	
	LwiLessonFile* lwil = (LwiLessonFile*) item;
	
	if (lwil != NULL)
		setLessonFile(lwil->lessonFile);
	else
		setLessonFile(NULL);
}

void LessonSelect::on_pbEdit_clicked(bool pressed)
{
	Q_UNUSED(pressed);
	
	if (lessonFile != NULL)
		mainWindow->actionEdit->trigger();
}

void LessonSelect::on_pbNew_clicked(bool pressed)
{
	Q_UNUSED(pressed);
	QDialog::DialogCode res = 
		(QDialog::DialogCode) mainWindow->dlgLessonMetaEdit->exec();
	if (res == QDialog::Accepted) {
		// TODO: build better file names
		QString fname = mainWindow->dlgLessonMetaEdit->title.c_str();
		bool ok = false;
		fname = QInputDialog::getText(this, APPNAME, 
			trUtf8("Please enter a file name for this lesson\n(it will be saved to your lessons directory):"),
			QLineEdit::Normal, fname, &ok);
		if (!ok) return;
		
		QString fileName = QString("lessons/%1%2")
			.arg(fname)
			.arg(DVT_TRAINING_LESSON_FILE_SUFFIX);
		QFileInfo fi(fileName);
		
		while (fi.exists()) {
			fname = QInputDialog::getText(this, APPNAME, 
				trUtf8("File already exists, please choose a different one:"),
				QLineEdit::Normal, fname, &ok);
			if (!ok) return;
			
			fileName = QString("lessons/%1%2")
				.arg(fname)
				.arg(DVT_TRAINING_LESSON_FILE_SUFFIX);
			fi.setFile(fileName);
			
		}
		
		Dvt::LessonFile* lessonFile = core->createLessonFile();
		mainWindow->dlgLessonMetaEdit->setToLessonFile(lessonFile);
		
		try {
			lessonFile->writeToFile(QSTR2STR(fileName));
				
		} catch(Dvt::Exception e) {
			QMessageBox::critical(this, APPNAME,
				QString(trUtf8("Cannot write to file %1:\n\n"))
					.arg(fileName) + 
				QString("Dvt::Exception %1: %2")
					.arg(e.code)
					.arg(e.msg.c_str()),
				QMessageBox::Cancel);
			
			return;
			
		}
		
		mainWindow->dlgLessonMetaEdit->setFromLessonFile(NULL);
		loadLessonFiles();
		
	}
}

void LessonSelect::on_pbTrain_clicked(bool pressed)
{
	Q_UNUSED(pressed);
	
	if (lessonFile != NULL)
		mainWindow->actionTrain->trigger();
}
