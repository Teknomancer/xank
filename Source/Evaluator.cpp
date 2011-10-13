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
#include "EvaluatorDefs.h"
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
                      99, enmOperatorDirectionNone,   0,      "(",   NULL,  "(<expr>",        "Begin subexpresion or function."),
    Operator(XANK_CLOSE_PARANTHESIS_OPERATOR_ID,
                      99,  enmOperatorDirectionNone,  0,      ")",   NULL,  "<expr>)",        "End subexpression or function."),
    Operator(XANK_PARAM_SEPARATOR_OPERATOR_ID,
                       0,  enmOperatorDirectionLeft,  2,      ",",   NULL,  "<expr>, <expr>", "Function Parameter separator."),
    Operator(XANK_ASSIGNMENT_OPERATOR_ID,
                       0,  enmOperatorDirectionLeft,  2,      "=",   NULL, "<lval>=<rval>",   "Assignment operator."),
};

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
    const char *pszStart = pcszExpr;
    int iRadix = 0;
    uint64_t i = 0;

    /*
     * Octal prefix ("0")
     */
    if (*pcszExpr == '0')
    {
        ++pcszExpr;
        while (*pcszExpr)
        {
            if (*pcszExpr >= '0' && *pcszExpr < '8')
                sNum[i++] = *pcszExpr;
            else if (!isspace(*pcszExpr))
                break;

            pcszExpr++;
        }
        iRadix = 8;

        /*
         * Hexadecimal prefix ("0x" or "0X") or binary prefix ("0b" or "0B")
         */
        if (i == 0)
        {
            if (*pcszExpr == 'x' || *pcszExpr == 'X')
            {
                ++pcszExpr;
                while (*pcszExpr)
                {
                    if (   (*pcszExpr >= '0' && *pcszExpr <= 'F')
                        || (*pcszExpr >= 'a' && *pcszExpr <= 'f'))
                    {
                        sNum[i++] = *pcszExpr;
                    }
                    else if (!isspace(*pcszExpr))
                        break;

                    pcszExpr++;
                }
                iRadix = 16;
            }
            else if (*pcszExpr == 'b' || *pcszExpr == 'B')
            {
                ++pcszExpr;
                while (*pcszExpr)
                {
                    if (*pcszExpr == '1' || *pcszExpr == '0')
                        sNum[i++] = *pcszExpr;
                    else if (!isspace(*pcszExpr))
                        break;

                    pcszExpr++;
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
        pcszExpr = pszStart;
        iRadix = 0;

        /*
         * Hexadecimal sans prefix, or Decimal.
         */
        while (*pcszExpr)
        {
            if (isdigit(*pcszExpr))
            {
                sNum[i++] = *pcszExpr;
            }
            else if (*pcszExpr == '.')
            {
                if (iRadix == 0)    /* eg:  ".5" */
                {
                    sNum[i++] = *pcszExpr;
                    iRadix = 10;
                }
                else                /* eg: "fa.5" or "10.5.5" */
                {
                    iRadix = -1;
                    break;
                }
            }
            else if (   (*pcszExpr >= 'A' && *pcszExpr <= 'F')
                     || (*pcszExpr >= 'a' && *pcszExpr <= 'f'))
            {
                if (iRadix != 10)   /* eg: "af" or "53a"  */
                    iRadix = 16;
                else                /* eg: ".af" */
                {
                    iRadix = -1;
                    break;
                }
                sNum[i++] = *pcszExpr;
            }
            else if (!isspace(*pcszExpr))
                break;

            pcszExpr++;
        }

        if (i > 0 && iRadix == 0)
            iRadix = 10;
    }

    if (   i == 0
        || iRadix == 0)
    {
        pcszExpr = pszStart;
        return NULL;
    }

    /*
     * eg: "b2mb", we've read "b2" as hexadecimal and then check if an alphabet is following
     * the number, if so we don't interpret it as a hexadecimal number because numbers are
     * never suffixed.
     */
    if (isalpha(*pcszExpr))
        return NULL;

    return NULL;
}


Atom *Evaluator::ParseOperator(const char *pcszExpr, const char **ppcszEnd, const Atom *pcPreviousAtom)
{
    static uint64_t cOperators = sizeof(m_sOperators / sizeof(Operator));
    for (uint64_t i = 0; i < cOperators; i++)
    {
        size_t cbOperator = strlen(m_sOperators[i].Name().c_str());
        if (!strncmp(m_sOperators[i].Name().c_str(), pcszExpr, cbOperator))
        {
            /*
             * Verify if there are enough parameters on the queue for left associative operators.
             * e.g for binary '-', the previous atom must exist and must not be an open parenthesis or any
             * other operator.
             */
            if (m_sOperators[i].Direction() == enmOperatorDirectionLeft)
            {
                /* e.g: "-4" */
                if (!pcPreviousAtom)
                    continue;

                /* e.g: "(-4"  and "(expr)-4" */
                if (   pcPreviousAtom->Operator()
                    && pcPreviousAtom->Operator()->IsOpenParanthesis())
                    continue;
            }

            Atom *pAtom = new Atom;
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
