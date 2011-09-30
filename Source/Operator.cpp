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

#include "Operator.h"
#include "EvaluatorDefs.h"

Operator::Operator(uint32_t Id_, int32_t Priority_, OperatorDirection Dir_, uint8_t cParams_, std::string sName_,
                    PFNOPERATOR pfnOperator_, std::string sShortDesc_, std::string sLongDesc_)
    : m_Id(Id_),
    m_Priority(Priority_),
    m_Direction(Dir_),
    m_cParams(cParams_),
    m_sName(sName_),
    m_pfnOperator(pfnOperator_),
    m_sShortDesc(sShortDesc_),
    m_sLongDesc(sLongDesc_)
{
}


Operator::~Operator()
{
}


inline uint32_t Operator::Id() const
{
    return m_Id;
}


inline OperatorDirection Operator::Direction() const
{
    return m_Direction;
}


inline std::string Operator::LongDesc() const
{
    return m_sLongDesc;
}


inline std::string Operator::Name() const
{
    return m_sName;
}


inline std::string Operator::ShortDesc() const
{
    return m_sShortDesc;
}


inline uint8_t Operator::Parameters() const
{
    return m_cParams;
}


inline int32_t Operator::Priority() const
{
    return m_Priority;
}


inline bool Operator::IsOpenParanthesis() const
{
    return (m_Id == XANK_OPEN_PARANTHESIS_OPERATOR_ID);
}


inline bool Operator::IsCloseParanthesis() const
{
    return (m_Id == XANK_CLOSE_PARANTHESIS_OPERATOR_ID);
}


inline bool Operator::IsParameterSeparator() const
{
    return (m_Id == XANK_PARAM_SEPARATOR_OPERATOR_ID);
}


inline bool Operator::IsAssignment() const
{
    return (m_Id == XANK_ASSIGNMENT_OPERATOR_ID);
}


