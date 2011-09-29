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

#include <gmpxx.h>
#include <string>

#include "EvaluatorDefs.h"

/**
 * The type of Atom.
 * An Atom has to be one of the following distinct types.
 */
typedef enum AtomType
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
} AtomType;

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

        /** Returns the type of Atom. */
        AtomType                    Type() const;
        /** Returns if this is a Number Atom.  */
        bool                        IsNumber() const;
        /** Returns if this is a Function Atom. */
        bool                        IsFunction() const;
        /** Returns if this is a Variable Atom. */
        bool                        IsVariable() const;

    private:
        /** Sets this Atom to be a duplicate of another. */
        void                        SetTo_(const Atom &Atom);

        /** The type this Atom represents. */
        AtomType                    m_AtomType;
        /** Cursor position, an index used to associate an Atom with an error. */
        uint64_t                    m_iPosition;
        /** Number of parameters to pass if this is a Function Atom. */
        uint64_t                    m_cParams;
        /** Name of the variable if this is/might become a Variable Atom. */
        std::string                 m_sVariable;
        union
        {
            /* Value of the number for a Number Atom. */
            mpz_class               Value;
            /* Pointer to the Operator for an Operator Atom. */
            Operator               *pOperator;
            /* Pointer to the Function for a Function Atom. */
            Function               *pFunction;
            /* Pointer to the Variable for a Variable Atom. */
            Variable               *pVariable;
        } u;
};


#endif

