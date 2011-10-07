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

#include <stdint.h>

static int FxAdd (Atom *apAtoms_[], uint64_t cAtoms_)
{
    return INF_SUCCESS;
}

const Operator Evaluator::m_sOperators[] =
{
    /*     Id        Pri    Associativity            cParams  Name   pfn    ShortHelp      LongHelp */

    /* Special Operators*/
    Operator(XANK_OPEN_PARANTHESIS_OPERATOR_ID,
                      99, enmOperatorDirectionNone,   0,      "(",   NULL,  "(<expr>",        "Begin subexpresion or function." },
    Operator(XANK_CLOSE_PARANTHESIS_OPERATOR_ID,
                      99,  enmOperatorDirectionNone,  0,      ")",   NULL,  "<expr>)",        "End subexpression or function." },
    Operator(XANK_PARAM_SEPARATOR_OPERATOR_ID,
                       0,  enmOperatorDirectionLeft,  2,      ",",   NULL,  "<expr>, <expr>", "Function Parameter separator." },
    Operator(XANK_ASSIGNMENT_OPERATOR_ID,
                       0,  enmOperatorDirectionLeft,  2,      "=",   NULL, "<lval>=<rval>",   "Assignment operator." },
}

const Function Evaluator::m_sFunctions[] =
{
    Function(1, UINT64_MAX, "avg",       FxAdd, "Average", "Returns the arithmetic average."),
    Function(1, UINT64_MAX, "fact",      FxAdd, "Factorial", "Returns the factorial."),
};


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
    std::string sNum;
    const char *pszStart = pszExpr;
    int iRadix = 0;
    uint64_t i = 0;

    /*
     * Octal prefix ("0")
     */
    if (*pszExpr == '0')
    {
        ++pszExpr;
        while (*pszExpr)
        {
            if (*pszExpr >= '0' && *pszExpr < '8')
                szNum[i++] = *pszExpr;
            else if (!isspace(*pszExpr))
                break;

            pszExpr++;
        }
        iRadix = 8;

        /*
         * Hexadecimal prefix ("0x" or "0X") or binary prefix ("0b" or "0B")
         */
        if (i == 0)
        {
            if (*pszExpr == 'x' || *pszExpr == 'X')
            {
                ++pszExpr;
                while (*pszExpr)
                {
                    if (   (*pszExpr >= '0' && *pszExpr <= 'F')
                        || (*pszExpr >= 'a' && *pszExpr <= 'f'))
                    {
                        szNum[i++] = *pszExpr;
                    }
                    else if (!isspace(*pszExpr))
                        break;

                    pszExpr++;
                }
                iRadix = 16;
            }
            else if (*pszExpr == 'b' || *pszExpr == 'B')
            {
                ++pszExpr;
                while (*pszExpr)
                {
                    if (*pszExpr == '1' || *pszExpr == '0')
                        sNum[i++] = *pszExpr;
                    else if (!isspace(*pszExpr))
                        break;

                    pszExpr++;
                }
                iRadix = 2;
            }
        }
    }

    /*
     * If nothing has been accumulated in "sNum", we've not got any recognized
     * numeric under whatever radices we've checked. Reset pointer to the original &
     * try parsing without any prefix.
     */
    if (i == 0)
    {
        pszExpr = pszStart;
        iRadix = 0;

        /*
         * Hexadecimal sans prefix, or Decimal.
         */
        while (*pszExpr)
        {
            if (isdigit(*pszExpr))
            {
                szNum[i++] = *pszExpr;
            }
            else if (*pszExpr == '.')
            {
                if (iRadix == 0)    /* eg:  ".5" */
                {
                    szNum[i++] = *pszExpr;
                    iRadix = 10;
                }
                else                /* eg: "fa.5" or "10.5.5" */
                {
                    iRadix = -1;
                    break;
                }
            }
            else if (   (*pszExpr >= 'A' && *pszExpr <= 'F')
                     || (*pszExpr >= 'a' && *pszExpr <= 'f'))
            {
                if (iRadix != 10)   /* eg: "af" or "53a"  */
                    iRadix = 16;
                else                /* eg: ".af" */
                {
                    iRadix = -1;
                    break;
                }
                szNum[i++] = *pszExpr;
            }
            else if (!isspace(*pszExpr))
                break;

            pszExpr++;
            if (i >= sizeof(szNum) - 1)
                break;
        }

        if (i > 0 && iRadix == 0)
            iRadix = 10;
    }

    if (   i == 0
        || iRadix == 0)
    {
        pszExpr = pszStart;
        return NULL;
    }

    /*
     * eg: "b2mb", we've read "b2" as hexadecimal and then check if an alphabet is following
     * the number, if so we don't interpret it as a hexadecimal number because numbers are
     * never suffixed.
     */
    if (isalpha(*pszExpr))
        return NULL;

    return NULL;
}


Atom *Evaluator::ParseOperator(const char *pcszExpr, const char **ppcszEnd, const Atom *pcPreviousAtom)
{
    for (uint64_t i = 0; i < g_cOperators; i++)
    {
        size_t cbOperator =

            StrLen(g_aOperators[i].pszOperator);
        if (!StrNCmp(g_aOperators[i].pszOperator, pszExpr, cbOperator))
        {
            /*
             * Verify if there are enough parameters on the queue for left associative operators.
             * e.g for binary '-', the previous atom must exist and must not be an open parenthesis or any
             * other operator.
             */
            if (g_aOperators[i].Direction == enmDirLeft)
            {
                /* e.g: "-4" */
                if (!pPreviousAtom)
                    continue;

                /* e.g: "(-4"  and "(expr)-4" */
                if (   AtomIsOperator(pPreviousAtom)
                    && AtomIsOpenParenthesis(pPreviousAtom))
                    continue;
            }

            PATOM pAtom = MemAlloc(sizeof(ATOM));
            if (!pAtom)
                return NULL;
            pAtom->Type = enmAtomOperator;
            pAtom->u.pOperator = &g_aOperators[i];
            pszExpr += cbOperator;
            *ppszEnd = pszExpr;
            return pAtom;
        }
    }
    return NULL;
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
