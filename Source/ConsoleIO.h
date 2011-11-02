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

#include "TextIO.h"
#ifdef XANK_OS_WINDOWS
# include <cstdint>
#else
# include <inttypes.h>
#endif

#if defined(FMT_D32) || defined(FMT_D64) || defined(FMT_I32) || defined(FMT_I64) || defined(FMT_U32) || defined(FMT_U64) || defined(FMT_X64)
# error bummer.
#endif

#if defined(XANK_OS_WINDOWS) && defined(_MSC_VER)
# define FMT_U8            "I8u"
# define FMT_I8            "I8i"
# define FMT_D32           "I32d"
# define FMT_D64           "I64d"
# define FMT_I32           "I32i"
# define FMT_I64           "I64i"
# define FMT_U32           "I32u"
# define FMT_U64           "I64u"
# define FMT_X64           "I64x"
#else
# define FMT_U8            PRIu8
# define FMT_I8            PRId8
# define FMT_D32           PRId32
# define FMT_D64           PRId64
# define FMT_I32           PRIi32
# define FMT_I64           PRIi64
# define FMT_U32           PRIu32
# define FMT_U64           PRIu64
# define FMT_X64           PRIx64
#endif

/**
 * Console text foreground color codes.
 */
enum ConsoleColor
{
    enmConsoleColorReset = 0,
    enmConsoleColorBlack,
    enmConsoleColorRed,
    enmConsoleColorGreen,
    enmConsoleColorYellow,
    enmConsoleColorBlue,
    enmConsoleColorPurple,
    enmConsoleColorCyan,
    enmConsoleColorWhite,
    enmConsoleColorBoldBlack,
    enmConsoleColorBoldRed,
    enmConsoleColorBoldGreen,
    enmConsoleColorBoldYellow,
    enmConsoleColorBoldBlue,
    enmConsoleColorBoldPurple,
    enmConsoleColorBoldCyan,
    enmConsoleColorBoldWhite
};

class ConsoleIO : public TextIO
{
    public:
        ConsoleIO();
        virtual ~ConsoleIO();

        virtual void            AssertPrintf(const char *pcszAssertMsg, ...);
        virtual void            ErrorPrintf(int rc, const char *pcszMsg, ...);

        /**
         * Set whether color output is enabled for the console output functions.
         *
         * @param fColorOutput      Whether color output is enabled or not.
         */
        virtual void            SetColor(bool fColorOutput);

        /**
         * Prints a message to the console (stdout) in a particular color.
         *
         * @param enmColor      The color code.
         * @param pcszMsg       The message, va_args style.
         */
        virtual void            ColorPrintf(ConsoleColor enmColor, const char *pcszMsg, ...);

        /**
         * Prints a message to the console (stdout) in resetted/default color.
         *
         * @param pcszMsg       The message, va_args style.
         */
        virtual void            Printf(const char *pcszMsg, ...);

        /**
         * Prints debug message to console (stderr).
         *
         * @param pcszMsg       The message, va_args style.
         */
        static void             DebugPrintf(const char *pcszMsg, ...);

    protected:
        bool                    m_fxTermColors;     /**< Whether color output is enabled. */
};

#endif /* XANK_CONSOLE_IO_H */

