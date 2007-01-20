/** @file
 * 
 * MainWindow - Part of the Qt4-GUI of the portable Dictionary and 
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

#include "mainWindow.h"

#include "lessonSelect.h"
#include "lessonEdit.h"
#include "dlgLessonMetaEdit.h"
#include "simpleTrainer.h"

#include "dvtExceptions.h"

#include <QMessageBox>
#include <QHBoxLayout>
#include <QWidget>
#include <QDir>
#include <QStringList>
#include <QCloseEvent>

MainWindow::MainWindow()
{
	setupUi(this);
	
	core = Dvt::Core::getInstance();
	
	try {
		core->init();
		
	} catch(Dvt::Exception e) {
		QMessageBox::critical(this, "DVT",
			QString("Dvt::Exception %1: %2")
				.arg(e.code)
				.arg(e.msg.c_str()),
			QMessageBox::Cancel);
		qApp->quit();
		exit(1);
		
	}
	
	QDir dir("lang");
	QStringList flagFiles = dir.entryList(QStringList() << "*.png");
	QStringList::const_iterator it;
	for (it = flagFiles.begin(); it != flagFiles.end(); it++) {
		QString lc = it->left(2);
		QIcon* icon = new QIcon(dir.path() + "/" + *it);
		if (!icon->isNull())
			flags[lc] = icon;
		else
			delete icon;
	}
	
	QHBoxLayout* hbox = new QHBoxLayout(centralWidget());
	hbox->setMargin(0);
	
	lessonSelect = new LessonSelect(this, centralWidget());
	lessonEdit = new LessonEdit(this, centralWidget());
	dlgLessonMetaEdit = new DlgLessonMetaEdit(this);
	simpleTrainer = new SimpleTrainer(this, centralWidget());
	
	lessonSelect->show();
	lessonEdit->hide();
	simpleTrainer->hide();
	
	hbox->addWidget(lessonSelect);
	hbox->addWidget(lessonEdit);
	hbox->addWidget(simpleTrainer);
	
	lessonSelect->loadLessons();
	lessonSelect->show();
	
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent* e)
{
	if (lessonEdit->mayClose()) {
		e->accept();
		
	} else {
		e->ignore();
		
	}
}

void MainWindow::on_actionQuit_triggered(bool checked)
{
	Q_UNUSED(checked);
	qApp->quit();
}

void MainWindow::on_actionOpen_triggered(bool checked)
{
	Q_UNUSED(checked);
	if (lessonEdit->mayClose() && simpleTrainer->mayClose()) {
		simpleTrainer->hide();
		simpleTrainer->setLesson(NULL);
		lessonEdit->hide();
		lessonEdit->setLesson(NULL);
		lessonSelect->show();
	}
}

void MainWindow::on_actionEdit_triggered(bool checked)
{
	Q_UNUSED(checked);
	lessonSelect->hide();
	simpleTrainer->hide();
	
	lessonEdit->setLesson(lessonSelect->lesson);
	lessonEdit->show();
}

void MainWindow::on_actionTrain_triggered(bool checked)
{
	Q_UNUSED(checked);
	lessonSelect->hide();
	lessonEdit->hide();
	
	simpleTrainer->setLesson(lessonSelect->lesson);
	simpleTrainer->show();
}
