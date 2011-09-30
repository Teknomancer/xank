/** @file
 * xank - Function, header.
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

#ifndef XANK_FUNCTION_H
# define XANK_FUNCTION_H

/** A Functor function. */
typedef int FNFUNCTION(Atoms *apAtoms_[], uint64_t cAtoms_);
/** Pointe to a Functor function. */
typedef FNFUNCTION *PFNFUNCTION;

/**
 * FunctionParameter: Function parameter types.
 * The types of parameters a Function can handle.
 */
typedef enum ParameterType
{
    enmParameterInt = 0x44
    enmParameterUnsignedInt,
    enmParameterFloat
} ParameterType;

/**
 * A Function.
 * A Function accepts zero or more parameters and outputs one value.
 */
class Function
{
    public:
        Function(std::string sName_, ParameterType ParamType_, uint64_t cMinParams_,
            uint64_t cMaxParams_,  PFNFUNCTION pfnFunction_, std::string sShortDesc_, std::string sLongDesc_);
        virtual ~Function();

        /**
         * Returns a copy of the name of this Function.
         *
         * @return std::string
         */
        std::string         Name() const;

        /**
         * Returns the parameter type accepted by this Function.
         *
         * @return ParameterType
         */
        Paramater           ParameterType() const;

        /**
         * Returns the minimum number of parameters accepted by this Function.
         *
         * @return uint64_t
         */
        uint64_t            MinParams() const;

        /**
         * Returns the maximum number of parameters accepted by this Function.
         *
         * @return uint64_t
         */
        uint64_t            MaxParams() const;

        /**
         * Returns a copy of the short description of this Function.
         *
         * @return std::string
         */
        std::string         ShortDesc() const;

        /**
         * Returns a copy of the long description of this Function.
         *
         * @return std::string
         */
        std::string         LongDesc() const;

        /**
         * Invokes the function associated with this Function.
         *
         * @param apAtoms_      An array of pointers to Atoms.
         * @param cAtoms_       Number of elements in the array apAtoms_.
         *
         * @return int: xank error code..
         */
        int                 InvokeFunction(Atoms *apAtoms_[], uint64_t cAtoms_);

    private:
        std::string         m_sName;          /**< Name of the Function as seen in the expression. */
        uint64_t            m_cMinParams;     /**< Minimum parameters accepted by @a pfnFunctor. */
        uint64_t            m_cMaxParams;     /**< Maximum paramaters accepted by @a pfnFunctor. */
        ParameterType       m_ParamType;      /**< Whether this Function casts all it's parameters to an Integer. */
        PFNFUNCTION         m_pfnFunction;    /**< Pointer to the Function evaluator function. */
        std::string         m_sShortDesc;     /**< Short description of the Function (usually syntax). */
        std::string         m_sLongDesc;      /**< Long description of the Function (additional documentation). */
};

#endif /* XANK_FUNCTION_H */

