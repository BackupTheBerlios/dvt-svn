/** @file
 * 
 * LessonMetaEdit - Part of the Qt4-GUI of the portable Dictionary and 
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

#ifndef DLGLESSONMETAEDIT_H_
#define DLGLESSONMETAEDIT_H_

#include "ui_dlgLessonMetaEdit.h"

#include "dvtCore.h"
#include "dvtLessonFile.h"
#include "dvtMlString.h"

#include <QDialog>

class MainWindow;

class DlgLessonMetaEdit : public QDialog, public Ui::DlgLessonMetaEdit
{
	Q_OBJECT
	
private:
	Dvt::Core* core;
	MainWindow* mainWindow;
	
	Dvt::LessonFile* lessonFile;
	
	int cboxTitleLangIndex;
	int cboxDescLangIndex;
	int cboxLicenseLangIndex;
	
public:
	Dvt::MlString title;
	Dvt::MlString desc;
	Dvt::MlString license;

	DlgLessonMetaEdit(MainWindow* parent);
	virtual ~DlgLessonMetaEdit();
	
	void setFromLessonFile(Dvt::LessonFile* lessonFile);
	void setToLessonFile(Dvt::LessonFile* lessonFile);
	
public slots:
	void on_cboxTitleLang_currentIndexChanged(int index);
	void on_cboxDescLang_currentIndexChanged(int index);
	void on_cboxLicenseLang_currentIndexChanged(int index);
	
	void validateAccept();
	virtual void accept();
	
};

#endif /*DLGLESSONMETAEDIT_H_*/
