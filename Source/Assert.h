/** @file
 * xank - Assert, header.
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

#ifndef XANK_ASSERT_H
# define XANK_ASSERT_H

#include <cstdlib>

#include "XGenericDefs.h"

#if defined(_MSC_VER) && defined(XANK_OS_WINDOWS)
# define XANK_PRETTY_FUNC       __FUNCSIG__
#else
# define XANK_PRETTY_FUNC       __PRETTY_FUNCTION__
#endif

#ifdef XANK_DEBUG
# define Assert(expr)  \
    do { \
        if (!(expr)) \
        { \
            AssertMsg(#expr, __LINE__, __FILE__, XANK_PRETTY_FUNC); \
            abort(); \
        } \
    } while (0)
#else
# define Assert(expr)     do { } while (0)
#endif

#define AssertReturn(expr, rc)  \
    do { \
        if (!(expr)) \
        { \
            AssertMsg(#expr, __LINE__, __FILE__, XANK_PRETTY_FUNC); \
            return (rc); \
        } \
    } while (0)

#define AssertReturnVoid(expr)  \
    do { \
        if (!(expr)) \
        { \
            AssertMsg(#expr, __LINE__, __FILE__, XANK_PRETTY_FUNC); \
            return; \
        } \
    } while (0)

/* To avoid warnings we NOREF the temporary, thus this cannot be used in headers.
   If we need this macro in headers, make a new version without the NOREF. */
#define AssertCompile(expr) \
    { \
       char ____compileTimeAssertFailed[(expr)?1:-1]; \
       ____compileTimeAssertFailed[0]=0; \
	   NOREF(____compileTimeAssertFailed); \
    }

#define AssertRelease(expr) \
    do { \
        if (!(expr)) \
        { \
            AssertMsg(#expr, __LINE__, __FILE__, XANK_PRETTY_FUNC); \
            abort(); \
        } \
    } while (0)

extern void AssertMsg(const char *pcszExpr, unsigned uLine, const char *pcszFile, const char *pcszFunction);

#endif /* XANK_ASSERT_H */
