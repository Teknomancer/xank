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

/**
 * Operator constructor.
 *
 * @param Id_               The Operator Id.
 * @param Priority_         The Operator priority.
 * @param Dir_              The Operator direction.
 * @param cParams_          Number of parameters (valid values are 0, 1 and 2
 *                          only)
 * @param sName_            Name of this Operator.
 * @param pfnOperator_      Function pointer to Operator function.
 * @param sShortDesc_       Short description of this Operator.
 * @param sLongDesc_        Long description of this Operator.
 */
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


/**
 * Operator destructor.
 */
Operator::~Operator()
{
}


/**
 * Returns the Id of this Operator.
 *
 * @return uint32_t.
 */
inline uint32_t Operator::Id() const
{
    return m_Id;
}


/**
 * Returns the direction of this Operator.
 *
 * @return OperatorDirection.
 */
inline OperatorDirection Operator::Direction() const
{
    return m_Direction;
}


/**
 * Returns a copy of the long description of this Operator.
 *
 * @return std::string
 */
inline std::string Operator::LongDesc() const
{
    return m_sLongDesc;
}


/**
 * Returns a copy of the name of this Operator.
 *
 * @return std::string
 */
inline std::string Operator::Name() const
{
    return m_sName;
}


/**
 * Returns a copy of the short description of this Operator.
 *
 * @return std::string
 */
inline std::string Operator::ShortDesc() const
{
    return m_sShortDesc;
}


/**
 * Returns the number of parameters this Operator function takes.
 *
 * @return uint8_t (valid values are 0, 1 and 2 only)
 */
inline uint8_t Operator::Parameters() const
{
    return m_cParams;
}


/**
 * Returns the priority of this Operator.
 *
 * @return int32_t
 */
inline int32_t Operator::Priority() const
{
    return m_Priority;
}

