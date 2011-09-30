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

class Atom;

/**
 * OperatorDirection: Operator direction.
 * The associativity associated with the Operator. hehe.
 */
typedef enum OperatorDirection
{
    enmOperatorDirectionNone = 0x30,
    enmOperatorDirectionLeft,
    enmOperatorDirectionRight
} OperatorDirection;

/** An Operator function. */
typedef int FNOPERATOR(Atom *apAtoms_[]);
/** Pointer to an Operator function. */
typedef FNOPERATOR *PFNOPERATOR;

/**
 * An Operator.
 * An Operator performs an operation on one or more operands.
 */
class Operator
{
    public:
        Operator(uint32_t Id_, int32_t Priority_, OperatorDirection Dir_, uint8_t cParams_, std::string sName,
            PFNOPERATOR pfnOperator, std::string sShortDesc, std::string sLongDesc);
        virtual ~Operator();

        /** Returns the Id of this Operator. */
        uint32_t                Id() const;
        /** Returns the priority of this Operator. */
        int32_t                 Priority() const;
        /** Returns the direction (associativity) of this Operator. */
        OperatorDirection       Direction() const;
        /** Returns the number of parameters taken by the Operator function. */
        uint8_t                 Parameters() const;
        /** Returns a copy of the name of this Operator. */
        std::string             Name() const;
        /** Returns a copy of the short description of this Operator. */
        std::string             ShortDesc() const;
        /** Returns a copy of the long descriptrion of this Operator. */
        std::string             LongDesc() const;
        /** Invokes the function associated with this Operator. */
        int                     InvokeFunction(Atoms *apAtoms_[]);

    private:
        uint32_t                m_Id;           /**< The operator Id, used to identify certain key Operators. */
        int32_t                 m_Priority;     /**< Operator priority, value is relative to Operators. */
        OperatorDIr             m_Direction;    /**< Operator associativity. */
        uint8_t                 m_cParams;      /**< Number of parameters to the operator (valid values: 0, 1 or 2) */
        std::string             m_sName;        /**< Name of the Operator as seen in the expression. */
        PFNOPERATOR             m_pfnOperator;  /**< Pointer to the Operator evaluator function. */
        std::string             m_sShortDesc;   /**< Short description of the Operator. */
        std::string             m_sLongDesc;    /**< Long description of the Operator. */

}

#endif /* XANK_OPERATOR_H */

