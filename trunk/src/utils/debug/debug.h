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

#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdio.h>

#define DBG_GENERAL 	0
#define DBG_XML			1
#define DBG_GUI			2
#define DBG_DLP			3

#define DBGINIT dbgInit
#define DEBUG debug

void dbgInit(FILE* logFile);
void debug(const int cat, const char* msg, ...);

#endif /*DEBUG_H_*/
