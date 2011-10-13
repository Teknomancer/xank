/** @file
 * xank - Console Input/Output, implementation.
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

#include "ConsoleIO.h"
#include "ConsoleColors.h"
#include "CStringOps.h"
#include "XErrors.h"

#include <cstdio>
#include <cstdarg>

static const bool g_fxTermColors = true;
static const char *const g_aszConsoleColors[] =
{
    "\033[0m",
    "\033[0;30m",
    "\033[0;31m",
    "\033[0;32m",
    "\033[0;33m",
    "\033[0;34m",
    "\033[0;35m",
    "\033[0;36m",
    "\033[0;37m",
    "\033[1;30m",
    "\033[1;31m",
    "\033[1;32m",
    "\033[1;33m",
    "\033[1;34m",
    "\033[1;35m",
    "\033[1;36m",
    "\033[1;37m"
};


void AssertPrintf(const char *pcszAssertMsg, ...)
{
    va_list FmtArgs;
    char szBuf[2048];

    va_start(FmtArgs, pcszAssertMsg);
    vsnprintf(szBuf, sizeof(szBuf) - 1, pcszAssertMsg, FmtArgs);
    va_end(FmtArgs);

    char *pszBuf = szBuf;
    fprintf(stderr, "Assertion Failed!\n%s\n", pszBuf);
}


void ErrorPrintf(int rc, const char *pcszError, ...)
{
    va_list FmtArgs;
    char szBuf[2048];

    va_start(FmtArgs, pcszError);
    vsnprintf(szBuf, sizeof(szBuf) - 1, pcszError, FmtArgs);
    va_end(FmtArgs);

    char *pszBuf = StrStripLF(szBuf, NULL /* pfStripped */);
    const ErrorMessage *pcErrorMsg = ErrorMessageForRC(rc);
    if (pcErrorMsg)
    {
        fprintf(stderr, "%sError!%s %s rc=%s%s%s (%d)\n\n", g_aszConsoleColors[enmConsoleColorBoldRed],
                    g_aszConsoleColors[enmConsoleColorReset], pszBuf,
                    g_aszConsoleColors[enmConsoleColorRed], pcErrorMsg->pcszName,
                    g_aszConsoleColors[enmConsoleColorReset], rc);
    }
    else
        fprintf(stderr, "Extreme error! Missing pcErrorMsg!\n");
}


void ColorPrintf(ConsoleColor enmColor, const char *pcszMsg, ...)
{
    va_list FmtArgs;
    char szBuf[2048];

    va_start(FmtArgs, pcszMsg);
    vsnprintf(szBuf, sizeof(szBuf) - 1, pcszMsg, FmtArgs);
    va_end(FmtArgs);

    bool fNewLine = true;
    char *pszBuf = StrStripLF(szBuf, &fNewLine);

    const char *pcszColorCode = g_aszConsoleColors[enmColor];

    fprintf(stdout, "%s%s%s%s",
            g_fxTermColors ? pcszColorCode : "",
            pszBuf,
            CIOCOLOR_RESET,
            fNewLine ? "\n" : "");
}


void DebugPrintf(const char *pcszMsg, ...)
{
    va_list FmtArgs;
    char szBuf[2048];

    va_start(FmtArgs, pcszMsg);
    vsnprintf(szBuf, sizeof(szBuf) - 1, pcszMsg, FmtArgs);
    va_end(FmtArgs);

    bool fNewLine = true;
    char *pszBuf = StrStripLF(szBuf, &fNewLine);

    fprintf(stderr, "%s%s%s", fNewLine ? "  dbg:" : "", pszBuf, fNewLine ? "\n" : "");
}

