/** @file
 * xank - Atom, implementation.
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

#include "Atom.h"

/**
 * Atom constructor.
 *
 */
Atom::Atom()
    : m_AtomType(enmAtomTypeEmpty)
{
}


/**
 * Atom copy constructor.
 *
 */
Atom::Atom(const Atom &a_Atom)
{
    SetTo_(a_Atom);
}

/**
 * Atom destructor.
 *
 */
Atom::~Atom()
{
}


/**
 * Returns if this Atom is a Function or not.
 *
 * @return  true if this Atom is a function, false otherwise.
 */
inline bool Atom::IsFunction() const
{
    return m_AtomType == enmAtomTypeFunction;
}


/**
 * Returns if this Atom is an Operator or not.
 *
 * @return  true if this Atom is an operator, false otherwise.
 */
inline bool Atom::IsOperator() const
{
    return m_AtomType == enmAtomTypeOperator;
}


/**
 * Returns if this Atom is a Variable or not.
 *
 * @return  true if this Atom is a variable, false otherwise.
 */
inline bool Atom::IsVariable() const
{
    return m_AtomType == enmAtomTypeVariable;
}


/**
 * Returns if this Atom is a Number or not.
 *
 * @return  true if this Atom is a number, false otherwise.
 */
inline bool Atom::IsNumber() const
{
    return m_AtomType == enmAtomTypeNumber;
}


/**
 * Returns the type for this Atom.
 *
 * @returns The AtomType of this Atom.
 */
inline AtomType Atom::Type() const
{
    return m_AtomType;
}


/**
 * Sets this Atom to be a duplicate of another.
 *
 * @param a_Atom            Reference to the source Atom.
 */
void Atom::SetTo_(const Atom &a_Atom)
{
    m_AtomType  = a_Atom.m_AtomType;
    m_cParams   = a_Atom.m_cParams;
    m_iPosition = a_Atom.m_iPosition;
    m_sVariable = a_Atom.m_sVariable;
}


