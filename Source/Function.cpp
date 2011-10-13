/** @file
 * xank - Function, implementation.
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

#include "Function.h"
#include "Errors.h"

Function::Function(uint64_t cMinParams, uint64_t cMaxParams, std::string sName, PFNFUNCTION pfnFunction,
                    std::string sShortDesc, std::string sLongDesc)
    : m_cMinParams(cMinParams),
    m_cMaxParams(cMaxParams),
    m_sName(sName),
    m_pfnFunction(pfnFunction),
    m_sShortDesc(sShortDesc),
    m_sLongDesc(sLongDesc)
{
}


Function::Function(const Function &Fn)
{
    SetTo(Fn);
}

Function::~Function()
{
}


inline std::string Function::Name() const
{
    return m_sName;
}


inline int Function::SetName(const std::string &scName)
{
    m_sName = scName;
    return INF_SUCCESS;
}


inline std::string Function::ShortDesc() const
{
    return m_sShortDesc;
}


inline int Function::SetShortDesc(const std::string &scDesc)
{
    m_sShortDesc = scDesc;
    return INF_SUCCESS;
}


inline std::string Function::LongDesc() const
{
    return m_sLongDesc;
}


inline int Function::SetLongDesc(const std::string &scDesc)
{
    m_sLongDesc = scDesc;
    return INF_SUCCESS;
}


inline uint64_t Function::MinParams() const
{
    return m_cMinParams;
}


inline int Function::SetMinParams(uint64_t cMinParams)
{
    m_cMinParams = cMinParams;
    return INF_SUCCESS;
}


inline uint64_t Function::MaxParams() const
{
    return m_cMaxParams;
}


inline int Function::SetMaxParams(uint64_t cMaxParams)
{
    m_cMaxParams = cMaxParams;
    return INF_SUCCESS;
}


inline PFNFUNCTION Function::FunctionPtr() const
{
    return m_pfnFunction;
}


inline int Function::SetFunctionPtr(PFNFUNCTION pfnFunction)
{
    m_pfnFunction = pfnFunction;
    return INF_SUCCESS;
}


inline int Function::Invoke(Atom *apAtoms[], uint64_t cAtoms)
{
    return m_pfnFunction(apAtoms, cAtoms);
}


const Function &Function::operator =(const Function &Fn)
{
    SetTo(Fn);
    return *this;
}


void Function::SetTo(const Function &Fn)
{
    m_cMinParams     = Fn.m_cMinParams;
    m_cMaxParams     = Fn.m_cMaxParams;
    m_sName          = Fn.m_sName;
    m_pfnFunction    = Fn.m_pfnFunction;
    m_sShortDesc     = Fn.m_sShortDesc;
    m_sLongDesc      = Fn.m_sLongDesc;
}

