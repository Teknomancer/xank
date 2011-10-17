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

class XAtom;
class XFunction;
class XOperator;

class XEvaluator
{
    public:
        XEvaluator();
        virtual ~XEvaluator();

        /**
         * Parses an expression.
         *
         * @param pcszExr           The expression to parse.
         *
         * @return int: xank error code.
         */
        int                         Parse(const char *pcszExr);

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

        std::string                 m_sExpr;        /**< The full, unmodified expression */
        std::queue<XAtom*>          m_RPNQueue;     /**< Internal RPN representation done at the parsing stage. */
        std::list<XAtom*>           m_VarList;      /**< List of variables being evaulated, used for circular dependency checks. */
        static const XFunction      m_sFunctions[]; /**< Static array of Function objects. */
        static const XOperator      m_sOperators[]; /**< Static array of Operator objects. */
};

#endif /* XANK_EVALUATOR_H */
