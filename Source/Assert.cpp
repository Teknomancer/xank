/** @file
 * xank - Assert, implementation.
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

#include "Assert.h"
#include <iostream>

void AssertMsg(char *pszExpr, unsigned uLine, char *pszFile, const char *pszFunction)
{
    std::cerr << "\n!!Assertion Failed!!\n"
            "Expression: " << pszExpr << "\n"
            "Location:   " << pszFile << ":" << uLine << " " << pszFunction;
}

