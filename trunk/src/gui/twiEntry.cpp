/** @file
 * 
 * TwiEntry - Part of the Qt4-GUI of the portable Dictionary and 
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

#include "twiEntry.h"

#include "utils.h"

#include <QPalette>

/* TwLabel ********************************************************************/

TwLabel::TwLabel(QTableWidget* tw, QTableWidgetItem* twi)
	:QLabel(tw) 
{
	this->tw = tw;
	this->twi = twi;
	setMargin(2);
}

void TwLabel::setHighlighted(bool highlighted)
{
	if (highlighted) {
		if (foregroundRole() != QPalette::HighlightedText)
			setForegroundRole(QPalette::HighlightedText);
	} else {
		if (foregroundRole() != QPalette::WindowText)
			setForegroundRole(QPalette::WindowText);
	}
}

void TwLabel::mousePressEvent(QMouseEvent* e) 
{
	tw->setCurrentItem(twi);
	e->accept();
}

/* TwiCellEntry ***************************************************************/

TwiCellEntry::TwiCellEntry(TwiEntry* entry, int row, int col, bool useLabel, int type)
	:QTableWidgetItem(type)
{
	this->entry = entry;
	this->col = col;
	
	if (useLabel) {
		label = new TwLabel(entry->tw, this);
		entry->tw->setCellWidget(row, col, label);
		
	} else {
		label = false;
		
	}
	
}

TwiCellEntry::~TwiCellEntry()
{
	entry->cellEntryDestroyed(this);
}

void TwiCellEntry::refresh()
{
	if (label != NULL)
		label->setText(STR2QSTR((*entry)[col]->longNameHtml()));
	else
		setText(STR2QSTR((*entry)[col]->longName()));
}

/* TwiEntry *******************************************************************/

TwiEntry::TwiEntry(Dvt::TrainingEntry* trainingEntry, int row, QTableWidget* tw, 
	bool useLabel)
{
	this->tw = tw;
	this->trainingEntry = trainingEntry;
	trans = new TwiCellEntry(this, row, 0, useLabel);
	orig = new TwiCellEntry(this, row, 1, useLabel);
	tw->setItem(row, 0, trans);
	tw->setItem(row, 1, orig);
	refresh();
}

void TwiEntry::refresh()
{
	if (trans != NULL) trans->refresh();
	if (orig != NULL) orig->refresh();
}

void TwiEntry::cellEntryDestroyed(TwiCellEntry* cellEntry)
{
	if (cellEntry == trans)
		trans = NULL;
	else if (cellEntry == orig)
		orig = NULL;
	else
		assert(false);
		
	if (trans == NULL && orig == NULL)
		delete this;
}

void TwiEntry::setSelected(bool selected)
{
	if (trans != NULL && trans->label != NULL)
		trans->label->setHighlighted(selected);
	if (orig != NULL && orig->label != NULL)
		orig->label->setHighlighted(selected);
}

Dvt::Entry* TwiEntry::operator[](int col)
{
	switch (col) {
		case 0: return trainingEntry->trans();
		case 1: return trainingEntry->orig();
		default: assert(false);
	}
}


