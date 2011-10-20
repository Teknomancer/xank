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

#include "XEvaluator.h"
#include "Assert.h"
#include "XEvaluatorDefs.h"
#include "XAtom.h"
#include "XFunction.h"
#include "XGenericDefs.h"
#include "XOperator.h"
#include "XErrors.h"

#ifdef XANK_DEBUG
# include "ConsoleIO.h"
#endif

#include <cstring>
#include <stdint.h>
#include <gmp.h>

static int FxAdd (XAtom *apAtoms_[], uint64_t cAtoms_)
{
    return INF_SUCCESS;
}

const XOperator XEvaluator::m_sOperators[] =
{
    /*     Id        Pri    Associativity          cParams  Name   pfn    ShortHelp      LongHelp */
    /* Special Operators*/
    XOperator(XANK_OPEN_PARENTHESIS_OPERATOR_ID,
                      99, enmOperatorDirNone,        0,      "(",   NULL,  "(<expr>",        "Begin subexpresion or function."),
    XOperator(XANK_CLOSE_PARENTHESIS_OPERATOR_ID,
                      99,  enmOperatorDirNone,       0,      ")",   NULL,  "<expr>)",        "End subexpression or function."),
    XOperator(XANK_PARAM_SEPARATOR_OPERATOR_ID,
                       0,  enmOperatorDirLeft,       2,      ",",   NULL,  "<expr>, <expr>", "Function Parameter separator."),
    XOperator(XANK_ASSIGNMENT_OPERATOR_ID,
                       0,  enmOperatorDirLeft,       2,      "=",   NULL, "<lval>=<rval>",   "Assignment operator."),
};

const XFunction XEvaluator::m_sFunctions[] =
{
    XFunction(1, UINT64_MAX, "avg",       FxAdd, "Average", "Returns the arithmetic average."),
    XFunction(1, UINT64_MAX, "fact",      FxAdd, "Factorial", "Returns the factorial."),
};


XEvaluator::XEvaluator()
{
    /** @todo add parameters to accept settings */
    m_pOpenParenthesisOperator = &m_sOperators[0];   /* for now, later set this while verifying list of Operators */
}


XEvaluator::~XEvaluator()
{
}


int XEvaluator::Parse(const char *pcszExpr)
{
    return INF_SUCCESS;
}


XAtom *XEvaluator::ParseAtom(const char *pcszExpr, const char **ppcszEnd, const XAtom *pcPreviousAtom)
{
    DEBUGPRINTF(("ParseAtom\n"));
    XAtom *pAtom = NULL;
    while (*pcszExpr)
    {
        if (isspace(*pcszExpr))
        {
            pcszExpr++;
            continue;
        }

        pAtom = ParseOperator(pcszExpr, ppcszEnd, pcPreviousAtom);
        if (pAtom)
            break;

        pAtom = ParseFunction(pcszExpr, ppcszEnd, pcPreviousAtom);
        if (pAtom)
            break;

        pAtom = ParseNumber(pcszExpr, ppcszEnd, pcPreviousAtom);
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


XAtom *XEvaluator::ParseFunction(const char *pcszExpr, const char **ppcszEnd, const XAtom *pcPreviousAtom)
{
    static uint64_t cFunctions = XANK_ARRAY_ELEMENTS(m_sFunctions);
    for (uint64_t i = 0; i < cFunctions; i++)
    {
        size_t cbFunction = std::strlen(m_sFunctions[i].Name().c_str());
        if (!std::strncmp(m_sFunctions[i].Name().c_str(), pcszExpr, cbFunction))
        {
            /*
             * Skip over whitespaces till we encounter an open parenthesis.
             */
            pcszExpr += cbFunction;
            while (isspace(*pcszExpr))
                pcszExpr++;

            if (!std::strncmp(pcszExpr, m_pOpenParenthesisOperator->Name().c_str(),
                            std::strlen(m_pOpenParenthesisOperator->Name().c_str())))
            {
                XAtom *pAtom = new(std::nothrow) XAtom;
                if (!pAtom)
                    return NULL;
                pAtom->SetFunction(&m_sFunctions[i]);
                *ppcszEnd = pcszExpr;
                return pAtom;
            }
        }
    }
    return NULL;
}


XAtom *XEvaluator::ParseNumber(const char *pcszExpr, const char **ppcszEnd, const XAtom *pcPreviousAtom)
{
    std::string sNum;
    const char *pcszStart = pcszExpr;
    int iRadix = 0;
    bool fFloat = false;

    /*
     * Binary prefix.
     */
    if (*pcszExpr == 'b' || *pcszExpr == 'B')
    {
        ++pcszExpr;
        while (*pcszExpr)
        {
            if (*pcszExpr == '1' || *pcszExpr == '0')
                sNum += *pcszExpr;
            else if (!isspace(*pcszExpr))
                break;

            pcszExpr++;
        }
        iRadix = 2;
    }
    else if (*pcszExpr == '0')
    {
        /*
         * Octal prefix.
         */
        ++pcszExpr;
        while (*pcszExpr)
        {
            if (*pcszExpr >= '0' && *pcszExpr < '8')
                sNum += *pcszExpr;
            else if (!isspace(*pcszExpr))
                break;

            pcszExpr++;
        }
        iRadix = 8;

        /*
         * Hexadecimal prefix.
         */
        if (   sNum.empty()
            && (*pcszExpr == 'x' || *pcszExpr == 'X'))
        {
            ++pcszExpr;
            while (*pcszExpr)
            {
                if (   isdigit(*pcszExpr)
                    || (*pcszExpr >= 'a' && *pcszExpr <= 'f')
                    || (*pcszExpr >= 'A' && *pcszExpr <= 'F'))
                {
                    sNum += *pcszExpr;
                }
                else if (!isspace(*pcszExpr))
                    break;

                pcszExpr++;
            }
            iRadix = 16;
        }
    }

    /*
     * No explicit number prefixes, we fall back to parsing numbers based
     * on default settings.
     */
    if (sNum.empty())
    {
        pcszExpr = pcszStart;
        iRadix = 0;

        /*
         * Hexadecimal sans prefix, or Decimal.
         */
        while (*pcszExpr)
        {
            if (isdigit(*pcszExpr))
            {
                sNum += *pcszExpr;
                iRadix = 10;
            }
            else if (*pcszExpr == '.')
            {
                if (   fFloat == false
                    && (iRadix == 0 || iRadix == 10))  /* eg: ".5" or "2.5" */
                {
                    sNum += *pcszExpr;
                    iRadix = 10;
                    fFloat = true;
                }
                else                    /* eg: "fa.5" or "10.5.5" */
                {
                    iRadix = -1;
                    break;
                }
            }
            else if (   (*pcszExpr >= 'A' && *pcszExpr <= 'F')
                     || (*pcszExpr >= 'a' && *pcszExpr <= 'f'))
            {
                if (fFloat == false)    /* eg: "af" or "53a"  */
                {
                    iRadix = 16;
                    sNum += *pcszExpr;
                }
                else                    /* eg: ".af" */
                {
                    iRadix = -1;
                    break;
                }
            }
            else if (!isspace(*pcszExpr))
                break;

            pcszExpr++;
        }
    }

    if (   sNum.empty()
        || iRadix == -1)
    {
        pcszExpr = pcszStart;
        return NULL;
    }

    Assert(iRadix != 0);

    /*
     * We've parsed a number in a known radix, construct a number Atom for it.
     */
    XAtom *pAtom = new(std::nothrow) XAtom;
    if (!pAtom)
        return NULL;
    if (fFloat)
        pAtom->SetFloatFromStr(sNum.c_str(), iRadix);
    else
        pAtom->SetIntegerFromStr(sNum.c_str(), iRadix);
    *ppcszEnd = pcszExpr;
    return pAtom;
}


XAtom *XEvaluator::ParseOperator(const char *pcszExpr, const char **ppcszEnd, const XAtom *pcPreviousAtom)
{
    static uint64_t cOperators = XANK_ARRAY_ELEMENTS(m_sOperators);
    for (uint64_t i = 0; i < cOperators; i++)
    {
        size_t cbOperator = std::strlen(m_sOperators[i].Name().c_str());
        if (!std::strncmp(m_sOperators[i].Name().c_str(), pcszExpr, cbOperator))
        {
            /*
             * Verify if there are enough parameters on the queue for left associative operators.
             * e.g for binary '-', the previous atom must exist and must not be an open parenthesis or any
             * other operator.
             */
            if (m_sOperators[i].Dir() == enmOperatorDirLeft)
            {
                /* e.g: "-4" */
                if (!pcPreviousAtom)
                    continue;

                /*
                 * pPreviousAtom should never be close parentheis as it's deleted in Parse(),
                 * but included in here for the logical completeness.
                 */

                /* e.g: "(-4"  and "(expr)-4" */
                if (   pcPreviousAtom->Operator()
                    && pcPreviousAtom->Operator()->IsCloseParenthesis() == false)
                    continue;
            }

            XAtom *pAtom = new(std::nothrow) XAtom;
            if (!pAtom)
                return NULL;
            pAtom->SetOperator(&m_sOperators[i]);
            pcszExpr += cbOperator;
            *ppcszEnd = pcszExpr;
            return pAtom;
        }
    }
    return NULL;
}


XAtom *XEvaluator::ParseVariable(const char *pcszExpr, const char **ppcszEnd, const XAtom *pcPreviousAtom)
{
    return NULL;
}


XAtom *XEvaluator::ParseCommand(const char *pcszExpr,  const char **ppcszEnd,  const XAtom *pcPreviousAtom)
{
    return NULL;
}
