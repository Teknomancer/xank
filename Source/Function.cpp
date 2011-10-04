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

Function::Function(std::string sName, uint64_t cMinParams, uint64_t cMaxParams, ParameterType enmParamType,
                    PFNFUNCTION pfnFunction, std::string sShortDesc, std::string sLongDesc)
    : m_sName(sName),
    m_cMinParams(cMinParams),
    m_cMaxParams(cMaxParams),
    m_enmParamType(enmParamType),
    m_pfnFunction(pfnFunction),
    m_sShortDesc(sShortDesc),
    m_sLongDesc(sLongDesc)
{
}


Function::~Function()
{
}


inline std::string Function::Name() const
{
    return m_sName;
}


inline std::string Function::ShortDesc() const
{
    return m_sShortDesc;
}


inline std::string Function::LongDesc() const
{
    return m_sLongDesc;
}


inline uint64_t Function::MinParams() const
{
    return m_cMinParams;
}


inline uint64_t Function::MaxParams() const
{
    return m_cMaxParams;
}


inline ParameterType Function::ParamType() const
{
    return m_enmParamType;
}


inline int Function::InvokeFunction(Atom *apAtoms[], uint64_t cAtoms)
{
    return m_pfnFunction(apAtoms, cAtoms);
}

