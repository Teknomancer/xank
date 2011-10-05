/** @file
 * xank - Console Input/Output, header.
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

#ifndef XANK_CONSOLE_IO_H
# define XANK_CONSOLE_IO_H

/**
 * Prints an error with appropriate formatting to console (stdout).
 *
 * @param rc                    The error code.
 * @param pszError              Any additional message to accompany the error
 *                              code, va_args style.
 */
void ErrorPrintf(int rc, char *pszError, ...);

/**
 * Prints colored output to console (stdout)
 *
 * @param pszColorCode          The name of the colour (one of the valid colors
 *                              see CIOCOLOR_* macros).
 * @param pszMsg                The message, va_args style.
 */
void ColorPrintf(char *pszColorCode, char *pszMsg, ...);

/**
 * Prints debug message to console (stderr).
 *
 * @param pszMsg                The message, va_args style.
 */
void DebugPrintf(char *pszMsg, ...);

#ifdef XANK_DEBUG
/** Print debug message to console (stderr), NOP in release builds */
# define DEBUGPRINTF(s)         DebugPrintf s
#else
/** Print debug message to console (stderr), NOP in release builds */
# define DEBUGPRINTF(s)         do { } while (0)
#endif

#endif /* XANK_CONSOLE_IO_H */

