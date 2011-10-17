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

static int FxAdd (XAtom *apAtoms_[], uint64_t cAtoms_)
{
    return INF_SUCCESS;
}

const XOperator XEvaluator::m_sOperators[] =
{
    /*     Id        Pri    Associativity          cParams  Name   pfn    ShortHelp      LongHelp */
    /* Special Operators*/
    XOperator(XANK_OPEN_PARANTHESIS_OPERATOR_ID,
                      99, enmOperatorDirNone,        0,      "(",   NULL,  "(<expr>",        "Begin subexpresion or function."),
    XOperator(XANK_CLOSE_PARANTHESIS_OPERATOR_ID,
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
    return NULL;
}


XAtom *XEvaluator::ParseNumber(const char *pcszExpr, const char **ppcszEnd, const XAtom *pcPreviousAtom)
{
    std::string sNum;
    const char *pcszStart = pcszExpr;
    int iRadix = 0;
    uint64_t i = 0;

    /*
     * Binary prefix.
     */
    if (*pcszExpr == 'b' || *pcszExpr == 'B')
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
    else if (*pcszExpr == '0')
    {
        /*
         * Octal prefix.
         */
        ++pcszExpr;
        while (*pcszExpr)
        {
            if (*pcszExpr >= '0' && *pcszExpr < '8')
                sNum[i++] = *ppcszExprszExpr;
            else if (!isspace(*pcszExpr))
                break;

            pcszExpr++;
        }
        iRadix = 8;

        /*
         * Hexadecimal prefix.
         */
        if (   i == 0
            && (*pcszExpr == 'x' || *pcszExpr == 'X'))
        {
            ++pcszExpr;
            while (*pcszExpr)
            {
                if (   isdigit(*pcszExpr)
                    || (*pcszExpr >= 'a' && *pcszExpr <= 'f')
                    || (*pcszExpr >= 'A' && *pcszExpr <= 'F'))
                {
                    sNum[i++] = *pcszExpr;
                }
                else if (!isspace(*pcszExpr))
                    break;

                pcszExpr++;
            }
            iRadix = 16;
        }
    }

    /*
     * If nothing has been accumulated in our 'szNum' array, we've not
     * got any recognized numeric under whatever radices we've checked.
     * Reset pointer to the original & try parsing without any prefix.
     */
    if (i == 0)
    {
        pcszExpr = pcszStart;
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

    /*
     * We've got a number. Terminate our string buffer, and convert it.
     */
    szNum[i] = '\0';
    char *pszEndTmp = NULL;
    FLOAT dValue = 0;
    errno = 0;
    if (iRadix != 10)
        dValue = (FLOAT)strtoull(szNum, &pszEndTmp, iRadix);
    else
        dValue = (FLOAT)strtold(szNum, &pszEndTmp);

    /*
     * An error while converting the number.
     */
    if (errno)
    {
        DEBUGPRINTF(("Error while string to unsigned conversion of %s\n", szNum));
        return NULL;
    }

    PATOM pAtom = MemAlloc(sizeof(ATOM));
    if (!pAtom)
        return NULL;
    pAtom->Type = enmAtomNumber;
    pAtom->u.dValue = dValue;
    *ppszEnd = pszExpr;
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

                /* e.g: "(-4"  and "(expr)-4" */
                if (   pcPreviousAtom->Operator()
                    && pcPreviousAtom->Operator()->IsOpenParanthesis())
                    continue;
            }

            /** @todo ugh, ownership semantics are screwed. rethink this shit.  */
#if 0
            XAtom *pAtom = new XAtom();
            if (!pAtom)
                return NULL;
            pAtom->SetOperator(
            pAtom->Type = enmAtomOperator;
            pAtom->u.pOperator = &g_aOperators[i];
            pszExpr += cbOperator;
            *ppszEnd = pszExpr;
            return pAtom;
#endif
        }
    }
    return NULL;
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
