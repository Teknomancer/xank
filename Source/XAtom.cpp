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
    : m_AtomType(enmAtomTypeEmpty),
    m_iPosition(0),
    m_cParams(0)
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


int XAtom::GetInteger(mpz_t Result) const
{
    if (m_AtomType == enmAtomTypeInteger)
    {
        mpz_set(Result, m_u.Integer);
        return INF_SUCCESS;
    }
    return ERR_INVALID_ATOM_TYPE_FOR_OPERATION;
}


int XAtom::SetIntegerFromStr(const char *pcszStr, int iRadix)
{
    Destroy();
    mpz_init_set_str(m_u.Integer, pcszStr, iRadix);
    m_AtomType = enmAtomTypeInteger;
    return INF_SUCCESS;
}


int XAtom::GetFloat(mpf_t Result) const
{
    if (m_AtomType == enmAtomTypeFloat)
    {
        mpf_set(Result, m_u.Float);
        return INF_SUCCESS;
    }
    return ERR_INVALID_ATOM_TYPE_FOR_OPERATION;
}


int XAtom::SetFloatFromStr(const char *pcszStr, int iRadix)
{
    Destroy();
    mpf_init_set_str(m_u.Float, pcszStr, iRadix);
    m_AtomType = enmAtomTypeFloat;
    return INF_SUCCESS;
}


const XOperator *XAtom::Operator() const
{
    if (m_AtomType == enmAtomTypeOperator)
        return m_u.pOperator;
    return NULL;
}


int XAtom::SetOperator(const XOperator *pOperator)
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


int XAtom::SetFunction(const XFunction *pFunction)
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
    if (m_AtomType == enmAtomTypeFloat)
        mpf_clear(m_u.Float);
    else if (m_AtomType == enmAtomTypeInteger)
        mpz_clear(m_u.Integer);

    std::memset(&m_u, 0, sizeof(m_u));
    m_AtomType = enmAtomTypeEmpty;
}


void XAtom::IncrementFunctionParams()
{
    if (m_AtomType == enmAtomTypeFunction)
        m_cParams++;
}


uint64_t XAtom::FunctionParams() const
{
    if (m_AtomType == enmAtomTypeFunction)
        return m_cParams;
    return UINT64_MAX;
}


std::string XAtom::PrintToString() const
{
    std::string sOut = "Atom: ";
    sOut += "Type: ";
    switch (m_AtomType)
    {
        case enmAtomTypeEmpty:
        {
            sOut += "Empty";
            break;
        }

        case enmAtomTypeInteger:
        {
            sOut += "Int: ";
            char *pszBuf = NULL;
            int rc = gmp_asprintf(&pszBuf, "%Zd", m_u.Integer);
            if (rc > 0)
            {
                sOut += pszBuf;
                free(pszBuf);
                pszBuf = NULL;
            }
            else
                sOut += "<NoMem?>";
            break;
        }

        case enmAtomTypeFloat:
        {
            sOut += "Float: ";
            char *pszBuf = NULL;
            int rc = gmp_asprintf(&pszBuf, "%.2Ff", m_u.Float);
            if (rc > 0)
            {
                sOut += pszBuf;
                free(pszBuf);
                pszBuf = NULL;
            }
            else
                sOut += "<NoMem?>";
            break;
        }

        case enmAtomTypeFunction:
        {
            sOut += "Function: ";
            sOut += m_u.pFunction ? m_u.pFunction->Name() : "NULL";
            break;
        }

        case enmAtomTypeOperator:
        {
            sOut += "Operator: ";
            sOut += m_u.pOperator ? m_u.pOperator->Name() : "NULL";
            break;
        }

        case enmAtomTypeVariable:
        {
            sOut += "Variable";
            sOut += m_u.pVariable ? m_u.pVariable->Name() : "NULL";
            break;
        }
    }

    return sOut;
}

