/** @file
 * xank - Expression Evaluator, implementation.
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

#include "Evaluator.h"
#include "Atom.h"
#include "Operator.h"
#include "Errors.h"

#ifdef XANK_DEBUG
# include "ConsoleIO.h"
#endif

Evaluator::Evaluator()
{
    /** @todo add parameters to accept settings */
}


Evaluator::~Evaluator()
{
}


int Evaluator::Parse(const char *pcszExpr)
{
    return INF_SUCCESS;
}


Atom *Evaluator::ParseAtom(const char *pcszExpr, const char **ppcszEnd, const Atom *pcPreviousAtom)
{
    DEBUGPRINTF(("ParseAtom\n"));
    Atom *pAtom = NULL;
    while (*pcszExpr)
    {
        if (isspace(*pcszExpr))
        {
            pcszExpr++;
            continue;
        }

        pAtom = ParseNumber(pcszExpr, ppcszEnd, pcPreviousAtom);
        if (pAtom)
            break;

        pAtom = ParseOperator(pcszExpr, ppcszEnd, pcPreviousAtom);
        if (pAtom)
            break;

        pAtom = ParseFunction(pcszExpr, ppcszEnd, pcPreviousAtom);
        if (pAtom)
            break;

        pAtom = ParseVariable(pcszExpr, ppcszEnd, pcPreviousAtom);
        if (pAtom)
            break;

        pAtom = ParseCommand(pcszExpr, ppcszEnd, pcPreviousAtom);
        if (pAtom)
            break;

        /** @todo hmm, think about this!? */
        *ppcszEnd = pcszExpr;
        break;
    }

    return pAtom;
}


Atom *Evaluator::ParseFunction(const char *pcszExpr, const char **ppcszEnd, const Atom *pcPreviousAtom)
{
    return NULL;
}


Atom *Evaluator::ParseNumber(const char *pcszExpr, const char **ppcszEnd, const Atom *pcPreviousAtom)
{
    return NULL;
}


Atom *Evaluator::ParseOperator(const char *pcszExpr, const char **ppcszEnd, const Atom *pcPreviousAtom)
{
    return NULL;
}


Atom *Evaluator::ParseVariable(const char *pcszExpr, const char **ppcszEnd, const Atom *pcPreviousAtom)
{
    return NULL;
}


Atom *Evaluator::ParseCommand(const char *pcszExpr,  const char **ppcszEnd,  const Atom *pcPreviousAtom)
{
    return NULL;
}
