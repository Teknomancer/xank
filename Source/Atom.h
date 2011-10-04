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
#include <gmp.h>

#include <string>

#include "EvaluatorDefs.h"

class Operator;
class Function;
// class Variable;

/**
 * The type of Atom.
 * An Atom has to be one of the following distinct types.
 */
enum AtomType
{
    /** Empty/uninitialized Atom. */
    enmAtomTypeEmpty = 0x10,
    /** Atom represents a number. */
    enmAtomTypeNumber,
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
class Atom
{
    public:
        Atom();
        Atom(const Atom &a_Atom);
        virtual ~Atom();

        /**
         * Returns the type of this Atom.
         *
         * @return AtomType
         */
        AtomType                    Type() const;

        /**
         * Returns if this Atom is a number.
         *
         * @return bool: true if it's a number, false otherwise.
         */
        bool                        IsNumber() const;

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

    private:
        /**
         * Sets this Atom to be identical to the passed in Atom.
         *
         * @param Atom          The source Atom.
         */
        void                        SetTo(const Atom &Atom);

        AtomType                    m_AtomType;   /**< The type this Atom represents. */
        uint64_t                    m_iPosition;  /**< Cursor position, an index used to associate an Atom with an error. */
        uint64_t                    m_cParams;    /**< Number of parameters if this is a Function Atom. */
        std::string                 m_sVariable;  /**< Name of the variable if this is/might become a Variable Atom. */
        union
        {
            mpz_t                  Value;        /**< Value of the number for a Number Atom. */
            Operator               *pOperator;    /*< Pointer to the Operator for an Operator Atom. */
            Function               *pFunction;    /*< Pointer to the Function for a Function Atom. */
            //Variable               *pVariable;    /*< Pointer to the Variable for a Variable Atom. */
        } m_u;
};

#endif /* XANK_ATOM_H */

