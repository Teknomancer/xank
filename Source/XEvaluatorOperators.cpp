/** @file
 * xank - Evaluator operators, implementation.
 */

/*
 * Copyright (C) 2011-2012 Ramshankar (aka Teknomancer)
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
#include "Debug.h"
#include "Assert.h"

/*
 * Smallest to largest type. Types that are higher
 * in this list will be promoted to those that are lower when
 * operations involving multiple types are encountered, e.g. Integer
 * gets promoted to Float.
 */
typedef enum
{
    enmUnknown,
    enmInteger,
    enmFloat,
    enmMax = enmFloat
} NumberType;


static NumberType FindLargestNumberType(XAtom *apAtoms[], size_t cAtoms)
{
    NumberType dstType = enmUnknown;
    for (size_t i = 0; i < cAtoms; i++)
    {
        NumberType srcType;
        Assert(apAtoms[i]->IsNumber());
        if (apAtoms[i]->IsInteger())
            srcType = enmInteger;
        else if (apAtoms[i]->IsFloat())
            srcType = enmFloat;

        if (srcType >= dstType)
            dstType = srcType;
        if (dstType == enmMax)
            break;
    }
    return dstType;
}


int OpAdd(XAtom *apAtoms[], size_t cAtoms, void *pvData)
{
    NOREF(pvData);
    DEBUGPRINTF(("OpAdd\n"));

    NumberType dstType = FindLargestNumberType(apAtoms, cAtoms);
    int rc = INF_SUCCESS;
    if (dstType == enmInteger)
    {
        mpz_t Operand1;
        mpz_t Operand2;
        mpz_init(Operand1);
        mpz_init(Operand2);
        int rc1 = apAtoms[0]->GetInteger(Operand1);
        int rc2 = apAtoms[1]->GetInteger(Operand2);
        if (IS_SUCCESS(rc1) && IS_SUCCESS(rc2))
        {
            mpz_t Result;
            mpz_init(Result);
            mpz_add(Result, Operand1, Operand2);
        }
        else
        {
            DEBUGPRINTF(("OpAdd failed dstType=%d rc1=%d rc2=%d\n", dstType, rc1, rc2));
            rc = ERR_INVALID_ATOM_TYPE_FOR_OPERATION;
        }
    }
    else if (dstType == enmFloat)
    {
        mpf_t Operand1;
        mpf_t Operand2;
        mpf_init(Operand1);
        mpf_init(Operand2);
        int rc1 = apAtoms[0]->PromoteGetFloat(Operand1);
        int rc2 = apAtoms[1]->PromoteGetFloat(Operand2);
        if (IS_SUCCESS(rc1) && IS_SUCCESS(rc2))
        {
            mpf_t Result;
            mpf_init(Result);
            mpf_add(Result, Operand1, Operand2);
            apAtoms[0]->SetFloat(Result);
        }
        else
        {
            DEBUGPRINTF(("OpAdd failed dstType=%d rc1=%d rc2=%d\n", dstType, rc1, rc2));
            rc = ERR_INVALID_ATOM_TYPE_FOR_OPERATION;
        }
    }
    else
    {
        DEBUGPRINTF(("OpAdd failed dstType=%d\n", dstType));
        rc = ERR_INVALID_ATOM_TYPE_FOR_OPERATION;
    }

    return rc;
}

const XOperator XEvaluator::m_sOperators[] =
{
    /*     Id        Pri    Associativity          cParams  Name   pfn    ShortHelp             LongHelp */
    /* Special Operators */
    XOperator(XANK_OPEN_PARENTHESIS_OPERATOR_ID,
                      99, enmOperatorDirNone,        0,      "(",   NULL,  "(<expr>",          "Begin expression or function."),
    XOperator(XANK_CLOSE_PARENTHESIS_OPERATOR_ID,
                      99,  enmOperatorDirNone,       0,      ")",   NULL,  "<expr>)",          "End expression or function."),
    XOperator(XANK_PARAM_SEPARATOR_OPERATOR_ID,
                       0,  enmOperatorDirLeft,       2,      ",",   NULL,  "<expr>, <expr>",   "Function parameter separator."),
    XOperator(XANK_ASSIGNMENT_OPERATOR_ID,
                       0,  enmOperatorDirLeft,       2,      "=",   NULL,  "<lval>=<rval>",    "Assignment operator."),

    /* Generic Operators */
    XOperator(10,     70,  enmOperatorDirLeft,       2,      "+",  OpAdd, "<expr1> + <expr2>", "Addition operator.")
};

const size_t XEvaluator::m_cOperators = XANK_ARRAY_ELEMENTS(m_sOperators);
