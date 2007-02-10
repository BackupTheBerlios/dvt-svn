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

#include "flashCardTrainer.h"

#include "mainWindow.h"

#include "utils.h"

FlashCardTrainer::FlashCardTrainer(MainWindow* mainWindow, QWidget* parent)
	:Module(mainWindow, parent)
{
	setupUi(this);
	
	tbNotKnown->setDefaultAction(actionNotKnown);
	tbKnown->setDefaultAction(actionKnown);
	tbTurn->setDefaultAction(actionTurn);
	
	trainer = new Dvt::FlashCardTrainer();
	trainer->setUseHtmlOrig(true);
	trainer->setUseDecl(true);
	trainer->setUseConj(true);
}

FlashCardTrainer::~FlashCardTrainer()
{
	delete trainer;
}

void FlashCardTrainer::setLesson(Dvt::Lesson* lesson)
{
	this->lesson = lesson;
	if (lesson != NULL) {
//		if (!lesson->dataRead()) 
//			lesson->readData();
		trainer->setLesson(lesson);
		trainer->generateQuery();
		qDebug("%d query pairs", trainer->query().size());
		
		lbLessonTitle->setText(STR2QSTR(lesson->title()));
		cardIsFront = false;
		
		Dvt::QueryPair qp = trainer->currentQueryPair();
		if (!qp.empty()) {
			setCard(true, STR2QSTR(qp.orig()));
			
		} else {
			setCard(true, trUtf8("You are through!"));
			
		}
		
		lbCardNumber->setText(QString("%1").arg(qp.number()));
		lbToGo->setText(QString("%1").arg(trainer->nbRemaining()));
		lbNotKnown->setText(QString("%1").arg(trainer->nbWrong()));
		lbKnown->setText(QString("%1").arg(trainer->nbCorrect()));
		
	}
	
}

bool FlashCardTrainer::mayClose()
{
	return true;
}

void FlashCardTrainer::setCard(bool front, QString text)
{
	QPalette palette = lbFlashCard->palette();
	if (front) {
		palette.setColor(QPalette::WindowText, Qt::darkRed);
		frmFlashCard->setFrameShadow(QFrame::Raised);
		lbFlag->setPixmap(mainWindow->flags[STR2QSTR(lesson->langProfile_o()->langCode())]->pixmap(20, 12));
		
	} else {
		palette.setColor(QPalette::WindowText, Qt::darkGreen);
		frmFlashCard->setFrameShadow(QFrame::Sunken);
		lbFlag->setPixmap(mainWindow->flags[STR2QSTR(lesson->langProfile_t()->langCode())]->pixmap(20, 12));
		
	}
	
	lbFlashCard->setPalette(palette);
	
	lbFlashCard->setText(text);
	
	cardIsFront = front;
}

void FlashCardTrainer::setNext()
{
	actionTurn->setChecked(false);
	
	Dvt::QueryPair qp = trainer->nextQueryPair();
	if (!qp.empty()) {
		setCard(true, STR2QSTR(qp.orig()));
		
	} else {
		setCard(true, trUtf8("You are through!"));
		
	}
	
	lbCardNumber->setText(QString("%1").arg(qp.number()));
	lbToGo->setText(QString("%1").arg(trainer->nbRemaining()));
	lbNotKnown->setText(QString("%1").arg(trainer->nbWrong()));
	lbKnown->setText(QString("%1").arg(trainer->nbCorrect()));
	
}

void FlashCardTrainer::on_actionTurn_triggered(bool checked)
{
	cardIsFront = !checked;
	
	Dvt::QueryPair qp = trainer->currentQueryPair();
	if (!qp.empty()) {
		if (cardIsFront)
			setCard(true, STR2QSTR(qp.orig()));
		else
			setCard(false, STR2QSTR(qp.trans()));
		
	}
}

void FlashCardTrainer::on_actionKnown_triggered(bool checked)
{
	Q_UNUSED(checked);
	
	trainer->setCurrentAsCorrect();
	actionTurn->setChecked(false);
	setNext();
}

void FlashCardTrainer::on_actionNotKnown_triggered(bool checked)
{
	Q_UNUSED(checked);
	
	trainer->setCurrentAsWrong();
	actionTurn->setChecked(false);
	setNext();
}
