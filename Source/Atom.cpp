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


Atom::Atom()
    : m_AtomType(enmAtomTypeEmpty)
{
}


Atom::Atom(const Atom &a_Atom)
{
    SetTo_(a_Atom);
}


Atom::~Atom()
{
}


inline bool Atom::IsFunction() const
{
    return m_AtomType == enmAtomTypeFunction;
}


inline bool Atom::IsOperator() const
{
    return m_AtomType == enmAtomTypeOperator;
}


inline bool Atom::IsVariable() const
{
    return m_AtomType == enmAtomTypeVariable;
}


inline bool Atom::IsNumber() const
{
    return m_AtomType == enmAtomTypeNumber;
}


inline AtomType Atom::Type() const
{
    return m_AtomType;
}


void Atom::SetTo_(const Atom &a_Atom)
{
    m_AtomType  = a_Atom.m_AtomType;
    m_cParams   = a_Atom.m_cParams;
    m_iPosition = a_Atom.m_iPosition;
    m_sVariable = a_Atom.m_sVariable;
    m_u         = a_Atom.m_u;
}

