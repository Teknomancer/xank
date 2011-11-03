/** @file
 * xank - Atom, header.
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

#ifndef XANK_ATOM_H
# define XANK_ATOM_H

#include <stdint.h>
#include <gmpxx.h>

#include <string>

#include "XEvaluatorDefs.h"

class XOperator;
class XFunction;
class XVariable;


/**
 * The type of Atom.
 * An Atom has to be one of the following distinct types.
 */
enum XAtomType
{
    /** Empty/uninitialized Atom. */
    enmAtomTypeEmpty = 0x10,
    /** Atom represents an integer. */
    enmAtomTypeInteger,
    /** Atom represents a float. */
    enmAtomTypeFloat,
    /** Atom represents an operator. */
    enmAtomTypeOperator,
    /** Atom represents a function. */
    enmAtomTypeFunction,
    /** Atom represents a variable. */
    enmAtomTypeVariable
};

/**
 * An Atom represents the smallest unit of parsing.
 * The Atom class represents a token of the unit parsing stage
 * of the Evaluator class.
 */
class XAtom
{
    public:
        XAtom();
        XAtom(const XAtom &a_Atom);
        virtual ~XAtom();

        /**
         * Returns the type of this Atom.
         *
         * @return AtomType
         */
        XAtomType                   Type() const;

        /**
         * Returns if this Atom is a number (i.e. either Integer or Float Atom).
         *
         * @return bool: true if it's a number, false otherwise.
         */
        bool                        IsNumber() const;

        /**
         * Returns if this Atom is an integer number.
         *
         * @return bool: true if it's an integer number, false otherwise.
         */
        bool                        IsInteger() const;

        /**
         * Returns if this Atom is a floating point number.
         *
         * @return bool: true if it's a floating point number, false otherwise.
         */
        bool                        IsFloat() const;

        /**
         * Returns if this Atom is a function.
         *
         * @return bool: true if it's a function, false otherwise.
         */
        bool                        IsFunction() const;

        /**
         * Returns if this Atom is a variable.
         *
         * @return bool: true if it's a variable, false otherwise.
         */
        bool                        IsVariable() const;

        /**
         * Returns if this Atom is an operator.
         *
         * @return bool: true if it's an operator, false otherwise.
         */
        bool                        IsOperator() const;

        /**
         * Gets the integer value for an Integer Atom.
         *
         * @param Result            Where to set the integer value.
         *
         * @return int: xank error code.
         */
        int                         GetInteger(mpz_t Result) const;

        /**
         * Sets the integer value from a string.
         *
         * @param pcszStr           The string representation of the itneger value.
         * @param iRadix            The radix of the integer value in @a pcszStr.
         *
         * @return int: xank error code.
         */
        int                         SetIntegerFromStr(const char *pcszStr, int iRadix);

        /**
         * Gets the floating point value for this Float Atom.
         *
         * @param Result            Where to set the floating point value.
         *
         * @return int: xank error code.
         */
        int                         GetFloat(mpf_t Result) const;

        /**
         * Sets the floating point value from a string.
         *
         * @param pcszStr           The string representation of the floating point
         *                          value.
         * @param iRadix            The radix of the floating point value in @a pcszStr.
         *
         * @return int: xank error code.
         */
        int                         SetFloatFromStr(const char *pcszStr, int iRadix);

        /**
         * Returns pointer to the Operator Atom.
         *
         * @return Operator*: Pointer to the operator for this Operator Atom, or NULL if
         *         this is not an Operator Atom.
         */
        const XOperator            *Operator() const;

        /**
         * Sets the operator and makes this an Operator Atom.
         *
         * @param pOperator         Pointer to the Operator.
         *
         * @return int: xank error code.
         */
        int                         SetOperator(const XOperator *pOperator);

        /**
         * Returns pointer to the Function Atom.
         *
         * @return Function*: Pointer to the function for this Function Atom, or NULL if
         *         this is not a Function Atom.
         */
        const XFunction            *Function() const;

        /**
         * Sets the function and makes it a Function Atom.
         *
         * @param pFunction         Pointer to the Function.
         *
         * @return int: xank error code.
         */
        int                         SetFunction(const XFunction *pFunction);

        /**
         * Returns pointer to the Variable Atom.
         *
         * @return Variable*: Pointer to the variable for this Variable Atom, or NULL if
         *         this is not a Variable Atom.
         */
        const XVariable            *Variable() const;

        /**
         * Sets the variable and makes this a Variable Atom.
         *
         * @param pVariable         Pointer to the Variable.
         *
         * @return int: xank error code.
         */
        int                         SetVariable(XVariable *pVariable);

        /**
         * Increments function parameters for a Function Atom.
         * Has no effect if invoked on a non-Function Atom.
         *
         */
        void                        IncrementFunctionParams();

        /**
         * Returns number of function parameters for a Function Atom.
         *
         * @return uint64_t: Number of function parameters, undefined
         * if this is not a Function Atom.
         */
        uint64_t                    FunctionParams() const;

        /**
         * Prints the current state of this Atom to a string and returns it.
         *
         * @return std::string
         */
        std::string                 PrintToString() const;

    private:
        /**
         * Sets this Atom to be identical to the passed in Atom.
         *
         * @param Atom          The source Atom.
         */
        void                        SetTo(const XAtom &Atom);

        /**
         * Destroys this object and reinitializes it to empty state.
         */
        void                        Destroy();

        XAtomType                   m_AtomType;   /**< The type this Atom represents. */
        uint64_t                    m_iPosition;  /**< Cursor position, an index used to associate an Atom with an error. */
        uint64_t                    m_cParams;    /**< Number of parameters if this is a Function Atom. */
        std::string                 m_sVariable;  /**< Name of the variable if this is/might become a Variable Atom. */
        union
        {
            mpz_t                   Integer;      /**< Integer value for a Number Atom. */
            mpf_t                   Float;        /**< Float point value for a Number Atom. */
            const XOperator        *pOperator;    /*< Pointer to the Operator for an Operator Atom. */
            const XFunction        *pFunction;    /*< Pointer to the Function for a Function Atom. */
            const XVariable        *pVariable;    /*< Pointer to the Variable for a Variable Atom. */
        } m_u;
};

#endif /* XANK_ATOM_H */

