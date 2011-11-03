/** @file
 * xank - Expression Evaluator defines, header.
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

#ifndef XANK_EVALUATOR_DEFS_H
# define XANK_EVALUATOR_DEFS_H

/** Maximum length of a variable name, in bytes. */
#define XANK_MAX_VARIABLE_NAME_LEN                  128

/**
 * Maximum number of parameters to an operator. Since this is
 * a uint8_t internally, it's UINT8_MAX here. If that changes
 * this must change & vice versa. Probably also have to update
 * all printf format specifiers for them.
 */
#define XANK_MAX_OPERATOR_PARAMETERS                UINT8_MAX

/** Operator Id for Open Paranthesis Operator. */
#define XANK_OPEN_PARENTHESIS_OPERATOR_ID           0

/** Operator Id for Close Paranthesis Operator. */
#define XANK_CLOSE_PARENTHESIS_OPERATOR_ID          1

/** Operator Id for Parameter Separator Operator. */
#define XANK_PARAM_SEPARATOR_OPERATOR_ID            2

/** Operator Id for Assignment Operator. */
#define XANK_ASSIGNMENT_OPERATOR_ID                 3


#endif /* XANK_EVALUATOR_DEFS_H */

