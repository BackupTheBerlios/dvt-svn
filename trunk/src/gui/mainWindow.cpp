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
#include "dlgTrainerSelect.h"
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
	simpleTrainer = NULL; //new SimpleTrainer(this, centralWidget());
	flashCardTrainer = new FlashCardTrainer(this, centralWidget());
	
	dlgLessonMetaEdit = new DlgLessonMetaEdit(this);
	dlgTrainerSelect = new DlgTrainerSelect(this);
	
	lessonSelect->show();
	lessonEdit->hide();
//	simpleTrainer->hide();
	flashCardTrainer->hide();
	
	hbox->addWidget(lessonSelect);
	hbox->addWidget(lessonEdit);
//	hbox->addWidget(simpleTrainer);
	hbox->addWidget(flashCardTrainer);
	
	currentModule = lessonSelect;
	lessonSelect->loadLessonFiles();
	if (lessonSelect->toolBar() != NULL)
		addToolBar(lessonSelect->toolBar());
	lessonSelect->show();
	
	actionOpen->setEnabled(false);
	
}

MainWindow::~MainWindow()
{
}

/**
 * Sets the current active module.
 * 
 * @param mod	Module
 * 
 * @return True if new module is active, false if old module didn't close
 */
bool MainWindow::setModule(Module* mod)
{
	if (currentModule->mayClose()) {
		currentModule->hide();
		if (currentModule != lessonSelect)
			currentModule->setLessonFile(NULL);
		if (currentModule->toolBar() != NULL)
			removeToolBar(currentModule->toolBar());
			
		currentModule = mod;
		mod->setLessonFile(lessonSelect->lessonFile);
		if (mod->toolBar() != NULL) {
			qDebug("adding toolbar");
			addToolBar(mod->toolBar());
			mod->toolBar()->show();
			
		}
		mod->show();
		
		return true;
		
	} else {
		return false;
		
	}
}

void MainWindow::closeEvent(QCloseEvent* e)
{
	if (currentModule->mayClose()) {
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
	
	if (setModule(lessonSelect)) {
		actionOpen->setEnabled(false);
		actionEdit->setEnabled(true);
		
	}
	
}

void MainWindow::on_actionEdit_triggered(bool checked)
{
	Q_UNUSED(checked);
	
	if (setModule(lessonEdit)) {
		actionOpen->setEnabled(true);
		actionEdit->setEnabled(false);
		
	}
	
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
	
	dlgTrainerSelect->setLessonFile(lessonSelect->lessonFile);
	QDialog::DialogCode res = (QDialog::DialogCode) dlgTrainerSelect->exec();
	if (res == QDialog::Accepted) {
		Module* trainer = dlgTrainerSelect->trainer;
		if (trainer != NULL) {
			if (setModule(trainer)) {
				actionOpen->setEnabled(true);
				actionEdit->setEnabled(true);
				
			}
			
		}
		
	}
	
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
