/** @file
 * 
 * EditDetailVerb - Part of the Qt4-GUI of the portable Dictionary and 
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

#include "editDetailVerb.h"

#include "lessonEdit.h"
#include "utils.h"

using namespace std;

EditDetailVerb::EditDetailVerb(LessonEdit* ledit, Dvt::LanguageProfile* lp, QWidget* parent)
	:EditDetail(ledit, lp, parent)
{
	setupUi(this);
	
	twiEntry = NULL;
	e = NULL;
	
	if (lp->conjugations().size() == 0) {
		wConjClasses->hide();
		
	} else {
		cboxConjClass->addItem(trUtf8("Undefined"), 0);
		map<string, Dvt::Conjugation*>::const_iterator it;
		for (it = lp->conjugations().begin(); it != lp->conjugations().end(); it++) {
			Dvt::Conjugation* c = it->second;
			cboxConjClass->addItem(STR2QSTR(c->name()), (int) c);
		}
		
	}
	
	lbPpSg1->setText(STR2QSTR(lp->getPersonalPronoun(Dvt::ppSingular1)));
	lbPpSg2->setText(STR2QSTR(lp->getPersonalPronoun(Dvt::ppSingular2)));
	lbPpSg3->setText(STR2QSTR(lp->getPersonalPronoun(Dvt::ppSingular3)));
	lbPpPl1->setText(STR2QSTR(lp->getPersonalPronoun(Dvt::ppPlural1)));
	lbPpPl2->setText(STR2QSTR(lp->getPersonalPronoun(Dvt::ppPlural2)));
	lbPpPl3->setText(STR2QSTR(lp->getPersonalPronoun(Dvt::ppPlural3)));
	
	setTabOrder(cboxConjClass, lbPpSg1);
	setTabOrder(lbPpSg1, lbPpSg2);
	setTabOrder(lbPpSg2, lbPpSg3);
	setTabOrder(lbPpSg3, lbPpPl1);
	setTabOrder(lbPpPl1, lbPpPl2);
	setTabOrder(lbPpPl2, lbPpPl3);
	
}

EditDetailVerb::~EditDetailVerb()
{
}

void EditDetailVerb::changeCurrentEntry(TwiEntry* entry)
{
	twiEntry = entry;
	
	if (entry != NULL) {
		if (entry->trainingEntry->trans()->languageProfile() == lp)
			e = entry->trainingEntry->trans();
		else
			e = entry->trainingEntry->orig();
			
		leSg1->setText(STR2QSTR(e->getConj(Dvt::ppSingular1)));
		leSg2->setText(STR2QSTR(e->getConj(Dvt::ppSingular2)));
		leSg3->setText(STR2QSTR(e->getConj(Dvt::ppSingular3)));
		lePl1->setText(STR2QSTR(e->getConj(Dvt::ppPlural1)));
		lePl2->setText(STR2QSTR(e->getConj(Dvt::ppPlural2)));
		lePl3->setText(STR2QSTR(e->getConj(Dvt::ppPlural3)));
		
		int i = cboxConjClass->findData((int) e->conjClass());
		if (i >= 0)
			cboxConjClass->setCurrentIndex(i);
		else
			cboxConjClass->setCurrentIndex(0);
			
	} else {
		leSg1->setText("");
		leSg2->setText("");
		leSg3->setText("");
		lePl1->setText("");
		lePl2->setText("");
		lePl3->setText("");
		cboxConjClass->setCurrentIndex(0);
		
	}
	
}

void EditDetailVerb::postCurrentEntry(TwiEntry* entry)
{
	twiEntry = entry;
	
	if (entry != NULL) {
		if (entry->trainingEntry->trans()->languageProfile() == lp)
			e = entry->trainingEntry->trans();
		else
			e = entry->trainingEntry->orig();
			
		string s1 = leSg1->text().trimmed().toUtf8().data();
		string s2 = leSg2->text().trimmed().toUtf8().data();
		string s3 = leSg3->text().trimmed().toUtf8().data();
		string p1 = lePl1->text().trimmed().toUtf8().data();
		string p2 = lePl2->text().trimmed().toUtf8().data();
		string p3 = lePl3->text().trimmed().toUtf8().data();
		
		Dvt::Conjugation* cc = (Dvt::Conjugation*) cboxConjClass->itemData(cboxConjClass->currentIndex()).toInt();
		
		bool edited = e->conjClass() != cc;
		edited = edited || s1 != e->getConj(Dvt::ppSingular1);
		edited = edited || s2 != e->getConj(Dvt::ppSingular2);
		edited = edited || s3 != e->getConj(Dvt::ppSingular3);
		edited = edited || p1 != e->getConj(Dvt::ppPlural1);
		edited = edited || p2 != e->getConj(Dvt::ppPlural2);
		edited = edited || p3 != e->getConj(Dvt::ppPlural3);
		
		if (edited) {
			e->setConj(Dvt::ppSingular1, s1);
			e->setConj(Dvt::ppSingular2, s2);
			e->setConj(Dvt::ppSingular3, s3);
			e->setConj(Dvt::ppPlural1, p1);
			e->setConj(Dvt::ppPlural2, p2);
			e->setConj(Dvt::ppPlural3, p3);
			e->setConjClass(cc);
			lessonEdit->setEdited(edited);
			
		}
		
	}
	
}

void EditDetailVerb::on_cboxConjClass_currentIndexChanged(int index)
{
	Dvt::Conjugation* cc = (Dvt::Conjugation*) cboxConjClass->itemData(index).toInt();
	if (twiEntry != NULL && e != NULL && e->conjClass() != cc) {
		postCurrentEntry(twiEntry);
		lessonEdit->emitCurrentEntryChanged();
	}
}
