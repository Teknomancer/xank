/** @file
 * xank - Variable, header.
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

#ifndef XANK_VARIABLE_H
# define XANK_VARiABLE_H

#include <queue>
#include <string>

class Atom;

/**
 * A Variable.
 * A variable represents a substitutable entity in a parsed expression.
 */
class Variable
{
    public:
        Variable();
        virtual ~Variable();

    private:
        std::string             m_sVariable;    /**< Name of the Variable as seen in the expression. */
        std::string             m_sExpr;        /**< The expression assigned to the variable. */
        std::queue<Atom*>       m_RPNQueue;     /** The RPN Queue. */
};

#endif /* XANK_VARIABLE_H */

