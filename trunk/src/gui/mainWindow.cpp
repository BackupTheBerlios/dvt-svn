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
#include "flashCardTrainer.h"
#include "utils.h"

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
	sysLocale = QLocale::system();
	
	core = Dvt::Core::getInstance();
	core->setSystemLocale(QSTR2STR(sysLocale.name()));
	
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
	
	setStatusBar(NULL);
	
	QHBoxLayout* hbox = new QHBoxLayout(centralWidget());
	hbox->setMargin(0);
	
	lessonSelect = new LessonSelect(this, centralWidget());
	lessonEdit = new LessonEdit(this, centralWidget());
	dlgLessonMetaEdit = new DlgLessonMetaEdit(this);
	simpleTrainer = NULL; //new SimpleTrainer(this, centralWidget());
	flashCardTrainer = new FlashCardTrainer(this, centralWidget());
	
	lessonSelect->show();
	lessonEdit->hide();
//	simpleTrainer->hide();
	flashCardTrainer->hide();
	
	hbox->addWidget(lessonSelect);
	hbox->addWidget(lessonEdit);
//	hbox->addWidget(simpleTrainer);
	hbox->addWidget(flashCardTrainer);
	
	actionOpen->setEnabled(false);
	lessonSelect->loadLessonFiles();
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
	
	if (lessonEdit->mayClose() && flashCardTrainer->mayClose()) {
		flashCardTrainer->hide();
		flashCardTrainer->setLesson(NULL);
		lessonEdit->hide();
		lessonEdit->setLesson(NULL);
		lessonSelect->show();
		actionOpen->setEnabled(false);
		actionEdit->setEnabled(true);
	}
}

void MainWindow::on_actionEdit_triggered(bool checked)
{
	Q_UNUSED(checked);
	lessonSelect->hide();
	flashCardTrainer->hide();
	
	lessonEdit->setLessonFile(lessonSelect->lessonFile);
	lessonEdit->show();
	
	actionOpen->setEnabled(true);
	actionEdit->setEnabled(false);
}

void MainWindow::on_actionEditProperties_triggered(bool checked)
{
	Q_UNUSED(checked);
	dlgLessonMetaEdit->setFromLessonFile(lessonSelect->lessonFile);
	QDialog::DialogCode res = (QDialog::DialogCode) dlgLessonMetaEdit->exec();
	if (res == QDialog::Accepted)
		lessonSelect->lessonFile->write();
	dlgLessonMetaEdit->setFromLessonFile(NULL);
}

void MainWindow::on_actionTrain_triggered(bool checked)
{
	Q_UNUSED(checked);
	lessonSelect->hide();
	lessonEdit->hide();
	
	flashCardTrainer->setLessonFile(lessonSelect->lessonFile);
	flashCardTrainer->show();
	
	actionOpen->setEnabled(true);
	actionEdit->setEnabled(true);
}

void MainWindow::on_actionAboutDvt_triggered(bool checked)
{
	Q_UNUSED(checked);
	
	QMessageBox::about(this, APPNAME, trUtf8(
		"Dvt:: Dictionary and vocabulary trainer v%1\n" \
		"(c) 2006-2007 Denis Martin").arg(APPVERSION));
}

void MainWindow::on_actionAboutQt_triggered(bool checked)
{
	Q_UNUSED(checked);
	
	QMessageBox::aboutQt(this);
}
