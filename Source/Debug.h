/** @file
 * xank - Debug, header.
 */

/*
 * Copyright (C) 2011 Ramshankar (aka Teknomancer)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef XANK_DEBUG_H
# define XANK_DEBUG_H

#include "ConsoleIO.h"

#ifdef XANK_DEBUG
/** Print debug message to console (stderr), NOP in release builds */
# define DEBUGPRINTF(s)         ConsoleIO::DebugPrintf s
#else
/** Print debug message to console (stderr), NOP in release builds */
# define DEBUGPRINTF(s)         do { } while (0)
#endif

#endif /* XANK_DEBUG_H */

