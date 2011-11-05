/** @file
 * xank - Evaluator operators, implementation.
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
#include "XAtom.h"
#include "XErrors.h"
#include "XGenericDefs.h"
#include "XOperator.h"

#if 0
/*
 * Smallest to largest type. Types that are higher
 * in this list will be promoted to those that are lower when
 * operations involving multiple types are encountered, e.g. Integer
 * gets promoted to Float.
 */
typedef enum NumberType
{
    enmUnknown,
    enmInteger,
    enmFloat,
};

static NumberType FindLargestNumberType(XAtom *apAtoms[], size_t cAtoms)
{
    NumberType dstType = enmUnknown;
    for (size_t i = 0; i < cAtoms; i++)
    {
        NumberType srcType;
        if (apAtoms[i]->IsInteger())
            srcType = enmInteger;
        else if (apAtoms[i]->IsFloat())
            srcType = enmFloat;

        if (srcType > dstType)
            dstType = srcType;
    }
    return dstType;
}


int OpAdd (XAtom *apAtoms[], size_t cAtoms)
{
    NumberType dstType = FindLargestNumberType(apAtoms, cAtoms);
    if (dstType == enmInteger)
    {
        mpz_t Operand1;
        mpz_t Operand2;
        int rc1 = apAtoms[0]->GetInteger(Operand1)
        int rc2 = apAtoms[1]->GetInteger(Operand2);
        if (IS_SUCCESS(rc1) && IS_SUCCESS(rc2))
        {
            mpz_t Result;
            gmp_mpz_add(Result, Operand1, Operand2);

        }
    }
    else if (apAtoms[0]->IsFloat())
    {
    }
    return INF_SUCCESS;
}
#endif

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

const size_t XEvaluator::m_cOperators = XANK_ARRAY_ELEMENTS(m_sOperators);
