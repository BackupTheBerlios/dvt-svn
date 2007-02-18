/** @file
 * 
 * SimpleTrainer - Part of the Qt4-GUI of the portable Dictionary and 
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

#include "simpleTrainer.h"

#include "mainWindow.h"
#include "dlgTrainerSelect.h"

#include "utils.h"

#include <QPainter>
#include <QPixmap>

#define PIC_DIR	"."

SimpleTrainer::SimpleTrainer(MainWindow* mainWindow, QWidget* parent)
	:Module(mainWindow, parent)
{
	QFont f;
	
	setupUi(this);
	
	f = lbOrig->font();
	f.setPointSize(14);
	lbOrig->setFont(f);
	lbCorrection->setFont(f);
	
	f = lbOrig->font();
	f.setPointSize(12);
	leTrans->setFont(f);
	
	trainer = new Dvt::SimpleTrainer();
	trainer->setUseHtmlOrig(true);
	trainer->setUseDecl(true);
	trainer->setUseConj(true);
}

SimpleTrainer::~SimpleTrainer()
{
	delete trainer;
}

void SimpleTrainer::setLessonFile(Dvt::LessonFile* lessonFile)
{
	this->lessonFile = lessonFile;
	if (lessonFile != NULL) {
//		if (!lesson->dataRead()) 
//			lesson->readData();
		trainer->setLessonFile(lessonFile);
		trainer->setUseDecl(mainWindow->dlgTrainerSelect->useDecls);
		trainer->setUseConj(mainWindow->dlgTrainerSelect->useConjs);
		trainer->setLessons(mainWindow->dlgTrainerSelect->lessons);
		trainer->generateQuery();
		qDebug("%d query pairs", trainer->query().size());
		setNext();
		
	}
	
}

bool SimpleTrainer::mayClose()
{
	return true;
}

void SimpleTrainer::setNext()
{
	if (trainer->query().size() > 0) {
		Dvt::QueryPair qp = trainer->query().front();
		lbOrig->setText(STR2QSTR(qp.orig()));
		leTrans->setText("");
		lbCorrection->setText("");
		currentPm = QPixmap(QString(PIC_DIR) + "/kyle_natwallpaper_reduced.png");
		currentPmScaled = currentPm.scaled(width(), height(), 
			Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}
	
}

void SimpleTrainer::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	if (!currentPmScaled.isNull()) {
		if (currentPmScaled.width() != width() || currentPmScaled.height() != height())
			currentPmScaled = currentPm.scaled(width(), height(), 
				Qt::KeepAspectRatio, Qt::SmoothTransformation);
		painter.drawPixmap(0, 0, width(), height(), currentPmScaled);
			
	}
}
