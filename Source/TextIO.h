/** @file
 * xank - TextIO, header.
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

#ifndef XANK_TEXTIO_H
# define XANK_TEXTIO_H

class TextIO
{
    public:
        TextIO();
        virtual ~TextIO();

        virtual void            AssertPrintf(const char *pcszAssertMsg, ...) = 0;
        virtual void            ErrorPrintf(int rc, const char *pcszMsg, ...) = 0;
};

#endif /* XANK_TEXTIO_H */

