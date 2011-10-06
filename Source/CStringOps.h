/** @file
 * xank - C string operations, header.
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

#ifndef XANK_C_STRING_OPS_H
# define XANK_C_STRING_OPS_H

/**
 * Strip a string of leading and trailing white spaces.
 *
 * @param   pszBuf    The string to be stripped.
 * @return char *:Pointer to stripped string.
 */
char *StrStrip(char *pszBuf);


/**
 * Strip a string of trailing new line (\n)
 *
 * @param   pszBuf          The string to be stripped.
 * @param   pfStripped      Where to store if the line was stripped or not, can be NULL.
 * @returns char *: Pointer to stripped string.
 */
char *StrStripLF(char *pszBuf, bool *pfStripped);

#endif /* XANK_C_STRING_OPS_H */
