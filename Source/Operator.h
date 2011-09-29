/** @file
 * xank - Operator, header.
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

#ifndef XANK_OPERATOR_H
# define XANK_OPERATOR_H

/**
 * OperatorDir: Operator direction.
 * The associativity associated with the Operator. hehe.
 */
typedef enum OperatorDir
{
    enmOperatorDirNone = 0x30,
    enmOperatorDirLeft,
    enmOperatorDirRight
} OperatorDir;

/** An Operator function. */
typedef int FNOPERATOR(Atom *a_apAtoms[]);
/** Pointer to an Operator function. */
typedef FNOPERATOR *PFNOPERATOR;

/**
 * An Operator.
 * An Operator performs an operation on one or more operands.
 */
typedef struct Operator
{
    /** The operator Id, used to identify certain key Operators. */
    uint32_t        OperatorId;
    /** Operator priority, value is relative to Operators. */
    int32_t         Priority;
    /** Operator associativity. */
    OperatorDIr     Direction;
    /** Number of parameters to the operator. */
    uint64_t        cParams;
    /** Name of the Operator as seen in the expression. */
    std::string     sOperator;
    /** Pointer to the Operator evaluator function. */
    PFNOPERATOR     pfnOperator;
    /** Short description of the Operator. */
    std::string     sShortDesc;
    /** Long description of the Operator. */
    std::string     sLongDesc;
} OPERATOR;
/** Pointer to an Operator object. */
typedef OPERATOR *POPERATOR;
/** Pointer to a const Operator object. */
typedef const OPERATOR *PCOPERATOR;

#endif /* XANK_OPERATOR_H */

