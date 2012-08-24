/** @file
 * xank - Expression Evaluator, implementation.
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
#include "Assert.h"
#include "XEvaluatorDefs.h"
#include "XAtom.h"
#include "XFunction.h"
#include "XGenericDefs.h"
#include "XOperator.h"
#include "XErrors.h"
#include "ConsoleIO.h"
#include "Debug.h"

#include <cstring>
#include <cstdarg>
#include <stdint.h>
#include <gmp.h>

#include "XEvaluatorFunctions.cpp.h"

XEvaluator::XEvaluator()
{
    /** @todo add parameters to accept settings */
    m_fInitialized             = false;
    m_Error                    = ERR_NOT_INITIALIZED;
    m_sError                   = "Evaluator not initialized.";
    m_pOpenParenthesisOperator = NULL;
}


XEvaluator::~XEvaluator()
{
    while (!m_RPNQueue.empty())
    {
        XAtom *pAtom = m_RPNQueue.front();
        m_RPNQueue.pop();
        Assert(pAtom);
        delete pAtom;
        pAtom = NULL;
    }
}


void XEvaluator::CleanUp(std::stack<XAtom*> *pStack, std::queue<XAtom*> *pQueue, int rc, const char *pcszMsg, ...)
{
    if (pStack)
        while (!pStack->empty())
        {
            XAtom *pAtom = pStack->top();
            pStack->pop();
            Assert(pAtom);
            delete pAtom;
            pAtom = NULL;
        }

    if (pQueue)
        while (!pQueue->empty())
        {
            XAtom *pAtom = pQueue->front();
            pQueue->pop();
            Assert(pAtom);
            delete pAtom;
            pAtom = NULL;
        }

    va_list FmtArgs;
    char szBuf[2048];

    va_start(FmtArgs, pcszMsg);
#ifdef XANK_OS_WINDOWS
    vsnprintf_s(szBuf, sizeof(szBuf), pcszMsg, FmtArgs);
#else
    vsnprintf(szBuf, sizeof(szBuf) - 1, pcszMsg, FmtArgs);
#endif
    va_end(FmtArgs);

    m_sError = szBuf;
    m_Error = rc;
}


static int OperatorCompare(const void *pcvOperator1, const void *pcvOperator2)
{
    const XOperator *pcOperator1 = reinterpret_cast<const XOperator*>(pcvOperator1);
    const XOperator *pcOperator2 = reinterpret_cast<const XOperator*>(pcvOperator2);
    const char *pcszOperator1    = pcOperator1->Name().c_str();
    const char *pcszOperator2    = pcOperator2->Name().c_str();
    const size_t cbOperator1     = pcOperator1->Name().length();
    const size_t cbOperator2     = pcOperator2->Name().length();

    int rc = std::strncmp(pcszOperator1, pcszOperator2, XANK_MAX(cbOperator1, cbOperator2));
    if (!rc)
        return pcOperator2->Params() - pcOperator1->Params();
    return -rc;
}


int XEvaluator::Init()
{
    int rc = ERR_UNDEFINED;
    for (size_t i = 0; i < m_cOperators; i++)
    {
        const XOperator *pcOperator = &m_sOperators[i];
        if (   pcOperator->Name().empty()
            || pcOperator->ShortDesc().empty()
            || pcOperator->LongDesc().empty())
        {
            rc = ERR_INVALID_OPERATOR;
            CleanUp(NULL, NULL, rc,
                "Operator with missing name, syntax or description. Index=%" FMT_SZT " Operator %s",
                i, pcOperator->PrintToString().c_str());
            return rc;
        }

        if (isdigit(*pcOperator->Name().c_str()) || pcOperator->Name() == ".")
        {
            rc = ERR_INVALID_OPERATOR;
            CleanUp(NULL, NULL, rc,
                "Invalid operator name. Index=%" FMT_U64 " Operator %s", i, pcOperator->PrintToString().c_str());
            return rc;
        }

        /*
         * Let us for now only allow Operators taking 2 or lower parameters.
         * The evaluation logic can of course handle any number of parameters
         * but we don't have a parameter separator for Operators unlike Functors.
         */
        if (pcOperator->Params() > 2)
        {
            rc = ERR_INVALID_OPERATOR;
            CleanUp(NULL, NULL, rc,
                "Too many parameters. Index=%" FMT_SZT " Operator %s", i, pcOperator->PrintToString().c_str());
            return rc;
        }

        for (size_t k = 0; k < m_cOperators; k++)
        {
            if (i == k)
                continue;

            const XOperator *pcCur = &m_sOperators[k];

            /*
             * Make sure each operator Id is unique.
             */
            if (pcOperator->Id() == pcCur->Id())
            {
                rc = ERR_CONFLICTING_OPERATORS;
                CleanUp(NULL, NULL, rc,
                        "Duplicate operator Id=%" FMT_U32 " %s at [%" FMT_SZT "] and %s at [%" FMT_SZT "]",
                        pcOperator->Id(), pcOperator->PrintToString().c_str(), i, pcCur->PrintToString().c_str(), k);
                return rc;
            }

            if (   pcOperator->Name() == pcCur->Name()
                && pcOperator->Dir() == pcCur->Dir())
            {
                if (pcOperator->Params() == pcCur->Params())
                {
                    rc = ERR_DUPLICATE_OPERATOR;
                    CleanUp(NULL, NULL, rc,
                        "Duplicate operator %s at [%" FMT_SZT "] and [%" FMT_SZT "]", pcOperator->PrintToString().c_str(),
                            i, k);
                    return rc;
                }

                rc = ERR_CONFLICTING_OPERATORS;
                CleanUp(NULL, NULL, rc, "Conflicting operator %s at [%" FMT_SZT "] and [%" FMT_SZT "]",
                        pcOperator->PrintToString().c_str(), i, k);
                return rc;
            }
        }
    }

    /*
     * Sort operator list to workaround overlapping operator names, e.g.: "++" always appears before "+".
     * While sorting operators with the same name, the one with more parameters is sorted first.
     * For e.g. binary '-' is placed before unary '-', See ParseOperator().
     */
    std::qsort((void*)m_sOperators, m_cOperators, sizeof(XOperator), OperatorCompare);

    const XOperator *pCloseParenthesisOperator = NULL;
    const XOperator *pParamSeparatorOperator   = NULL;
    for (size_t i = 0; i < m_cOperators; i++)
    {
        DEBUGPRINTF(("%s cParams=%" FMT_U8 " Id=%" FMT_U32 "\n", m_sOperators[i].Name().c_str(),
                m_sOperators[i].Params(), m_sOperators[i].Id()));
        if (m_sOperators[i].IsOpenParenthesis())
        {
            if (m_pOpenParenthesisOperator == NULL)
                m_pOpenParenthesisOperator = &m_sOperators[i];
            else
            {
                rc = ERR_DUPLICATE_OPERATOR;
                CleanUp(NULL, NULL, rc, "Invalid duplicate operator: Open parenthesis\n");
                return rc;
            }
        }
        else if (m_sOperators[i].IsCloseParenthesis())
        {
            if (pCloseParenthesisOperator == NULL)
                pCloseParenthesisOperator = &m_sOperators[i];
            else
            {
                rc = ERR_DUPLICATE_OPERATOR;
                CleanUp(NULL, NULL, rc, "Invalid duplicate operator: Close parenthesis\n");
                return rc;
            }
        }
        else if (m_sOperators[i].IsParamSeparator())
        {
            if (pParamSeparatorOperator == NULL)
                pParamSeparatorOperator = &m_sOperators[i];
            else
            {
                rc = ERR_DUPLICATE_OPERATOR;
                CleanUp(NULL, NULL, rc, "Invalid duplicate operator: Parameter sepatator.\n");
                return rc;
            }
        }
    }

    if (   !m_pOpenParenthesisOperator
        || !pParamSeparatorOperator
        || !pCloseParenthesisOperator)
    {
        rc = ERR_MISSING_BASIC_OPERATOR;
        CleanUp(NULL, NULL, rc, "Basic operator missing.\n");
        return rc;
    }

    m_fInitialized = true;
    return INF_SUCCESS;
}


int XEvaluator::Parse(const char *pcszExpr)
{
    NOREF(pcszExpr);
    if (!m_fInitialized)
        return ERR_NOT_INITIALIZED;

    std::queue<XAtom*> Queue;
    std::stack<XAtom*> Stack;
    const char *pcszEnd  = NULL;
    XAtom *pAtom         = NULL;
    XAtom *pPreviousAtom = NULL;
    int rc               = ERR_UNDEFINED;
    while ((pAtom = ParseAtom(pcszExpr, &pcszEnd, pPreviousAtom)) != NULL)
    {
        if (   pPreviousAtom
            && pPreviousAtom->Operator()
            && pPreviousAtom->Operator()->IsCloseParenthesis())
        {
            delete pPreviousAtom;
            pPreviousAtom = NULL;
        }

        if (pAtom->IsNumber())
        {
            DEBUGPRINTF(("Queue push number %s\n", pAtom->PrintToString().c_str()));
            Queue.push(pAtom);
        }
        else if (pAtom->Function())
        {
            DEBUGPRINTF(("Queue push function %s\n", pAtom->Function()->PrintToString().c_str()));
            Stack.push(pAtom);
        }
        else if (pAtom->IsVariable())
        {
            /** @todo variable atom */
            rc = ERR_NOT_SUPPORTED;
            AssertRelease(rc == ERR_NOT_SUPPORTED);
        }
        else if (pAtom->IsOperator())
        {
            DEBUGPRINTF(("Parsing operator %s\n", pAtom->Operator()->PrintToString().c_str()));
            const XOperator *pcOperator = pAtom->Operator();
            Assert(pcOperator);
            if (pcOperator->IsOpenParenthesis())
            {
                DEBUGPRINTF(("Stack push parenthesis begin '%s'.\n", pcOperator->Name().c_str()));
                Stack.push(pAtom);
            }
            else if (pcOperator->IsCloseParenthesis())
            {
                DEBUGPRINTF(("Parenthesis end '%s'.\n", pcOperator->Name().c_str()));
                XAtom *pStackAtom = NULL;
                while (!Stack.empty())
                {
                    pStackAtom = Stack.top();
                    Assert(pStackAtom);
                    if (   pStackAtom->Operator()
                        && pStackAtom->Operator()->IsOpenParenthesis())
                        break;
                    /** @todo implement XAtom::PrintToString() */
                    DEBUGPRINTF(("Popping '%s' to queue.\n", pStackAtom->PrintToString().c_str()));
                    Stack.pop();
                    Queue.push(pStackAtom);
                }

                if (!pStackAtom)
                {
                    DEBUGPRINTF(("Missing open parenthesis.\n"));
                    delete pAtom;
                    pAtom = NULL;
                    rc = ERR_UNBALANCED_PARENTHESIS;
                    CleanUp(&Stack, &Queue, rc, "Missing open parenthesis.");
                    return rc;
                }

                if (   pStackAtom->Operator()
                    && pStackAtom->Operator()->IsOpenParenthesis())
                {
                    Stack.pop();
                    delete pStackAtom;
                    pStackAtom = NULL;
                }

                /*
                 * If the left parenthesis is preceeded by a function, pop it to the Queue
                 * incrementing number of parameters the function already has.
                 */
                if (   !Stack.empty()
                    && (pStackAtom = Stack.top()) != NULL
                    && (pStackAtom->Function()))
                {
                    DEBUGPRINTF(("Popping Function '%s' to queue.\n", pStackAtom->Function()->Name().c_str()));
                    Stack.pop();
                    Queue.push(pStackAtom);

                    pStackAtom->IncrementFunctionParams();
                    if (pStackAtom->FunctionParams() > pStackAtom->Function()->MaxParams())
                    {
                        DEBUGPRINTF(("Too many params to Function '%s'.\n", pStackAtom->Function()->Name().c_str()));
                        delete pAtom;
                        pAtom = NULL;
                        rc = ERR_TOO_MANY_PARAMETERS;
                        CleanUp(&Stack, &Queue, rc,
                            "Too many parameters to function %s", pStackAtom->Function()->PrintToString().c_str());
                        return rc;
                    }
                    else if (pStackAtom->FunctionParams() < pStackAtom->Function()->MinParams())
                    {
                        DEBUGPRINTF(("Too few params to Function '%s'.\n", pStackAtom->Function()->Name().c_str()));
                        delete pAtom;
                        pAtom = NULL;
                        rc = ERR_TOO_FEW_PARAMETERS;
                        CleanUp(&Stack, &Queue, rc,
                            "Too few parameters to function %s", pStackAtom->Function()->PrintToString().c_str());
                        return rc;
                    }

                    DEBUGPRINTF(("Function %s cParams=%" FMT_U64 ".\n", pStackAtom->Function()->Name().c_str(),
                            pStackAtom->FunctionParams()));
                }
            }
            else if (pcOperator->IsParamSeparator())
            {
                DEBUGPRINTF(("Function param separator.\n"));
                XAtom *pStackAtom = NULL;
                while (   !Stack.empty()
                       && (pStackAtom = Stack.top()) != NULL)
                {
                    if (   pStackAtom->Operator()
                        && pStackAtom->Operator()->IsOpenParenthesis())
                    {
                        break;
                    }

                    Stack.pop();
                    Queue.push(pStackAtom);
                }

                if (  !pStackAtom
                    || (   pStackAtom->Operator()
                        && pStackAtom->Operator()->IsOpenParenthesis()))
                {
                    DEBUGPRINTF(("Operator '%s' param mismatch.\n", pcOperator->Name().c_str()));
                    delete pAtom;
                    pAtom = NULL;
                    rc = ERR_UNEXPECTED_PARENTHESIS_SEPARATOR;
                    CleanUp(&Stack, &Queue, rc, "Operator %s parameter mismatch.\n", pcOperator->PrintToString().c_str());
                    return rc;
                }

                Stack.pop();
                XAtom *pOpenParenAtom = pStackAtom;
                pStackAtom            = NULL;
                XAtom *pFunctionAtom  = NULL;
                if (!Stack.empty())
                {
                    pFunctionAtom = Stack.top();
                    Stack.pop();
                }

                if (   pFunctionAtom
                    && pFunctionAtom->Function())
                {
                    pFunctionAtom->IncrementFunctionParams();
                    if (pFunctionAtom->FunctionParams() >= pFunctionAtom->Function()->MaxParams())
                    {
                        DEBUGPRINTF(("Too many params to Function '%s' max=%" FMT_U64 ".\n", pFunctionAtom->Function()->Name().c_str(),
                                pFunctionAtom->Function()->MaxParams()));
                        delete pAtom;
                        pAtom = NULL;
                        rc = ERR_TOO_FEW_PARAMETERS;
                        CleanUp(&Stack, &Queue, rc, "Too many parameters to Function %s", pFunctionAtom->Function()->PrintToString().c_str());
                        return rc;
                    }

                    /*
                     * Now that we've recorded the information into the Function Atom, restore the
                     * stack items as though nothing happened :)
                     */
                    Stack.push(pFunctionAtom);
                    Stack.push(pOpenParenAtom);

                    DEBUGPRINTF(("Function '%s' cParams=%" FMT_U64 ".\n", pFunctionAtom->Function()->Name().c_str(),
                            pFunctionAtom->FunctionParams()));
                }
                else
                {
                    DEBUGPRINTF(("No function specified.\n"));
                    delete pAtom;
                    pAtom = NULL;
                    rc = ERR_UNEXPECTED_PARENTHESIS_SEPARATOR;
                    CleanUp(&Stack, &Queue, rc, "No function specified.\n");
                    return rc;
                }
            }
            else if (pcOperator->IsAssignment())
            {
                /** @todo variable assignment */
                rc = ERR_NOT_SUPPORTED;
                AssertRelease(rc == ERR_NOT_SUPPORTED);
            }
            else
            {
                /*
                 * Regular operator, handle preceedence.
                 */
                XAtom *pStackAtom = NULL;
                while (   !Stack.empty()
                        && (pStackAtom = Stack.top()) != NULL)
                {
                    const XOperator *pcStackOperator = pStackAtom->Operator();
                    if (  !pcStackOperator
                        || pcStackOperator->IsOpenParenthesis())
                    {
                        break;
                    }

                    if (   (pcOperator->Dir() == enmOperatorDirLeft  && pcOperator->Priority() <= pcStackOperator->Priority())
                        || (pcOperator->Dir() == enmOperatorDirRight && pcOperator->Priority() < pcStackOperator->Priority()))
                    {
                        DEBUGPRINTF(("Moving operator '%s' cParams=%" FMT_U8 " from stack to queue.\n",
                                pcStackOperator->Name().c_str(), pcStackOperator->Params()));
                        Stack.pop();
                        m_RPNQueue.push(pStackAtom);
                    }
                    else
                        break;
                }

                DEBUGPRINTF(("Pushing operator '%s' (id=%" FMT_U32 ") cParams=%" FMT_U8 " to stack.\n",
                        pcOperator->Name().c_str(), pcOperator->Id(), pcOperator->Params()));
                Stack.push(pAtom);
            }
        }
        else
        {
            DEBUGPRINTF(("Unknown token.\n"));
            delete pAtom;
            pAtom = NULL;
            break;
        }
        pcszExpr = pcszEnd;
        pPreviousAtom = pAtom;
    }

    /*
     * If there are any Atoms left in the stack we must pop them into the queue.
     * However, an open parenthesis on top of the stack means we have unbalanced parenthesis.
     */
    pAtom = NULL;
    if (   !Stack.empty()
        && (pAtom = Stack.top()) != NULL
        && pAtom->Operator()
        && pAtom->Operator()->IsOpenParenthesis())
    {
        rc = ERR_UNBALANCED_PARENTHESIS;
        Stack.pop();
        delete pAtom;
        pAtom = NULL;
        CleanUp(&Stack, &Queue, rc, "Unbalanced parenthesis.\n");
        return rc;
    }

    /*
     * Pop the remaining Operators, Functions to the queue.
     */
    while (   !Stack.empty()
           && (pAtom = Stack.top()) != NULL)
    {
        Stack.pop();
        Queue.push(pAtom);
    }

    if (Queue.empty())
    {
        rc = ERR_INVALID_EXPRESSION;
        CleanUp(&Stack, &Queue, rc, "No atoms detected.\n");
        return rc;
    }

    /*
     * Clear old queue if any, and restore the new one.
     */
    while (  !m_RPNQueue.empty()
           && (pAtom = m_RPNQueue.front()) != NULL)
    {
        m_RPNQueue.pop();
        delete pAtom;
        pAtom = NULL;
    }

    m_RPNQueue = Queue;
    CleanUp(NULL, NULL, INF_SUCCESS, "Expression parsed successfully.");
    return INF_SUCCESS;
}


int XEvaluator::Evaluate()
{
    if (!m_fInitialized)
        return ERR_NOT_INITIALIZED;

    if (m_RPNQueue.empty())
        return ERR_UNPARSED_EXPRESSION;

    std::stack<XAtom *> Stack;
    XAtom *pAtom = NULL;
    int rc       = ERR_NOT_INITIALIZED;

    while ((pAtom = m_RPNQueue.front()) != NULL)
    {
        m_RPNQueue.pop();

        if (pAtom->IsNumber())
        {
            DEBUGPRINTF(("Pushing %s to stack.\n", pAtom->PrintToString().c_str()));
            Stack.push(pAtom);
        }
        else if (pAtom->Operator())
        {
            const XOperator *pcOperator = pAtom->Operator();
            DEBUGPRINTF(("Operator %s\n", pcOperator->Name().c_str()));
            if (Stack.size() < pcOperator->Params())
            {
                DEBUGPRINTF(("Stack size=%" FMT_SZT " cParams=" FMT_U8 ".\n", Stack.size(), pcOperator->Params()));
                rc = ERR_TOO_FEW_PARAMETERS;
                CleanUp(&Stack, &m_RPNQueue, rc,
                        "Insufficient parameters to operator %s cParams=%" FMT_U8 "\n", pcOperator->Name().c_str(),
                        pcOperator->Params());
                return rc;
            }

            /*
             * Construct the array of parameter Atoms and pass it to the Operator evaluator if any, otherwise just
             * push the first parameter as the result.
             */
            XAtom *apAtoms[XANK_MAX_OPERATOR_PARAMETERS];
            Assert(pcOperator->Params() < XANK_MAX_OPERATOR_PARAMETERS);
            uint8_t cParams = pcOperator->Params();
            while (--cParams)
            {
                apAtoms[cParams] = Stack.top();   /* We've already checked Stack.size() above, so this is fine. */
                Stack.pop();

                /** @todo handle type-cast stuff here, i.e. operator specifies what range it
                 *        can take and we fiddle with the Atom to figure out if it's valid or
                 *        not. */
            }

            XAtom *pResultAtom = NULL;
            if (pcOperator->Function())
            {
                rc = pcOperator->Invoke(apAtoms, pcOperator->Params(), NULL /* pvData */);
                if (IS_SUCCESS(rc))
                    pResultAtom = apAtoms[0];
            }
            else
            {
                rc = INF_SUCCESS;
                pResultAtom = apAtoms[0];
            }

            if (pResultAtom)
            {
                Assert(IS_SUCCESS(rc));
                for (uint8_t i = 1 /* not zero, duh! */; i < pcOperator->Params(); i++)
                {
                    delete apAtoms[i];
                    apAtoms[i] = NULL;
                }
                Stack.push(apAtoms[0]);
            }
            else
            {
                Assert(IS_FAILURE(rc));
                DEBUGPRINTF(("Operator %s failed on given operands. rc=%d\n", pcOperator->Name().c_str(), rc));
                CleanUp(&Stack, &m_RPNQueue, rc,
                        "Operator %s failed on given operands.", pcOperator->Name().c_str());
                return rc;
            }
        }
        else if (pAtom->Function())
        {
            const XFunction *pcFunction = pAtom->Function();
            DEBUGPRINTF(("%s ", pcFunction->Name().c_str()));

            if (Stack.size() < pAtom->FunctionParams())
            {
                DEBUGPRINTF(("Stack size=%" FMT_SZT " cParams=%" FMT_U8 "\n", Stack.size(), pAtom->FunctionParams()));
                rc = ERR_TOO_FEW_PARAMETERS;
                CleanUp(&Stack, &m_RPNQueue, rc,
                        "Insufficient parameters to function %s cParams=%" FMT_U8 "\n", pcFunction->Name().c_str(),
                        pAtom->FunctionParams());
                return rc;
            }

            /*
             * Construct an array of maximum possible XAtoms parameters and pass it to the function
             * actuator if any, otherwise just push the first paramater as the result.
             */
            AssertCompile(XANK_MAX_FUNCTION_PARAMETERS == SIZE_MAX);
            Assert(pcFunction->MaxParams() <= XANK_MAX_FUNCTION_PARAMETERS);

            const size_t cParams = XANK_MIN(pAtom->FunctionParams(), XANK_MAX_FUNCTION_PARAMETERS);
            XAtom **ppaAtoms     = new(std::nothrow) XAtom *[cParams];
            if (!ppaAtoms)
            {
                rc = ERR_NO_MEMORY;
                CleanUp(&Stack, &m_RPNQueue, rc,
                        "No memory to allocate %" FMT_SZT " Atoms.\n", cParams);
                return rc;
            }

            rc = ERR_UNDEFINED;
            size_t cParamsTmp = cParams;
            while (cParamsTmp--)
            {
                ppaAtoms[cParamsTmp] = Stack.top();
                Stack.pop();

                /** @todo check if the Function can cast to the required to perform its
                 *        operation. If not, we cannot proceed as it would invoke undefined
                 *        behaviour. */
            }

            XAtom *pResultAtom = NULL;
            if (pcFunction->Function())
            {
                rc = pcFunction->Invoke(ppaAtoms, cParams);
                if (IS_SUCCESS(rc))
                    pResultAtom = ppaAtoms[0];
            }
            else
            {
                pResultAtom = ppaAtoms[0];
                AssertReturn(pResultAtom, ERR_GENERAL_FAILURE);
                rc = INF_SUCCESS;
            }

            /* Cleanup temporaries and stack. */
            for (size_t i = 1; i < cParams; i++)
            {
                delete ppaAtoms[i];
                ppaAtoms[i] = NULL;
            }
            delete[] ppaAtoms;
            ppaAtoms = NULL;

            if (IS_SUCCESS(rc))
            {
                Assert(pResultAtom);
                Stack.push(pResultAtom);
            }
            else
            {
                DEBUGPRINTF(("Function %s failed with given operands. rc=%d\n", pcFunction->Name().c_str(), rc));
                CleanUp(&Stack, &m_RPNQueue, rc,
                        "Function %s failed with given operands.\n", pcFunction->Name().c_str());
                return rc;
            }
        }
        else if (pAtom->Variable())
        {
            /** @todo variable evaluation. */
        }
        else
        {
            DEBUGPRINTF(("Wow, an undiscovered Atom!\n"));
            delete pAtom;
            pAtom = NULL;
        }
    }

    /*
     * Result is on the stack, otherwise it's an error.
     */
    if (Stack.size() == 1)
    {
        pAtom = Stack.top();
        Stack.pop();
        DEBUGPRINTF(("Result is %s\n", pAtom->PrintToString().c_str()));
        rc = INF_SUCCESS;
        CleanUp(NULL, NULL, rc,
                "Expression evaluated successfully.\n");

        /** @todo do something with the result atom! Copy it to a string? or cast and
         *        send it to the caller? Hmm think about this. */

        delete pAtom;
        pAtom = NULL;
        return rc;
    }

    rc = ERR_INVALID_EXPRESSION;
    CleanUp(&Stack, &m_RPNQueue, rc,
            "Excess atoms, invalid expression.\n");
    return rc;
}

XAtom *XEvaluator::ParseAtom(const char *pcszExpr, const char **ppcszEnd, const XAtom *pcPreviousAtom)
{
    DEBUGPRINTF(("ParseAtom \"%s\"\n", pcszExpr));
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
    NOREF(pcPreviousAtom);
    static uint64_t cFunctions = XANK_ARRAY_ELEMENTS(m_sFunctions);
    for (uint64_t i = 0; i < cFunctions; i++)
    {
        size_t cbFunction = m_sFunctions[i].Name().length();
        if (!std::strncmp(m_sFunctions[i].Name().c_str(), pcszExpr, cbFunction))
        {
            pcszExpr += cbFunction;
            while (isspace(*pcszExpr))
                pcszExpr++;

            if (!std::strncmp(pcszExpr, m_pOpenParenthesisOperator->Name().c_str(),
                            m_pOpenParenthesisOperator->Name().length()))
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
    NOREF(pcPreviousAtom);
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
    for (size_t i = 0; i < m_cOperators; i++)
    {
        size_t cbOperator = m_sOperators[i].Name().length();
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
    /** @todo Parse variables. */
    NOREF(pcszExpr); NOREF(ppcszEnd); NOREF(pcPreviousAtom);
    return NULL;
}


XAtom *XEvaluator::ParseCommand(const char *pcszExpr,  const char **ppcszEnd,  const XAtom *pcPreviousAtom)
{
    /** @todo Parse commands. */
    NOREF(pcszExpr); NOREF(ppcszEnd); NOREF(pcPreviousAtom);
    return NULL;
}
