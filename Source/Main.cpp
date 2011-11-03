/** @file
 * xank - Main, implementation.
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

#include "XEvaluator.h"
#include "ConsoleIO.h"
#include "XErrors.h"
#include "XGenericDefs.h"

int main(int argc, char **argv)
{
    NOREF(argc); NOREF(argv);
    ConsoleIO Console;
    XEvaluator Eval;
    int rc = Eval.Init();
    if (IS_SUCCESS(rc))
    {
        Console.ColorPrintf(enmConsoleColorRed, "Initialized Evaluator %s.\n", "Hello");
    }
    else
        Console.ColorPrintf(enmConsoleColorRed, "Evaluator initilization failed.\n");
    return 0;
}
