/** @file
 * 
 * debug
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

#include "debug.h"

#include <stdio.h>
#include <stdarg.h>

const char* dbg_strcat[] = {"GEN", "XML", "GUI", "DLP"}; 

static bool dbg_general = true;
static bool dbg_xml = true;
static bool dbg_gui = true;
static bool dbg_dlp = true;

static FILE* dbg_logFile = stderr;

#define DBG_FMTSTR_LEN	1024
static char dbg_strfmt[DBG_FMTSTR_LEN];

void dbgInit(FILE* logFile) {
	dbg_logFile = logFile;
}

void debug(const int cat, const char* msg, ...) 
{
	if ((DBG_GENERAL == cat && dbg_general) || 
		(DBG_XML == cat && dbg_xml) ||
		(DBG_GUI == cat && dbg_gui) ||
		(DBG_DLP == cat && dbg_dlp))
	{
		snprintf(dbg_strfmt, DBG_FMTSTR_LEN, "[%s] %s\n", dbg_strcat[cat], msg);
		
		va_list vargs;
		va_start(vargs, msg);
		vfprintf(dbg_logFile, dbg_strfmt, vargs);
		va_end(vargs);
		
		fflush(dbg_logFile);
	}
}
