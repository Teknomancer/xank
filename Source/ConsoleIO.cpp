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

/** @todo Probably a bad idea, but whatever should be readonly. Fix this
 *                later. */
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

ConsoleIO::ConsoleIO()
#ifdef XANK_OS_WINDOWS
    : m_fxTermColors(false)
#else
    : m_fxTermColors(true)
#endif
{
}


ConsoleIO::~ConsoleIO()
{
}


void ConsoleIO::AssertPrintf(const char *pcszAssertMsg, ...)
{
    va_list FmtArgs;
    char szBuf[2048];

    va_start(FmtArgs, pcszAssertMsg);
#ifdef XANK_OS_WINDOWS
    vsnprintf_s(szBuf, sizeof(szBuf), pcszAssertMsg, FmtArgs);
#else
    vsnprintf(szBuf, sizeof(szBuf) - 1, pcszAssertMsg, FmtArgs);
#endif
    va_end(FmtArgs);

    char *pszBuf = szBuf;
    fprintf(stderr, "Assertion Failed!\n%s\n", pszBuf);
}


void ConsoleIO::ErrorPrintf(int rc, const char *pcszError, ...)
{
    va_list FmtArgs;
    char szBuf[2048];

    va_start(FmtArgs, pcszError);
#ifdef XANK_OS_WINDOWS
    vsnprintf_s(szBuf, sizeof(szBuf), pcszError, FmtArgs);
#else
    vsnprintf(szBuf, sizeof(szBuf) - 1, pcszError, FmtArgs);
#endif
    va_end(FmtArgs);

    char *pszBuf = StrStripLF(szBuf, NULL /* pfStripped */);
    const ErrorMessage *pcErrorMsg = ErrorMessageForRC(rc);
    if (pcErrorMsg)
    {
        if (m_fxTermColors)
        {
            fprintf(stderr, "%sError!%s %s rc=%s%s%s (%d)\n\n", g_aszConsoleColors[enmConsoleColorBoldRed],
                        g_aszConsoleColors[enmConsoleColorReset], pszBuf,
                        g_aszConsoleColors[enmConsoleColorRed], pcErrorMsg->pcszName,
                        g_aszConsoleColors[enmConsoleColorReset], pcErrorMsg->rc);
        }
        else
            fprintf(stderr, "Error! %s rc=%s (%d)\n\n", pszBuf, pcErrorMsg->pcszName, pcErrorMsg->rc);
    }
    else
        fprintf(stderr, "Extreme error! Missing pcErrorMsg!\n");
}


void ConsoleIO::Printf(const char *pcszMsg, ...)
{
    va_list FmtArgs;
    char szBuf[2048];

    va_start(FmtArgs, pcszMsg);
#ifdef XANK_OS_WINDOWS
    vsnprintf_s(szBuf, sizeof(szBuf), pcszMsg, FmtArgs);
#else
    vsnprintf(szBuf, sizeof(szBuf) - 1, pcszMsg, FmtArgs);
#endif
    va_end(FmtArgs);

    bool fNewLine = true;
    char *pszBuf = StrStripLF(szBuf, &fNewLine);

    if (m_fxTermColors)
    {
        const char *pcszColorCode = g_aszConsoleColors[enmConsoleColorReset];

        fprintf(stdout, "%s%s%s%s", pcszColorCode, pszBuf, g_aszConsoleColors[enmConsoleColorReset],
                fNewLine ? "\n" : "");
    }
    else
        fprintf(stdout, "%s%s", pszBuf, fNewLine ? "\n" : "");
}

void ConsoleIO::ColorPrintf(ConsoleColor enmColor, const char *pcszMsg, ...)
{
    va_list FmtArgs;
    char szBuf[2048];

    va_start(FmtArgs, pcszMsg);
#ifdef XANK_OS_WINDOWS
    vsnprintf_s(szBuf, sizeof(szBuf), pcszMsg, FmtArgs);
#else
    vsnprintf(szBuf, sizeof(szBuf) - 1, pcszMsg, FmtArgs);
#endif
    va_end(FmtArgs);

    bool fNewLine = true;
    char *pszBuf = StrStripLF(szBuf, &fNewLine);

    if (m_fxTermColors)
    {
        const char *pcszColorCode = g_aszConsoleColors[enmColor];

        fprintf(stdout, "%s%s%s%s", pcszColorCode, pszBuf, g_aszConsoleColors[enmConsoleColorReset],
                fNewLine ? "\n" : "");
    }
    else
        fprintf(stdout, "%s%s", pszBuf, fNewLine ? "\n" : "");
}


void ConsoleIO::DebugPrintf(const char *pcszMsg, ...)
{
    va_list FmtArgs;
    char szBuf[2048];

    va_start(FmtArgs, pcszMsg);
#ifdef XANK_OS_WINDOWS
    vsnprintf_s(szBuf, sizeof(szBuf), pcszMsg, FmtArgs);
#else
    vsnprintf(szBuf, sizeof(szBuf) - 1, pcszMsg, FmtArgs);
#endif
    va_end(FmtArgs);

    bool fNewLine = true;
    char *pszBuf = StrStripLF(szBuf, &fNewLine);

    fprintf(stderr, "%s%s%s", fNewLine ? "  dbg:" : "", pszBuf, fNewLine ? "\n" : "");
}


void ConsoleIO::SetColor(bool fColorOutputEnable)
{
#ifndef XANK_OS_WINDOWS
    m_fxTermColors = fColorOutputEnable;
#endif
}
