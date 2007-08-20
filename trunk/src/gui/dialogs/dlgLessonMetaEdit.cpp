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


#include "dlgLessonMetaEdit.h"

#include "mainWindow.h"
#include "utils.h"

#include "dvtLanguageProfile.h"

#include "assert.h"

#include <QLocale>
#include <QMessageBox>
#include <QInputDialog>

using namespace std;

DlgLessonMetaEdit::DlgLessonMetaEdit(MainWindow* parent)
	:QDialog(parent)
{
	setupUi(this);
	
	core = Dvt::Core::getInstance();
	mainWindow = parent;
	lessonFile = NULL;
	
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(validateAccept()));
		
	// populate combo boxes
	
	cboxTitleLang->addItem(trUtf8("International"), QString("ia"));
	cboxDescLang->addItem(trUtf8("International"), QString("ia"));
	cboxLicenseLang->addItem(trUtf8("International"), QString("ia"));
	
	map<string, Dvt::LanguageProfile*> lps = core->languageProfiles();
	map<string, Dvt::LanguageProfile*>::const_iterator lpsit;
	int defaultIndex = -1;
	for (lpsit = lps.begin(); lpsit != lps.end(); lpsit++) {
		Dvt::LanguageProfile* lp = lpsit->second;
		QIcon* icon = mainWindow->flags.value(STR2QSTR(lp->langCode()));
		if (icon != NULL) {
			cboxBaseLang->addItem(*icon, STR2QSTR(lp->name()), (uint) lp);
			cboxForeignLang->addItem(*icon, STR2QSTR(lp->name()), (uint) lp);
			
		} else {
			cboxBaseLang->addItem(STR2QSTR(lp->name()), (uint) lp);
			cboxForeignLang->addItem(STR2QSTR(lp->name()), (uint) lp);
			
		}
		
		cboxTitleLang->addItem(STR2QSTR(lp->name()), STR2QSTR(lp->langCode()));
		cboxDescLang->addItem(STR2QSTR(lp->name()), STR2QSTR(lp->langCode()));
		cboxLicenseLang->addItem(STR2QSTR(lp->name()), STR2QSTR(lp->langCode()));
		
		if (mainWindow->sysLocale.name().startsWith(STR2QSTR(lp->langCode())))
			defaultIndex = cboxBaseLang->count()-1;
		
	}
	
	if (defaultIndex > -1) {
		cboxBaseLang->setCurrentIndex(defaultIndex);
		cboxForeignLang->setCurrentIndex(defaultIndex);
		
		cboxTitleLangIndex = defaultIndex + 1;
		cboxDescLangIndex = defaultIndex + 1;
		cboxLicenseLangIndex = defaultIndex + 1;
		cboxTitleLang->setCurrentIndex(cboxTitleLangIndex);
		cboxDescLang->setCurrentIndex(cboxDescLangIndex);
		cboxLicenseLang->setCurrentIndex(cboxLicenseLangIndex);
		
	}
	
}

DlgLessonMetaEdit::~DlgLessonMetaEdit()
{
}

void DlgLessonMetaEdit::refreshLessonList()
{
	lwLessons->clear();
	
	if (lessonFile != NULL) {
		if (!lessonFile->dataRead()) 
			lessonFile->readData();
	
		int i = 1;
		map<int, Dvt::Lesson*>::const_iterator lit;
		for (lit = lessonFile->lessons().begin(); lit != lessonFile->lessons().end(); lit++, i++) {
			QListWidgetItem* lwi = new QListWidgetItem(QString("%1. %2")
				.arg(i).arg(STR2QSTR(lit->second->title())), lwLessons);
			lwi->setData(Qt::UserRole, (int) lit->second);
			
		}
		
	}
	
}

void DlgLessonMetaEdit::setFromLessonFile(Dvt::LessonFile* lessonFile)
{
	this->lessonFile = lessonFile;
	
	if (lessonFile != NULL) {
		title = lessonFile->title();
		desc = lessonFile->description();
		license = lessonFile->license();
		
		int index = cboxBaseLang->findData((uint) lessonFile->langProfile_o());
		assert(index >= 0);
		cboxBaseLang->setCurrentIndex(index);
		
		index = cboxBaseLang->findData((uint) lessonFile->langProfile_t());
		assert(index >= 0);
		cboxForeignLang->setCurrentIndex(index);
		
		leTitle->setText(STR2QSTR(
			title.getString(QSTR2STR(cboxTitleLang->itemData(cboxTitleLang->currentIndex()).toString()))));
		teDesc->setPlainText(STR2QSTR(
			desc.getString(QSTR2STR(cboxDescLang->itemData(cboxDescLang->currentIndex()).toString()))));
		teLicense->setPlainText(STR2QSTR(
			license.getString(QSTR2STR(cboxLicenseLang->itemData(cboxLicenseLang->currentIndex()).toString()))));
			
		refreshLessonList();
			
	} else {
		title.clear();
		desc.clear();
		license.clear();
		
		leTitle->setText("");
		teDesc->setPlainText("");
		teLicense->setPlainText("");
		
		lwLessons->clear();
		
	}
	
}

void DlgLessonMetaEdit::setToLessonFile(Dvt::LessonFile* lessonFile)
{
	lessonFile->title().setFrom(title);
	lessonFile->description().setFrom(desc);
	lessonFile->license().setFrom(license);
	lessonFile->setLangProfile_o(
		(Dvt::LanguageProfile*) cboxBaseLang->itemData(cboxBaseLang->currentIndex()).toUInt());
	lessonFile->setLangProfile_t(
		(Dvt::LanguageProfile*) cboxForeignLang->itemData(cboxForeignLang->currentIndex()).toUInt());
}

void DlgLessonMetaEdit::on_cboxTitleLang_currentIndexChanged(int index)
{
	if (cboxTitleLangIndex != index) {
		title.setString(QSTR2STR(cboxTitleLang->itemData(cboxTitleLangIndex).toString()), 
			QSTR2STR(leTitle->text()));
		leTitle->setText(STR2QSTR(
			title.getString(QSTR2STR(cboxTitleLang->itemData(index).toString()))));
		
		cboxTitleLangIndex = index;
		
	}
}

void DlgLessonMetaEdit::on_cboxDescLang_currentIndexChanged(int index)
{
	if (cboxDescLangIndex != index) {
		desc.setString(QSTR2STR(cboxDescLang->itemData(cboxDescLangIndex).toString()), 
			QSTR2STR(teDesc->toPlainText()));
		teDesc->setPlainText(STR2QSTR(
			desc.getString(QSTR2STR(cboxDescLang->itemData(index).toString()))));
			
		cboxDescLangIndex = index;
		
	}
}

void DlgLessonMetaEdit::on_cboxLicenseLang_currentIndexChanged(int index)
{
	if (cboxLicenseLangIndex != index) {
		license.setString(QSTR2STR(cboxLicenseLang->itemData(cboxLicenseLangIndex).toString()), 
			QSTR2STR(teLicense->toPlainText()));
		teLicense->setPlainText(STR2QSTR(
			license.getString(QSTR2STR(cboxLicenseLang->itemData(index).toString()))));
			
		cboxLicenseLangIndex = index;
		
	}
}

void DlgLessonMetaEdit::on_pbNewLesson_clicked(bool checked)
{
	Q_UNUSED(checked);
	
	bool ok = true;
	QString name = "";
	
	while (name == "" && ok) {
		name = QInputDialog::getText(this, APPNAME, trUtf8("Enter name of lesson:"),
			QLineEdit::Normal, QString(), &ok);
		name = name.trimmed();
		
		if (ok) {
			if (name == "") {
				QMessageBox::warning(this, APPNAME, trUtf8("You must enter a name for the lesson."));
				
			} else {
				// determin id of last lesson
				int id = 1;
				map<int, Dvt::Lesson*>::reverse_iterator it = lessonFile->lessons().rbegin();
				if (it != lessonFile->lessons().rend()) {
					Dvt::Lesson* last = it->second;
					id = last->number() + 1;
				}
				
				// TODO: let user choose which language to use
				lessonFile->createLesson(id, Dvt::MlString("ia", QSTR2STR(name))); 
				refreshLessonList();
				
				break;
				
			}
			
		}
		
	}
		
	
}

void DlgLessonMetaEdit::on_pbRenameLesson_clicked(bool checked)
{
	Q_UNUSED(checked);
	
	if (lwLessons->currentItem() != NULL) {
		Dvt::Lesson* lesson = (Dvt::Lesson*) lwLessons->currentItem()->data(Qt::UserRole).toInt();
		assert(lesson != NULL);
		
		bool ok = true;
		QString name = "";
		
		while (name == "" && ok) {
			QString name = STR2QSTR(lesson->title());
			name = QInputDialog::getText(this, APPNAME, trUtf8("Enter name of lesson:"),
				QLineEdit::Normal, name, &ok);
			name = name.trimmed();
			
			if (ok) {
				if (name == "") {
					QMessageBox::warning(this, APPNAME, trUtf8("You must enter a name for the lesson."));
					
				} else {
					// TODO: let user choose which language to use
					lesson->setTitle(Dvt::MlString("ia", QSTR2STR(name)));
					refreshLessonList();
					
					break;
					
				}
				
			}
			
		}
		
	}
}

void DlgLessonMetaEdit::on_pbDeleteLesson_clicked(bool checked)
{
	Q_UNUSED(checked);
	
	if (lwLessons->currentItem() != NULL) {
		Dvt::Lesson* lesson = (Dvt::Lesson*) lwLessons->currentItem()->data(Qt::UserRole).toInt();
		assert(lesson != NULL);
		int res = QMessageBox::question(this, APPNAME,
			QString(trUtf8("Are you sure that you want to delete the lesson \"%1\"?\nAll entries will get lost."))
				.arg(STR2QSTR(lesson->title())),
			QMessageBox::Yes, QMessageBox::No);
		
		if (res == QMessageBox::Yes) {
			lessonFile->deleteLesson(lesson);
			refreshLessonList();
			
		}
		
	}
}

void DlgLessonMetaEdit::on_lwLessons_currentItemChanged(QListWidgetItem* item, QListWidgetItem* prev)
{
	Q_UNUSED(prev);
	
	pbRenameLesson->setEnabled(item != NULL);
	pbDeleteLesson->setEnabled(item != NULL);
}

void DlgLessonMetaEdit::validateAccept()
{
	title.setString(QSTR2STR(cboxTitleLang->itemData(cboxTitleLangIndex).toString()), 
		QSTR2STR(leTitle->text()));
	desc.setString(QSTR2STR(cboxDescLang->itemData(cboxDescLangIndex).toString()), 
		QSTR2STR(teDesc->toPlainText()));
	license.setString(QSTR2STR(cboxLicenseLang->itemData(cboxLicenseLangIndex).toString()), 
		QSTR2STR(teLicense->toPlainText()));
	
	if (title.empty()) {
		QMessageBox::information(this, APPNAME, trUtf8("Please enter a title for this lesson file."));
		
	} else if (lessonFile->lessons().size() == 0) {
		QMessageBox::information(this, APPNAME, trUtf8("Please create at least one lesson."));
		
	}else {
		accept();
		
	}
}

void DlgLessonMetaEdit::accept()
{
	if (lessonFile != NULL)
		setToLessonFile(lessonFile);
	
	QDialog::accept();
}
