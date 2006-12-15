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

#include "lessonEdit.h"

#include "mainWindow.h"
#include "utils.h"
#include "lessonSelect.h"
#include "language.h"
#include "editExt.h"
#include "editDetail.h"

#include "dvtExceptions.h"
#include "dvtTrainingEntry.h"
#include "dvtLanguageProfile.h"

#include <vector>

#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>

using namespace std;

LessonEdit::LessonEdit(MainWindow* mainWindow, QWidget* parent)
	:Module(mainWindow, parent)
{
	currentEntry = NULL;
	lesson = NULL;
	edited = false;
	foreignLang = NULL;
	nativeLang = NULL;
	
	setupUi(this);
	
	tabDetails->hide();
	
	frmForeignExt->setLayout(new QHBoxLayout());
	frmForeignExt->layout()->setMargin(6);
	frmNativeExt->setLayout(new QHBoxLayout());
	frmNativeExt->layout()->setMargin(6);
	
	QFontMetrics fontMetrics(twEntries->verticalHeader()->font());
	int fontHeight = fontMetrics.height();
	twEntries->setColumnCount(2);
	twEntries->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
	twEntries->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
	twEntries->horizontalHeader()->setIconSize(QSize(23, 14));
	twEntries->verticalHeader()->setDefaultSectionSize(fontHeight + 2);
	twEntries->verticalHeader()->setResizeMode(1, QHeaderView::Fixed);
//	twEntries->verticalHeader()->hide();
	
	frmForeignGender->setVisible(false);
	frmNativeGender->setVisible(false);
	
	cboxWordClass->addItem(STR2QSTR(Dvt::WordClass::getString(Dvt::WordClass::Undefined)), Dvt::WordClass::Undefined);
	cboxWordClass->addItem(STR2QSTR(Dvt::WordClass::getString(Dvt::WordClass::Noun)), Dvt::WordClass::Noun);
	cboxWordClass->addItem(STR2QSTR(Dvt::WordClass::getString(Dvt::WordClass::Verb)), Dvt::WordClass::Verb);
	cboxWordClass->addItem(STR2QSTR(Dvt::WordClass::getString(Dvt::WordClass::Adjective)), Dvt::WordClass::Adjective);
	cboxWordClass->addItem(STR2QSTR(Dvt::WordClass::getString(Dvt::WordClass::Adverb)), Dvt::WordClass::Adverb);
	cboxWordClass->addItem(STR2QSTR(Dvt::WordClass::getString(Dvt::WordClass::Pronoun)), Dvt::WordClass::Pronoun);
	cboxWordClass->addItem(STR2QSTR(Dvt::WordClass::getString(Dvt::WordClass::Preposition)), Dvt::WordClass::Preposition);
	cboxWordClass->addItem(STR2QSTR(Dvt::WordClass::getString(Dvt::WordClass::Conjunction)), Dvt::WordClass::Conjunction);
	cboxWordClass->addItem(STR2QSTR(Dvt::WordClass::getString(Dvt::WordClass::Interjection)), Dvt::WordClass::Interjection);
	cboxWordClass->addItem(STR2QSTR(Dvt::WordClass::getString(Dvt::WordClass::Numeral)), Dvt::WordClass::Numeral);
	cboxWordClass->addItem(STR2QSTR(Dvt::WordClass::getString(Dvt::WordClass::Phrase)), Dvt::WordClass::Phrase);
	cboxWordClass->addItem(STR2QSTR(Dvt::WordClass::getString(Dvt::WordClass::NonNounNonVerb)), Dvt::WordClass::NonNounNonVerb);
	
	saDetails = new QScrollArea(tabDetails);
	saDetails->setWidgetResizable(true);
	saDetails->setFrameStyle(QFrame::NoFrame);
	saDetails->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	
	tabDetails->setLayout(new QVBoxLayout());
	tabDetails->layout()->setMargin(0);
	tabDetails->layout()->addWidget(saDetails);
	tabWidget->setCurrentIndex(0);
	tabWidget->setTabEnabled(1, false);
	
	setTwiEntry(NULL);
	
	resetTabOrder();
	
}

LessonEdit::~LessonEdit()
{
}

void LessonEdit::setLesson(Dvt::Lesson* lesson)
{
	if (lesson != NULL) {
		if (this->lesson != lesson) {
			this->lesson = lesson;
			
			if (!lesson->dataRead()) 
				lesson->readData();
			
			tabWidget->setCurrentIndex(0);
			tabWidget->setTabEnabled(1, false);
			
			if (foreignLang != NULL) delete foreignLang;
			if (nativeLang != NULL) delete nativeLang;
			foreignLang = new Language(this, lesson->langProfile_t());
			nativeLang = new Language(this, lesson->langProfile_o());
			
			// titles
			
			gboxForeignLang->setTitle(STR2QSTR(lesson->langProfile_t()->name()));
			gboxNativeLang->setTitle(STR2QSTR(lesson->langProfile_o()->name()));
			
			// genders
			
			vector<Dvt::Gender::Type>::iterator git;
			
			cboxForeignGender->clear();
			for (git = lesson->langProfile_t()->genders().begin();
				git != lesson->langProfile_t()->genders().end();
				git++)
			{
				cboxForeignGender->addItem(STR2QSTR(Dvt::Gender::getString(*git)), *git);
			}
			frmForeignGender->setVisible(false);
			
			cboxNativeGender->clear();
			for (git = lesson->langProfile_o()->genders().begin();
				git != lesson->langProfile_o()->genders().end();
				git++)
			{
				cboxNativeGender->addItem(STR2QSTR(Dvt::Gender::getString(*git)), *git);
			}
			frmNativeGender->setVisible(false);
			
			// edit extensions
			
			if (foreignLang->editExt() != NULL) {
				frmForeignExt->layout()->addWidget(foreignLang->editExt());
				frmForeignExt->show();
				
			} else {
				frmForeignExt->hide();
				
			}
				
			if (nativeLang->editExt() != NULL) {
				frmNativeExt->layout()->addWidget(nativeLang->editExt());
				frmNativeExt->show();
				
			} else {
				frmNativeExt->hide();
				
			}
			
			resetTabOrder();
			
			// table entries
			
			twEntries->clear();
			twEntries->setRowCount(lesson->entries().size());
			qDebug("%d lesson entries", lesson->entries().size());
			
			bool useLabels = lesson->entries().size() < 201;
			int i = 0;
			vector<Dvt::TrainingEntry*>::iterator it;
			for (it = lesson->entries().begin(); it != lesson->entries().end(); it++, i++) {
				Dvt::TrainingEntry* te = *it;
				new TwiEntry(te, i, twEntries, useLabels);
			}
			
			// table headers
			
			QTableWidgetItem* h;
			QIcon* icon;
			
			h = new QTableWidgetItem(gboxForeignLang->title());
			icon = mainWindow->flags[lesson->langProfile_t()->langCode().c_str()];
			if (icon != NULL) h->setIcon(*icon);
			twEntries->setHorizontalHeaderItem(0, h);
			
			h = new QTableWidgetItem(gboxNativeLang->title());
			icon = mainWindow->flags[lesson->langProfile_o()->langCode().c_str()];
			if (icon != NULL) h->setIcon(*icon);
			twEntries->setHorizontalHeaderItem(1, h);
			
			edited = false;
		}
		
	} else {
		edited = false;
		this->lesson = lesson;
		setTwiEntry(NULL);
		twEntries->clear();
		
	}
	
}

bool LessonEdit::mayClose()
{
	if (lesson == NULL) return true;
	
	if (currentEntry != NULL)
		postCurrentEntry();
		
	if (edited) {
		QMessageBox::StandardButton b = 
			QMessageBox::question(this, "DVT", trUtf8("Save changes?"), 
				QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, 
				QMessageBox::Yes);
				
		switch (b) {
			case QMessageBox::Yes: {
				actionSave->trigger();
				return true;
				break;
			}
			case QMessageBox::No: {
				lesson->unloadData();
				return true;
				break;
			}
			case QMessageBox::Cancel: {
				return false;
				break;
			}
			default: {
				assert(false);
			}
		}
		
	}
			
	return true;		
}

void LessonEdit::setDetail(Dvt::WordClass::Type wc)
{
	QWidget* w = saDetails->widget();
		
	if (currentEntry != NULL && 
		wc != Dvt::WordClass::Undefined) 
	{
		EditDetail* ed = foreignLang->getEditDetail(wc);
		if (ed != w) {
			saDetails->takeWidget();
			saDetails->setWidget(ed);
			if (ed != NULL) {
				ed->show();
				tabWidget->setTabEnabled(1, true);
			
			} else {
				tabWidget->setCurrentIndex(0);
				tabWidget->setTabEnabled(1, false);
				
			}
		}
		
	} else {
		saDetails->takeWidget();
		tabWidget->setCurrentIndex(0);
		tabWidget->setTabEnabled(1, false);
		
	}
}

void LessonEdit::setEdited(bool edited)
{
	this->edited = this->edited || edited;
}

void LessonEdit::resetTabOrder()
{
	setTabOrder(cboxWordClass, leForeignLang);
	setTabOrder(leForeignLang, cboxForeignGender);
	
	if (foreignLang != NULL && foreignLang->editExt() != NULL) {
		setTabOrder(cboxForeignGender, foreignLang->editExt());
		foreignLang->editExt()->setNextTabWidget(leNativeLang);
		
	} else {
		setTabOrder(cboxForeignGender, leNativeLang);
		
	}
	
	setTabOrder(leNativeLang, cboxNativeGender);
	
	if (nativeLang != NULL && nativeLang->editExt() != NULL) {
		setTabOrder(cboxForeignGender, nativeLang->editExt());
		nativeLang->editExt()->setNextTabWidget(pbNew);
		
	} else {
		setTabOrder(cboxNativeGender, pbNew);
		
	}
	
	setTabOrder(pbNew, pbSave);
	setTabOrder(pbSave, pbDelete);
}

void LessonEdit::setEntry(Dvt::TrainingEntry* te)
{
	if (te != NULL) {
		
		// word class
		
		Dvt::WordClass::Type wc = te->wordClass();
		int index = cboxWordClass->findData(wc);
		if (index >= 0)
			cboxWordClass->setCurrentIndex(index);
		else
			cboxWordClass->setCurrentIndex(0);
			
		// base names
		
		leForeignLang->setText(STR2QSTR(te->trans()->baseName()));
		leNativeLang->setText(STR2QSTR(te->orig()->baseName()));
		
		// genders
		
		if (wc == Dvt::WordClass::Noun) {
			if (lesson->langProfile_t()->genders().size() > 1)
				frmForeignGender->setVisible(true);
			if (lesson->langProfile_o()->genders().size() > 1)
				frmNativeGender->setVisible(true);
			
		} else {
			frmForeignGender->setVisible(false);
			frmNativeGender->setVisible(false);
			
		}
		
		index = cboxForeignGender->findData(te->trans()->gender());
		if (index >= 0)
			cboxForeignGender->setCurrentIndex(index);
		else
			cboxForeignGender->setCurrentIndex(0);
		
		index = cboxNativeGender->findData(te->orig()->gender());
		if (index >= 0)
			cboxNativeGender->setCurrentIndex(index);
		else
			cboxNativeGender->setCurrentIndex(0);
		
		// detail widget
		
		setDetail(te->trans()->wordClass());
		
	} else {
		cboxWordClass->setCurrentIndex(0);
		cboxForeignGender->setCurrentIndex(0);
		cboxNativeGender->setCurrentIndex(0);
		leForeignLang->setText("");
		leNativeLang->setText("");
		setDetail(Dvt::WordClass::Undefined);
		
	}
	
	pbDelete->setEnabled(te != NULL);
	cboxWordClass->setEnabled(te != NULL);
	gboxForeignLang->setEnabled(te != NULL);
	gboxNativeLang->setEnabled(te != NULL);
	
}

void LessonEdit::setTwiEntry(TwiEntry* te)
{
	currentEntry = te;
	if (te != NULL) {
		twEntries->setCurrentItem(te->trans);
		te->setSelected(true);
		
	} else {
		twEntries->setCurrentItem(NULL);
		
	}
	
	emitCurrentEntryChanged();
}

void LessonEdit::postCurrentEntry(bool recursive)
{
	if (currentEntry == NULL) {
		qDebug("ERROR: LessonEdit::postCurrentEntry(): currentEntry == NULL");
		return;
		
	}
	
	Dvt::WordClass::Type wc = 
		(Dvt::WordClass::Type) cboxWordClass->itemData(cboxWordClass->currentIndex()).toInt();
	
	Dvt::Gender::Type gen_t = Dvt::Gender::Undefined;
	Dvt::Gender::Type gen_o = Dvt::Gender::Undefined;
	if (wc == Dvt::WordClass::Noun) {
		gen_t = (Dvt::Gender::Type) cboxForeignGender->itemData(cboxForeignGender->currentIndex()).toInt();
		gen_o = (Dvt::Gender::Type) cboxNativeGender->itemData(cboxNativeGender->currentIndex()).toInt();
	}
		
	string o = leNativeLang->text().trimmed().toUtf8().data();
	string t = leForeignLang->text().trimmed().toUtf8().data();
	
	Dvt::TrainingEntry* te = currentEntry->trainingEntry;
	
	edited = edited || te->wordClass() != wc;
	edited = edited || te->orig()->baseName() != o;
	edited = edited || te->trans()->baseName() != t;
	edited = edited || te->orig()->gender() != gen_o;
	edited = edited || te->trans()->gender() != gen_t;
	
	currentEntry->trainingEntry->orig()->setBaseName(o);
	currentEntry->trainingEntry->trans()->setBaseName(t);
	currentEntry->trainingEntry->setWordClass(wc);
	
	if (wc == Dvt::WordClass::Noun) {
		currentEntry->trainingEntry->orig()->setGender(gen_o);
		currentEntry->trainingEntry->trans()->setGender(gen_t);
	}
	
	if (recursive)
		emitCurrentEntryPosted();
	
	currentEntry->setSelected(false);
	currentEntry->refresh();
	
	emitCurrentEntryChanged();
}

void LessonEdit::showEvent(QShowEvent* e)
{
	Q_UNUSED(e);
}

void LessonEdit::emitCurrentEntryChanged()
{
	if (currentEntry != NULL) {
		setEntry(currentEntry->trainingEntry);
		
	} else {
		setEntry(NULL);
		
	}
	
	emit currentEntryChanged(currentEntry);
}

void LessonEdit::emitCurrentEntryPosted()
{
	if (currentEntry != NULL)
		emit currentEntryPosted(currentEntry);
		
}

void LessonEdit::on_twEntries_currentItemChanged(QTableWidgetItem* current, 
	QTableWidgetItem* previous)
{
	Q_UNUSED(previous);
	
	if (currentEntry != NULL) {
		int row = twEntries->row(currentEntry->trans);
		if (current == NULL || row != twEntries->row(current))
			postCurrentEntry();
	
	}
	
	if (current != NULL) 
		setTwiEntry(((TwiCellEntry*) current)->entry);
	else
		setTwiEntry(NULL);
}

void LessonEdit::on_cboxWordClass_currentIndexChanged(int index)
{
	Q_UNUSED(index);
	if (lesson != NULL && currentEntry != NULL) {
		Dvt::WordClass::Type wc = 
			(Dvt::WordClass::Type) cboxWordClass->itemData(cboxWordClass->currentIndex()).toInt();
		if (wc != currentEntry->trainingEntry->wordClass())
			postCurrentEntry(false);
			
	}
	
}

void LessonEdit::on_actionNew_triggered(bool checked)
{
	Q_UNUSED(checked);
	
	if (currentEntry != NULL) postCurrentEntry();
	
	Dvt::TrainingEntry* te = lesson->addEntry();
	twEntries->setRowCount(lesson->entries().size());
	int row = lesson->entries().size()-1;
	
	setTwiEntry(new TwiEntry(te, row, twEntries));
	
	cboxWordClass->setFocus();
}

void LessonEdit::on_actionDelete_triggered(bool checked)
{
	Q_UNUSED(checked);
	
	assert(currentEntry != NULL);
	
	int row = twEntries->row(currentEntry->trans);
	Dvt::TrainingEntry* te = currentEntry->trainingEntry;
	
	currentEntry = NULL;
	lesson->deleteEntry(te);
	twEntries->removeRow(row);
	
	edited = edited || true;
	
	if (row < twEntries->rowCount()) {
		twEntries->setCurrentCell(row, 0);
		
	} else if (row-1 >= 0) {
		twEntries->setCurrentCell(row-1, 0);
		
	}
}

void LessonEdit::on_actionSave_triggered(bool checked)
{
	Q_UNUSED(checked);
	
	postCurrentEntry();
	
	try {
		lesson->write();
		edited = false;
		
	} catch (Dvt::Exception e) {
		QMessageBox::critical(this, "DVT",
			QString("Dvt::Exception %1: %2")
				.arg(e.code)
				.arg(e.msg.c_str()),
			QMessageBox::Cancel);
		
	}
}

void LessonEdit::on_pbNew_clicked(bool checked)
{
	Q_UNUSED(checked);
	actionNew->trigger();
}

void LessonEdit::on_pbDelete_clicked(bool checked)
{
	Q_UNUSED(checked);
	actionDelete->trigger();
}

void LessonEdit::on_pbSave_clicked(bool checked)
{
	Q_UNUSED(checked);
	actionSave->trigger();
}

void LessonEdit::on_tabWidget_currentChanged(int index)
{
	switch (index) {
		case 0: {
			break;
		}
		case 1: {
			if (currentEntry != NULL)
				setDetail(currentEntry->trainingEntry->trans()->wordClass());
			break;
		}
		default: {}
	}
}
