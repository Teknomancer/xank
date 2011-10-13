/** @file
 * xank - Generic definitions, header.
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

#ifndef XANK_GENERIC_DEFS_H
# define XANK_GENERIC_DEFS_H

/** @def R_ARRAY_ELEMENTS
 * Returns the number of elements in an array.
 *
 * @param   aArray      An array.
 * @returns The number of elements in an array object.
 */
#define XANK_ARRAY_ELEMENTS(aArray)                     ( sizeof(aArray) / sizeof((aArray)[0]) )

/** @def R_MAX
 * Returns the greater of the two objects.
 *
 * @param   a           First object or POD type.
 * @param   b           Second object or POD type.
 * @returns The greater of the two objects.
 */
#define XANK_MAX(a,b)                                   ((a) >= (b) ? (a) : (b))

/** @def R_MIN
 * Returns the lesser of the two objects.
 *
 * @param   a           First object or POD type.
 * @param   b           Second object or POD type.
 * @returns The lesser of the two objects.
 */
#define XANK_MIN(a,b)                                   ((a) <= (b) ? (a) : (b))

/** @def NOREF
 * A no reference stub to shut up compiler warnings about unused variables.
 *
 * @param   a           Any unused variable.
 */
#define NOREF(a)                                     (void)(a)

#endif /* XANK_GENERIC_DEFS_H */

