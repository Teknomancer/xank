/** @file
 * xank - Expression Evaluator, header.
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

#ifndef XANK_EVALUATOR_H
# define XANK_EVALUATOR_H

#include <queue>
#include <list>
#include <string>
#include <stack>

#include "Settings.h"

class XAtom;
class XFunction;
class XOperator;

/**
 * Expression evaluator.
 * The evaluator class parses, stores and evaluates expressions.
 */
class XEvaluator
{
    public:
        XEvaluator();
        virtual ~XEvaluator();

        /**
         * Initializes the expression object. This must be invoked before any other
         * methods of this object, otherwise all of them will return
         * ERR_NOT_INITIALIZED error.
         *
         * @return int: xank error code.
         */
        int                         Init();

        /**
         * Parses an expression into an internal representation.
         *
         * @param pcszExr           The expression to parse.
         *
         * @return int: xank error code.
         */
        int                         Parse(const char *pcszExr);

        /**
         * Evaluates the internal representation of the previously parsed expression.
         * The logic is roughly reverse polish notation but modified to support
         * variables, variable arguments to functions etc.
         *
         * @return int: xank error code.
         */
        int                         Evaluate();

    private:
        /**
         * Parses the expression for an Atom.
         *
         * @param pcszExpr          The expression to parse.
         * @param ppcszEnd          Where to store till-what-point @a pcszExpr was
         *                          parsed.
         * @param pcPreviousAtom    Pointer to the previously parsed Atom, must be NULL
         *                          on first call of an expression.
         *
         * @return Atom*: A newly allocated Atom or NULL if no atoms were parsed.
         */
        XAtom                      *ParseAtom(const char *pcszExpr, const char **ppcszEnd, const XAtom *pcPreviousAtom);

        /**
         * Parses the expression for a function.
         *
         * @param pcszexpr          The expression to parse.
         * @param ppcszEnd          Where to store till-what-point @a pcszExpr was
         *                          parsed.
         * @param pcPreviousAtom    Pointer to the previously parsed Atom, must be NULL
         *                          on the first call of an expression.
         *
         * @return Atom*: A newly allocated Atom or NULL if no functions were parsed.
         */
        XAtom                      *ParseFunction(const char *pcszexpr, const char **ppcszEnd, const XAtom *pcPreviousAtom);

        /**
         * Parses the expression for a number.
         *
         * @param pcszexpr          The expression to parse.
         * @param ppcszEnd          Where to store till-what-point @a pcszExpr was
         *                          parsed.
         * @param pcPreviousAtom    Pointer to the previously parsed Atom, must be NULL
         *                          on the first call of an expression.
         *
         * @return Atom*: A newly allocated Atom or NULL if no numbers were parsed.
         */
        XAtom                      *ParseNumber(const char *pcszExpr, const char **ppcszEnd, const XAtom *cpPreviousAtom);

        /**
         * Parses the expression for an operator.
         *
         * @param pcszExpr          The expression to parse.
         * @param ppcszEnd          Where to store till-what-point @a pcszExpr was
         *                          parsed.
         * @param pcPreviousAtom    Pointer to the previously parsed Atom, must be NULL
         *                          on the first call of an expression.
         *
         * @return Atom*: A newly allocated Atom or NULL if no operators were parsed.
         */
        XAtom                      *ParseOperator(const char *pcszExpr, const char **ppcszEnd, const XAtom *pcPreviousAtom);

        /**
         * Parses the expression for a variable.
         *
         * @param pcszExpr          The expression to parse.
         * @param ppcszEnd          Where to store till-what-point @a pcszExpr was
         *                          parsed.
         * @param pcPreviousAtom    Pointer to the previously parsed Atom, must be NULL
         *                          on the first call of an expression.
         *
         * @return Atom*: A newly allocated Atom or NULL if no variables were parsed.
         */
        XAtom                      *ParseVariable(const char *pcszExpr, const char **ppcszEnd, const XAtom *pcPreviousAtom);

        /**
         * Parses the expression for a command.
         *
         * @param pcszExpr          The expression to parse.
         * @param ppcszEnd          Where to store till-what-point @a pcszExpr was
         *                          parsed.
         * @param pcPreviousAtom    Pointer to the previously parsed Atom, must be NULL
         *                          on the first call of an expression.
         *
         * @return Atom*: A newly allocated Atom or NULL if no commands were parsed.
         */
        XAtom                      *ParseCommand(const char *pcszExpr, const char **ppcszEnd, const XAtom *pcPreviousAtom);

        /**
         * Clean up evaluator state and sets up error object accordingly.
         *
         * @param pStack            Pointer to the current stack.
         * @param rc                The error code.
         * @param pcszError         The string describing the error and required
         *                          details, va_args style.
         */
        void                        CleanUp(std::stack<XAtom*> *pStack, std::queue<XAtom*> *pQueue, int rc, const char *pcszError, ...);

        /**
         * Dumps a given stack of Atoms to debug output.
         *
         * @param pQueue            Pointer to the queue of Atoms.
         */
        void                        DumpAtomStack(std::stack<XAtom *> *pStack);

        /**
         * Dumps a given queue of Atoms to debug output.
         *
         * @param pQueue            Pointer to the queue of Atoms.
         */
        void                        DumpAtomQueue(std::queue<XAtom *> *pQueue);

        bool                        m_fInitialized; /**< Whether this object has been successfully initialized. */
        std::string                 m_sExpr;        /**< The full, unmodified expression */
        std::queue<XAtom*>          m_RPNQueue;     /**< Internal RPN representation done at the parsing stage. */
        std::list<XAtom*>           m_VarList;      /**< List of variables being evaulated, used for circular dependency checks. */
        std::string                 m_sError;       /**< The last error's descriptive string. */
        int                         m_Error;        /**< The last error. */
        static const XFunction      m_sFunctions[]; /**< Static array of Function objects. */
        static const XOperator      m_sOperators[]; /**< Static array of Operator objects. */
        static const size_t         m_cOperators;   /**< Static count of Operators in Operator objects array. */
        Settings                    m_Setttings;    /**< Settings for evaluator. */
        const XOperator            *m_pOpenParenthesisOperator;  /**< Pointer to open parenthesis operator. */
};

#endif /* XANK_EVALUATOR_H */

