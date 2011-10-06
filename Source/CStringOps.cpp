/** @file
 * xank - C string operations, implementation.
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

#include "CStringOps.h"
#include <cctype>
#include <cstring>

char *StrStrip(char *pszBuf)
{
    /* leading white space */
    while (isspace(*pszBuf))
        pszBuf++;

    /* trailing white space */
    char *pszEnd = strchr(pszBuf, '\0');
    while (--pszEnd > pszBuf && isspace(*pszEnd))
        *pszEnd = '\0';

    return pszBuf;
}

char *StrStripLF(char *pszBuf, bool *pfStripped)
{
    if (pfStripped)
        *pfStripped = false;
    if (pszBuf)
    {
        size_t cbBuf = strlen(pszBuf);
        if (   cbBuf > 1
            && pszBuf[cbBuf - 1] == '\n')
        {
            pszBuf[cbBuf - 1] = '\0';
            if (pfStripped)
                *pfStripped = true;
        }
    }
    return pszBuf;
}

