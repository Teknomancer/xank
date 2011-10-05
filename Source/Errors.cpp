/** @file
 * xank - Errors, implementation.
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

/*******************************************************************************
 *   Header Files                                                              *
 *******************************************************************************/
#include "Errors.h"
#include "GenericDefs.h"

#include <stdlib.h>

/*******************************************************************************
 *   Globals, Typedefs & Defines                                               *
 *******************************************************************************/
const ErrorMessage g_UnknownErrorMessage =
{
    "Unknown status.", 1
};

const ErrorMessage g_aErrorMessages[] =
{
#include "GenErrorData.h"
    { NULL, 0 }
};


/**
 * Finds the error message object for an error/return code.
 *
 * @param   rc          The error code to find.
 * @returns Pointer to the matching Status Message object.
 */
const ErrorMessage *ErrorMessageForRC(int rc)
{
    for (unsigned i = 0; i < XANK_ARRAY_ELEMENTS(g_aErrorMessages); i++)
    {
        if (g_aErrorMessages[i].rc == rc)
            return &g_aErrorMessages[i];
    }

    return &g_UnknownErrorMessage;
}

