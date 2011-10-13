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

XAtom::Atom()
    : m_AtomType(enmAtomTypeEmpty)
{
}


XAtom::Atom(const Atom &a_Atom)
{
    SetTo(a_Atom);
}


XAtom::~Atom()
{
}

inline XAtom::SetFunction(Function *pFunction)
{

}


inline bool XAtom::IsFunction() const
{
    return m_AtomType == enmAtomTypeFunction;
}


inline bool XAtom::IsOperator() const
{
    return m_AtomType == enmAtomTypeOperator;
}


inline bool XAtom::IsVariable() const
{
    return m_AtomType == enmAtomTypeVariable;
}


inline bool XAtom::IsNumber() const
{
    return m_AtomType == enmAtomTypeInteger || m_AtomType == enmAtomTypeFloat;
}


inline bool XAtom::IsInteger() const
{
    return m_AtomType == enmAtomTypeInteger;
}


inline bool XAtom::IsFloat() const
{
    return m_AtomType == enmAtomTypeFloat;
}


inline XAtomType XAtom::Type() const
{
    return m_AtomType;
}


inline XOperator *XAtom::Operator() const
{
    if (m_AtomType == enmAtomTypeOperator)
        return m_u.pOperator;
    return NULL;
}


inline XFunction *XAtom::Function() const
{
    if (m_AtomType == enmAtomTypeFunction)
        return m_u.pFunction;
    return NULL;
}


inline mpz_t *XAtom::Integer() const
{
    if (m_AtomType == enmAtomTypeInteger)
        return m_u.pInteger;
    return NULL;
}


inline mpq_t *XAtom::Float() const
{
    if (m_AtomType == enmAtomTypeFloat)
        return m_u.pFloat;
    return NULL;
}


void XAtom::SetTo(const Atom &atom)
{
    m_AtomType  = atom.m_AtomType;
    m_cParams   = atom.m_cParams;
    m_iPosition = atom.m_iPosition;
    m_sVariable = atom.m_sVariable;
    m_u         = atom.m_u;
}

