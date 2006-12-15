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

#ifndef TWIENTRY_H_
#define TWIENTRY_H_

#include "dvtWordClass.h"
#include "dvtEntry.h"
#include "dvtTrainingEntry.h"

#include <QLabel>
#include <QColor>
#include <QMouseEvent>
#include <QTableWidgetItem>

class TwLabel;
class TwiCellEntry;
class TwiEntry;

class TwLabel : public QLabel
{
private:
	QTableWidget* tw;
	QTableWidgetItem* twi;
	
public:
	TwLabel(QTableWidget* tw, QTableWidgetItem* twi);
	
	void setHighlighted(bool highlighted);
	
protected:
	virtual void mousePressEvent(QMouseEvent* e);
};

class TwiCellEntry : public QTableWidgetItem
{
public:
	TwiEntry* entry;
	TwLabel* label;
	int col;
	
	TwiCellEntry(TwiEntry* entry, int row, int col, bool useLabel = true, int type = Type);
	virtual ~TwiCellEntry();
	
	void refresh();
};

class TwiEntry
{
public:
	Dvt::TrainingEntry* trainingEntry;
	
	TwiCellEntry* trans;
	TwiCellEntry* orig;
	
	QTableWidget* tw;
	
	TwiEntry(Dvt::TrainingEntry* trainingEntry, int row, QTableWidget* tw, bool useLabel = true);
	
	void refresh();
	void cellEntryDestroyed(TwiCellEntry* cellEntry);
	void setSelected(bool selected);
	
	Dvt::Entry* operator[](int col);
	
};

#endif /*TWIENTRY_H_*/
