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

#include "XFunction.h"
#include "XErrors.h"

XFunction::XFunction(uint64_t cMinParams, uint64_t cMaxParams, std::string sName, PFNFUNCTION pfnFunction,
                    std::string sShortDesc, std::string sLongDesc)
    : m_cMinParams(cMinParams),
    m_cMaxParams(cMaxParams),
    m_sName(sName),
    m_sShortDesc(sShortDesc),
    m_sLongDesc(sLongDesc)
{
}


XFunction::XFunction(const XFunction &Fn)
{
    SetTo(Fn);
}

XFunction::~XFunction()
{
}

std::string XFunction::Name() const
{
    return m_sName;
}


int XFunction::SetName(const std::string &scName)
{
    m_sName = scName;
    return INF_SUCCESS;
}


std::string XFunction::ShortDesc() const
{
    return m_sShortDesc;
}


int XFunction::SetShortDesc(const std::string &scDesc)
{
    m_sShortDesc = scDesc;
    return INF_SUCCESS;
}


std::string XFunction::LongDesc() const
{
    return m_sLongDesc;
}


int XFunction::SetLongDesc(const std::string &scDesc)
{
    m_sLongDesc = scDesc;
    return INF_SUCCESS;
}


uint64_t XFunction::MinParams() const
{
    return m_cMinParams;
}


int XFunction::SetMinParams(uint64_t cMinParams)
{
    m_cMinParams = cMinParams;
    return INF_SUCCESS;
}


uint64_t XFunction::MaxParams() const
{
    return m_cMaxParams;
}


int XFunction::SetMaxParams(uint64_t cMaxParams)
{
    m_cMaxParams = cMaxParams;
    return INF_SUCCESS;
}


PFNFUNCTION XFunction::Function() const
{
    return m_pfnFunction;
}


int XFunction::SetFunction(PFNFUNCTION pfnFunction)
{
    m_pfnFunction = pfnFunction;
    return INF_SUCCESS;
}


int XFunction::Invoke(XAtom *apAtoms[], uint64_t cAtoms)
{
    int rc = (*m_pfnFunction)(apAtoms, cAtoms);
    return rc;
}


const XFunction &XFunction::operator =(const XFunction &Fn)
{
    SetTo(Fn);
    return *this;
}


void XFunction::SetTo(const XFunction &Fn)
{
    m_cMinParams     = Fn.m_cMinParams;
    m_cMaxParams     = Fn.m_cMaxParams;
    m_sName          = Fn.m_sName;
    m_pfnFunction    = Fn.m_pfnFunction;
    m_sShortDesc     = Fn.m_sShortDesc;
    m_sLongDesc      = Fn.m_sLongDesc;
}


std::string XFunction::PrintToString() const
{
    /** @todo fill in the other members here  */
    std::string sOut;
    sOut =  "Name      : " + m_sName + "\n";
    sOut += "Min Params: ";
    /* What the bloody hell? Can't append uint64_t to std::string on Winblows? */
#ifdef XANK_OS_WINDOWS
    sOut += (uint32_t)m_cMinParams;
#else
    sOut += (uint32_t)m_cMinParams;
#endif
    sOut += "\n";
    sOut += "Max Params: ";
#ifdef XANK_OS_WINDOWS
    sOut += (uint32_t)m_cMaxParams;
#else
    sOut += m_cMaxParams;
#endif
    sOut += "\n";
    return sOut;
}
