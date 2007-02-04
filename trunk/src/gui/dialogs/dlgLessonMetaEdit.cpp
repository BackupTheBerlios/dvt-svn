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

#include <QLocale>
#include <QMessageBox>

using namespace std;

DlgLessonMetaEdit::DlgLessonMetaEdit(MainWindow* parent)
	:QDialog(parent)
{
	setupUi(this);
	
	core = Dvt::Core::getInstance();
	mainWindow = parent;
	lesson = NULL;
	
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

void DlgLessonMetaEdit::setFromLesson(Dvt::Lesson* lesson)
{
	this->lesson = lesson;
	
	if (lesson != NULL) {
		title = lesson->title();
		desc = lesson->description();
		license = lesson->license();
		
		int index = cboxBaseLang->findData((uint) lesson->langProfile_o());
		assert(index >= 0);
		cboxBaseLang->setCurrentIndex(index);
		
		index = cboxBaseLang->findData((uint) lesson->langProfile_t());
		assert(index >= 0);
		cboxForeignLang->setCurrentIndex(index);
		
		leTitle->setText(STR2QSTR(
			title.getString(QSTR2STR(cboxTitleLang->itemData(cboxTitleLang->currentIndex()).toString()))));
		teDesc->setPlainText(STR2QSTR(
			desc.getString(QSTR2STR(cboxDescLang->itemData(cboxDescLang->currentIndex()).toString()))));
		teLicense->setPlainText(STR2QSTR(
			license.getString(QSTR2STR(cboxLicenseLang->itemData(cboxLicenseLang->currentIndex()).toString()))));
			
	} else {
		title.clear();
		desc.clear();
		license.clear();
		
		leTitle->setText("");
		teDesc->setPlainText("");
		teLicense->setPlainText("");
		
	}
	
}

void DlgLessonMetaEdit::setToLesson(Dvt::Lesson* lesson)
{
	lesson->title().setFrom(title);
	lesson->description().setFrom(desc);
	lesson->license().setFrom(license);
	lesson->setLangProfile_o(
		(Dvt::LanguageProfile*) cboxBaseLang->itemData(cboxBaseLang->currentIndex()).toUInt());
	lesson->setLangProfile_t(
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

void DlgLessonMetaEdit::validateAccept()
{
	title.setString(QSTR2STR(cboxTitleLang->itemData(cboxTitleLangIndex).toString()), 
		QSTR2STR(leTitle->text()));
	desc.setString(QSTR2STR(cboxDescLang->itemData(cboxDescLangIndex).toString()), 
		QSTR2STR(teDesc->toPlainText()));
	license.setString(QSTR2STR(cboxLicenseLang->itemData(cboxLicenseLangIndex).toString()), 
		QSTR2STR(teLicense->toPlainText()));
	
	if (title.empty()) {
		QMessageBox::information(this, APPNAME, trUtf8("Please enter a title for this lesson."));
		
	} else {
		accept();
		
	}
}

void DlgLessonMetaEdit::accept()
{
	if (lesson != NULL)
		setToLesson(lesson);
	
	QDialog::accept();
}
