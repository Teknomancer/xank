/** @file
 * xank - Operator, implementation.
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

#include "XOperator.h"
#include "XEvaluatorDefs.h"

XOperator::XOperator()
    : m_uId(UINT32_MAX)
{
}


XOperator::XOperator(uint32_t uId, int32_t iPriority, XOperatorDir Direction, uint8_t cParams, std::string sName,
                    PFNOPERATOR pfnOperator, std::string sShortDesc, std::string sLongDesc)
    : m_uId(uId),
    m_iPriority(iPriority),
    m_Dir(Direction),
    m_cParams(cParams),
    m_sName(sName),
    m_pfnOperator(pfnOperator),
    m_sShortDesc(sShortDesc),
    m_sLongDesc(sLongDesc)
{
}


XOperator::~XOperator()
{
}


uint32_t XOperator::Id() const
{
    return m_uId;
}


XOperatorDir XOperator::Dir() const
{
    return m_Dir;
}


std::string XOperator::LongDesc() const
{
    return m_sLongDesc;
}


std::string XOperator::Name() const
{
    return m_sName;
}


std::string XOperator::ShortDesc() const
{
    return m_sShortDesc;
}


uint8_t XOperator::Params() const
{
    return m_cParams;
}


int32_t XOperator::Priority() const
{
    return m_iPriority;
}


bool XOperator::IsOpenParanthesis() const
{
    return (m_uId == XANK_OPEN_PARANTHESIS_OPERATOR_ID);
}


bool XOperator::IsCloseParanthesis() const
{
    return (m_uId == XANK_CLOSE_PARANTHESIS_OPERATOR_ID);
}


bool XOperator::IsParameterSeparator() const
{
    return (m_uId == XANK_PARAM_SEPARATOR_OPERATOR_ID);
}


bool XOperator::IsAssignment() const
{
    return (m_uId == XANK_ASSIGNMENT_OPERATOR_ID);
}

