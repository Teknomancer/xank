/** @file
 * xank - Errors, header.
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

#ifndef XANK_ERRORS_H
# define XANK_ERRORS_H

/*
 * We use a sed script to convert symbolic names from numerics, marked by the sed _BEGIN & _END markers.
 * Each error code needs to be defined with a description before it in the form of a doxygen comment.
 */
/* ERR_SED_BEGIN */
/** Success! */
#define INF_SUCCESS                                    0
/** Warning, Output was truncated. */
#define WRN_TRUNCATED                               (100)
/** Invalid bitmask combination of flags. */
#define ERR_INVALID_FLAGS                           (-99)
/** No data available. */
#define ERR_NO_DATA                                (-100)
/** Out of memory. */
#define ERR_NO_MEMORY                              (-101)
/** Invalid parameter to function. */
#define ERR_INVALID_PARAMETER                      (-102)
/** Too much data to fill in buffer. */
#define ERR_BUFFER_OVERFLOW                        (-103)
/** Duplicate operator. */
#define ERR_DUPLICATE_OPERATOR                     (-104)
/** Conflicting operator. */
#define ERR_CONFLICTING_OPERATORS                  (-105)
/** Invalid operator. */
#define ERR_INVALID_OPERATOR                       (-106)
/** Functor invalid. */
#define ERR_INVALID_FUNCTOR                        (-107)
/** Duplicate functor. */
#define ERR_DUPLICATE_FUNCTOR                      (-108)
/** Syntax error. */
#define ERR_SYNTAX_ERROR                           (-108)
/** Invalid RPN, parsing had failed. */
#define ERR_INVALID_RPN                            (-110)
/** Invalid expression for grammar. */
#define ERR_EXPRESSION_INVALID                     (-111)
/** Variable name is too long. */
#define ERR_VARIABLE_NAME_TOO_LONG                 (-112)
/** Variable name contains invalid characters. */
#define ERR_VARIABLE_NAME_INVALID                  (-113)
/** Variable undefined but trying to evaluate. */
#define ERR_VARIABLE_UNDEFINED                     (-114)
/** Parenthesis unbalaned. */
#define ERR_PARENTHESIS_UNBALANCED                 (-115)
/** Some fundamental operator missing. */
#define ERR_BASIC_OPERATOR_MISSING                 (-116)
/** Invalid parameter separator position. */
#define ERR_PARANTHESIS_SEPARATOR_UNEXPECTED       (-117)
/** Too many parameters to functor. */
#define ERR_TOO_MANY_PARAMETERS                    (-118)
/** Too few parameters to functor. */
#define ERR_TOO_FEW_PARAMETERS                     (-119)
/** Invalid l-value assignment. */
#define ERR_INVALID_ASSIGNMENT                     (-120)
/** Circular variable dependency. */
#define ERR_CIRCULAR_DEPENDENCY                    (-121)
/** Operator on unitialized object. */
#define ERR_NOT_INITIALIZED                        (-301)
/** Magic mismatch. */
#define ERR_BAD_MAGIC                              (-302)
/** Operation not supported. */
#define ERR_NOT_SUPPORTED                          (-303)
/** Undefined error */
#define ERR_UNDEFINED                              (-666)
/** General failure, who is he? */
#define ERR_GENERAL_FAILURE                        (-667)
/** Evaluation will invoke Undefined Behaviour. */
#define ERR_UNDEFINED_BEHAVIOUR                    (-668)
/* ERR_SED_END */

#define IS_SUCCESS(rc)                              ( (int)(rc) >= INF_SUCCESS )
#define IS_FAILURE(rc)                              ( !IS_SUCCESS(rc) )

/**
 * ErrorMessage.
 * Error message with it's string association.
 */
typedef struct ErrorMessage
{
    /** Symbolic name of error as a string. */
    const char *pcszName;
    /** The error code. */
    int         rc;
} ErrorMessage;

extern const ErrorMessage g_aErrorMessages[];
extern const ErrorMessage g_UnknownErrorMessage;

const ErrorMessage *ErrorMessageForRC(int rc);

#endif /* XANK_ERRORS_H___ */

