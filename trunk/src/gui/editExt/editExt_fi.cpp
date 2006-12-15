/** @file
 * 
 * EditExt_fi - Part of the Qt4-GUI of the portable Dictionary and 
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

#include "editExt_fi.h"

#include "lessonEdit.h"

#include "dvtCore.h"
#include "dvtLanguageProfile.h"

#include <string>
#include <map>

using namespace std;

#define GEN_ID	"2"
#define PART_ID	"3"

EditExt_fi::EditExt_fi(LessonEdit* ledit, Dvt::LanguageProfile* lp, QWidget* parent)
	:EditExt(ledit, lp, parent)
{
	setupUi(this);
	
	twiEntry = NULL;
	e = NULL;
	
	cboxConjClass->addItem(trUtf8("Select conjugation class"), 0);
	lp = core->getLanguageProfile("fi");
	map<string, Dvt::Conjugation*>::const_iterator it;
	for (it = lp->conjugations().begin(); it != lp->conjugations().end(); it++) {
		Dvt::Conjugation* c = it->second;
		cboxConjClass->addItem(QString::fromUtf8(c->name().c_str()), (int) c);
	}
	
	lbGenitive->setText(QString::fromUtf8(lp->getCase(GEN_ID)->name().c_str()) + ":");
	lbPartitive->setText(QString::fromUtf8(lp->getCase(PART_ID)->name().c_str()) + ":");
	
	frmConj->hide();
	
	show();
}

EditExt_fi::~EditExt_fi()
{
}

void EditExt_fi::setNextTabWidget(QWidget* w)
{
	if (w != NULL) setTabOrder(cboxConjClass, w);
}

void EditExt_fi::focusInEvent(QFocusEvent* event)
{
	Q_UNUSED(event);
	if (frmDecl->isVisible())
		leGenitive->setFocus();
	else if (frmConj->isVisible())
		leFirstPersSg->setFocus();
}

void EditExt_fi::changeCurrentEntry(TwiEntry* entry)
{
	twiEntry = entry;
	if (entry != NULL) {
		if (entry->trainingEntry->trans()->languageProfile() == lp)
			e = entry->trainingEntry->trans();
		else
			e = entry->trainingEntry->orig();
		
		leGenitive->setText(QString::fromUtf8(e->getDecl(GEN_ID).c_str()));
		lePartitive->setText(QString::fromUtf8(e->getDecl(PART_ID).c_str()));
		leFirstPersSg->setText(QString::fromUtf8(e->getConj(Dvt::ppSingular1).c_str()));
		int i = cboxConjClass->findData((int) e->conjClass());
		if (i >= 0)
			cboxConjClass->setCurrentIndex(i);
		else
			cboxConjClass->setCurrentIndex(0);
		
		switch (e->wordClass()) {
			case Dvt::WordClass::Verb: {
				frmConj->show();
				frmDecl->hide();
				break;
			}
			default: {
				frmDecl->show();
				frmConj->hide();
			}
		}
		
	} else {
		leGenitive->setText("");
		lePartitive->setText("");
		leFirstPersSg->setText("");
		cboxConjClass->setCurrentIndex(0);
		
	}
	
}

void EditExt_fi::postCurrentEntry(TwiEntry* entry)
{
	twiEntry = entry;
	if (entry != NULL) {
		if (entry->trainingEntry->trans()->languageProfile() == lp)
			e = entry->trainingEntry->trans();
		else
			e = entry->trainingEntry->orig();
			
		string gen = leGenitive->text().trimmed().toUtf8().data();
		string part = lePartitive->text().trimmed().toUtf8().data();
		string fps = leFirstPersSg->text().trimmed().toUtf8().data();
		Dvt::Conjugation* cc = (Dvt::Conjugation*) cboxConjClass->itemData(cboxConjClass->currentIndex()).toInt();
		
		bool edited = e->getDecl(GEN_ID) != gen;
		edited = edited || e->getDecl(PART_ID) != part;
		edited = edited || e->getConj(Dvt::ppSingular1) != fps;
		edited = edited || e->conjClass() != cc;
		
		if (edited) {
			e->setDecl(GEN_ID, gen);
			e->setDecl(PART_ID, part);
			e->setConj(Dvt::ppSingular1, fps);
			e->setConjClass(cc);
			lessonEdit->setEdited(edited);
		}
		
	}
	
}

void EditExt_fi::on_cboxConjClass_currentIndexChanged(int index)
{
	Dvt::Conjugation* cc = (Dvt::Conjugation*) cboxConjClass->itemData(index).toInt();
	if (twiEntry != NULL && e != NULL && e->conjClass() != cc) {
		postCurrentEntry(twiEntry);
		lessonEdit->emitCurrentEntryChanged();
	}
}
