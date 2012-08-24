/** @file
 * xank - Expression Evaluator, Function implementions.
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

static int FxAdd (XAtom *apAtoms[], uint64_t cAtoms)
{
    NOREF(apAtoms);
    NOREF(cAtoms);
    return INF_SUCCESS;
}

const XFunction XEvaluator::m_sFunctions[] =
{
    XFunction(1, SIZE_MAX, "avg",       FxAdd, "Average", "Returns the arithmetic average."),
    XFunction(1, SIZE_MAX, "fact",      FxAdd, "Factorial", "Returns the factorial."),
};

