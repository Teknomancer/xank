/** @file
 * xank - Expression Evaluator, header.
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

#ifndef XANK_EVALUATOR_H
# define XANK_EVALUATOR_H

class Atom;

class Evaluator
{
    public:
        Evaluator();
        virtual ~Evaluator();

    private:
        std::queue<Atom*>           m_RPNQueue; /**< Internal RPN representation done at the parsing stage. */
        std::list<Atom*>            m_VarList;  /**< List of variables being evaulated, used for circular dependency checks. */

};

#endif /* XANK_EVALUATOR_H */

