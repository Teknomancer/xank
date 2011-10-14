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


#include "XAtom.h"

#include "Assert.h"
#include "XFunction.h"
#include "XOperator.h"
#include "XVariable.h"
#include "XErrors.h"

#include <cstring>

XAtom::XAtom()
    : m_AtomType(enmAtomTypeEmpty)
{
    std::memset(&m_u, 0, sizeof(m_u));
}


XAtom::XAtom(const XAtom &Atom)
{
    SetTo(Atom);
}


XAtom::~XAtom()
{
    Destroy();
}


bool XAtom::IsFunction() const
{
    return m_AtomType == enmAtomTypeFunction;
}


bool XAtom::IsOperator() const
{
    return m_AtomType == enmAtomTypeOperator;
}


bool XAtom::IsVariable() const
{
    return m_AtomType == enmAtomTypeVariable;
}


bool XAtom::IsNumber() const
{
    return m_AtomType == enmAtomTypeInteger || m_AtomType == enmAtomTypeFloat;
}


bool XAtom::IsInteger() const
{
    return m_AtomType == enmAtomTypeInteger;
}

bool XAtom::IsFloat() const
{
    return m_AtomType == enmAtomTypeFloat;
}


XAtomType XAtom::Type() const
{
    return m_AtomType;
}


const mpz_t *XAtom::Integer() const
{
    if (m_AtomType == enmAtomTypeInteger)
        return &m_u.Integer;
    return NULL;
}


int XAtom::SetInteger(mpz_t *pVal)
{
    Assert(pVal != NULL);
    Destroy();
    mpz_set(m_u.Integer, *pVal);
    m_AtomType = enmAtomTypeInteger;
    return INF_SUCCESS;
}


const mpf_t *XAtom::Float() const
{
    if (m_AtomType == enmAtomTypeFloat)
        return &m_u.Float;
    return NULL;
}


int XAtom::SetFloat(mpf_t *pVal)
{
    Assert(pVal);
    Destroy();
    mpf_set(m_u.Float, *pVal);
    m_AtomType = enmAtomTypeFloat;
    return INF_SUCCESS;
}


const XOperator *XAtom::Operator() const
{
    if (m_AtomType == enmAtomTypeOperator)
        return m_u.pOperator;
    return NULL;
}


int XAtom::SetOperator(XOperator *pOperator)
{
    Destroy();
    m_u.pOperator = pOperator;
    m_AtomType = enmAtomTypeOperator;
    return INF_SUCCESS;
}


const XFunction *XAtom::Function() const
{
    if (m_AtomType == enmAtomTypeFunction)
        return m_u.pFunction;
    return NULL;
}


int XAtom::SetFunction(XFunction *pFunction)
{
    Destroy();
    m_u.pFunction = pFunction;
    m_AtomType = enmAtomTypeFunction;
    return INF_SUCCESS;
}


const XVariable *XAtom::Variable() const
{
    if (m_AtomType == enmAtomTypeVariable)
        return m_u.pVariable;
    return NULL;
}


int XAtom::SetVariable(XVariable *pVariable)
{
    Destroy();
    m_u.pVariable = pVariable;
    m_AtomType = enmAtomTypeVariable;
    return INF_SUCCESS;
}


void XAtom::SetTo(const XAtom &atom)
{
    m_AtomType  = atom.m_AtomType;
    m_cParams   = atom.m_cParams;
    m_iPosition = atom.m_iPosition;
    m_sVariable = atom.m_sVariable;
    m_u         = atom.m_u;
}


void XAtom::Destroy()
{
    std::memset(&m_u, 0, sizeof(m_u));
    m_AtomType = enmAtomTypeEmpty;
}

