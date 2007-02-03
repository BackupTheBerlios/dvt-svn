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
		if (!lesson->dataRead()) 
			lesson->readData();
		trainer->setLesson(lesson);
		trainer->generateQuery();
		qDebug("%d query pairs", trainer->query().size());
		
		cardIsFront = false;
		actionTurn->trigger();
		
	}
	
}

bool FlashCardTrainer::mayClose()
{
	return true;
}

void FlashCardTrainer::setCard(bool front, QString text)
{
	QPalette palette = lbFlashCard->palette();
	if (front)
		palette.setColor(QPalette::WindowText, Qt::darkRed);
	else
		palette.setColor(QPalette::WindowText, Qt::darkGreen);
	lbFlashCard->setPalette(palette);
	
	lbFlashCard->setText(text);
	
	cardIsFront = front;
}

void FlashCardTrainer::setNext()
{
	Dvt::QueryPair qp = trainer->nextQueryPair();
	if (!qp.empty()) {
		setCard(true, STR2QSTR(qp.orig()));
		
	} else {
		setCard(true, trUtf8("You are through!"));
		
	}
	
}

void FlashCardTrainer::on_actionTurn_triggered(bool checked)
{
	Q_UNUSED(checked);
	
	Dvt::QueryPair qp = trainer->currentQueryPair();
	if (!qp.empty()) {
		cardIsFront = !cardIsFront;
		if (cardIsFront)
			setCard(true, STR2QSTR(qp.orig()));
		else
			setCard(false, STR2QSTR(qp.trans()));
		
	}
}

void FlashCardTrainer::on_actionKnown_triggered(bool checked)
{
	Q_UNUSED(checked);
	
	setNext();
}

void FlashCardTrainer::on_actionNotKnown_triggered(bool checked)
{
	Q_UNUSED(checked);
	
	setNext();
}
