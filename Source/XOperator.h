/** @file
 * xank - Operator, header.
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

#ifndef XANK_OPERATOR_H
# define XANK_OPERATOR_H

#include <stdint.h>

#include <string>

class XAtom;

/**
 * OperatorDirection: Operator direction.
 * The associativity associated with the Operator. hehe.
 */
enum XOperatorDir
{
    enmOperatorDirNone = 0x30,
    enmOperatorDirLeft,
    enmOperatorDirRight
};

/** An Operator function. */
typedef int FNOPERATOR(XAtom *apAtoms[], size_t cAtoms);
/** Pointer to an Operator function. */
typedef FNOPERATOR *PFNOPERATOR;

/**
 * An Operator.
 * An Operator performs an operation on one or more operands.
 */
class XOperator
{
    public:
        XOperator();
        XOperator(uint32_t uId, int32_t iPriority, XOperatorDir Dir, uint8_t cParams, std::string sName,
            PFNOPERATOR pfnOperator, std::string sShortDesc, std::string sLongDesc);
        virtual ~XOperator();

        /**
         * Returns the Id of this Operator.
         *
         * @return uint32_t
         */
        uint32_t                Id() const;

        /**
         * Returns the priority of this Operator.
         *
         * @return int32_t
         */
        int32_t                 Priority() const;

        /**
         * Returns the direction (associativity) of this Operator.
         *
         * @return OperatorDirection
         */
        XOperatorDir            Dir() const;

        /**
         * Returns the number of parameters taken by the Operator function.
         *
         * @return uint8_t
         */
        uint8_t                 Params() const;

        /**
         * Returns a copy of the name of this Operator.
         *
         * @return std::string
         */
        std::string             Name() const;

        /**
         * Returns a copy of the short description of this Operator.
         *
         * @return std::string
         */
        std::string             ShortDesc() const;

        /**
         * Returns a copy of the long descriptrion of this Operator.
         *
         * @return std::string
         */
        std::string             LongDesc() const;

        /**
         * Returns a pointer to the function of this Operator.
         *
         * @return PFNFUNCTION
         */
        PFNOPERATOR             Function() const;

        /**
         * Invokes the function associated with this Operator.
         *
         * @param apAtoms               Array of pointers to operand Atoms.
         * @param cAtoms                Number of items in @a apAtoms.
         *
         * @return int: xank error code.
         */
        int                     Invoke(XAtom *apAtoms[], size_t cAtoms) const;

        /**
         * Returns if this Operator is the Open Parenthesis Operator.
         *
         * @return bool: true if it's the Open Parenthesis Operator, false otherwise.
         */
        bool                    IsOpenParenthesis() const;

        /**
         * Returns if this Operator is the Close Parenthesis Operator.
         *
         * @return bool: true if it's the Close Parenthesis Operator, false otherwise.
         */
        bool                    IsCloseParenthesis() const;

        /**
         * Returns if this Operator is the Parameter Separator Operator.
         *
         * @return bool: true if it's the Parameter Separator Operator, false otherwise.
         */
        bool                    IsParamSeparator() const;

        /**
         * Returns if this Operator is the Assignment Operator.
         *
         * @return bool: true if it's the Assignment Operator, false otherwise.
         */
        bool                    IsAssignment() const;

        /**
         * Prints the current state of this Operator to a string and returns it.
         *
         * @return std::string
         */
        std::string             PrintToString() const;

    private:
        uint32_t                m_uId;          /**< The operator Id, used to identify certain key Operators. */
        int32_t                 m_iPriority;    /**< Operator priority, value is relative to Operators. */
        XOperatorDir            m_Dir;          /**< Operator associativity direction. */
        uint8_t                 m_cParams;      /**< Number of parameters to the operator, see XANK_MAX_OPERATOR_PARAMETERS. */
        std::string             m_sName;        /**< Name of the Operator as seen in the expression. */
        PFNOPERATOR             m_pfnOperator;  /**< Pointer to the Operator evaluator function. */
        std::string             m_sShortDesc;   /**< Short description of the Operator. */
        std::string             m_sLongDesc;    /**< Long description of the Operator. */
};

#endif /* XANK_OPERATOR_H */

