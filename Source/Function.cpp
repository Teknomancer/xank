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

Function::Function(std::string sName_, uint64_t cMinParams_, uint64_t cMaxParams_, ParameterType ParamType_,
                    PFNFUNCTION pfnFunction_, std::string sShortDesc_, std::string sLongDesc_)
    : m_sName(sName_),
    m_cMinParams(cMinParams_),
    m_cMaxParams(cMaxParams_),
    m_ParamType(ParamType_),
    m_pfnFunction(pfnFunction_),
    m_sShortDesc(sShortDesc_),
    m_sLongDesc(sLongDesc_)
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
    return m_ParamType;
}


inline int Function::InvokeFunction(Atom *apAtoms_[], uint64_t cAtoms_)
{
    return m_pfnFunction(apAtoms_, cAtoms_);
}

