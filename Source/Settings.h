/** @file
 * xank - Settings, header.
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

#ifndef XANK_SETTINGS_H
# define XANK_SETTINGS_H

#include <stdint.h>
#include <map>
#include <string>

/**
 * The type of the settings parameter.
 * The settings parameter has to be one of the following distinct types.
 */
enum SettingsValueParamType
{
    /** Empty type  */
    enmSettingsValueParamTypeEmpty = 0xE,
    /** Unsigned 64-bit integer */
    enmSettingsValueParamTypeUInt64,
    /** Unsigned 32-bit integer  */
    enmSettingsValueParamTypeUInt32,
    /** Unsigned 16-bit integer */
    enmSettingsValueParamTypeUInt16,
    /** Unsigned 8-bit integer */
    enmSettingsValueParamTypeUInt8,
    /** Signed 64-bit integer  */
    enmSettingsValueParamTypeInt64,
    /** Signed 32-bit integer */
    enmSettingsValueParamTypeInt32,
    /** Signed 16-bit integer */
    enmSettingsValueParamTypeInt16,
    /** Signed 8-bit integer  */
    enmSettingsValueParamTypeInt8,
    /** Boolean value  */
    enmSettingsValueParamTypeBool,
    /** String value */
    enmSettingsValueParamTypeString,
    /** Float value */
    enmSettingsValueParamTypeFloat
};

/**
 * The value object of a settings key-value pair.
 * The value object stored with each settings key.
 */
class SettingsValue
{
    public:
        SettingsValue();
        explicit SettingsValue(const char *pcszVal);
        explicit SettingsValue(std::string sVal);
        explicit SettingsValue(uint64_t u64Val);
        explicit SettingsValue(uint32_t u64Val);
        explicit SettingsValue(uint16_t u64Val);
        explicit SettingsValue(uint8_t u64Val);
        explicit SettingsValue(int64_t i64Val);
        explicit SettingsValue(int32_t i64Val);
        explicit SettingsValue(int16_t i64Val);
        explicit SettingsValue(int8_t i64Val);
        explicit SettingsValue(bool fVal);
        explicit SettingsValue(float gVal);
        virtual ~SettingsValue();

        /**
         * Returns the type of this settings value.
         *
         * @return SettingsValueParamType
         */
        SettingsValueParamType      Type() const;

    private:
        SettingsValueParamType      m_ParamType;  /**< The type of this value. */
        union
        {
            uint64_t            u64Val;
            uint32_t            u32Val;
            uint16_t            u16Val;
            uint8_t             u8Val;
            int64_t             i64Val;
            int32_t             i32Val;
            int16_t             i16Val;
            int8_t              i8Val;
            bool                fVal;
            float               gVal;
        } m_u;                                    /**< The type of this Settings Value. */
        std::string             m_sVal;           /**< String value, cannot be part of union (non-trivial ctor) */
};


/**
 * A settings object.
 * A setting object is capable of loading, saving and updating a map of
 * key-value pairs that are used for settings.
 */
class Settings
{
    public:
        Settings();
        virtual ~Settings();

        /**
         * Set a 64-bit unsigned value for a key.
         *
         * @param sKey          The key.
         * @param u64Val        The value.
         *
         * @return int: xank error code.
         */
        int                     SetUInt64(std::string sKey, uint64_t u64Val);

        /**
         * Set a 32-bit unsigned value for a key.
         *
         * @param sKey          The key.
         * @param u32Val        The value.
         *
         * @return int: xank error code.
         */
        int                     SetUInt32(std::string sKey, uint32_t u32Val);

        /**
         * Set a 16-bit unsigned value for a key.
         *
         * @param sKey          The key.
         * @param u16Val        The value.
         *
         * @return int: xank error code.
         */
        int                     SetUInt16(std::string sKey, uint16_t u16Val);

        /**
         * Set an 8-bit unsigned value for a key.
         *
         * @param sKey          The key.
         * @param u8Val         The value.
         *
         * @return int: xank error code.
         */
        int                     SetUInt8(std::string sKey, uint8_t u8Val);

        /**
         * Set a signed 64-bit value for a key.
         *
         * @param sKey          The key.
         * @param i64Val        The value.
         *
         * @return int: xank error code.
         */
        int                     SetInt64(std::string sKey, int64_t i64Val);

        /**
         * Set a signed 32-bit value for a key.
         *
         * @param sKey          The key.
         * @param i32Val        The value.
         *
         * @return int: xank error code.
         */
        int                     SetInt32(std::string sKey, int32_t i32Val);

        /**
         * Set a signed 16-bit value for a key.
         *
         * @param sKey          The key.
         * @param i16Val        The value.
         *
         * @return int: xank error code.
         */
        int                     SetInt16(std::string sKey, int16_t i16Val);

        /**
         * Set a signed 8-bit value for a key.
         *
         * @param sKey          The key.
         * @param i8Val         The value.
         *
         * @return int: xank error code.
         */
        int                     SetInt8(std::string sKey, int8_t i8Val);

        /**
         * Set a boolean value for a key.
         *
         * @param sKey          The key.
         * @param fVal          The value.
         *
         * @return int: xank error code.
         */
        int                     SetBool(std::string sKey, bool fVal);

        /**
         * Set a string value for a key.
         *
         * @param sKey          The key.
         * @param sVal          The value.
         *
         * @return int: xank error code.
         */
        int                     SetString(std::string sKey, std::string sVal);

        /**
         * Set a floating point value for a key.
         *
         * @param sKey          The key.
         * @param gVal          The value.
         *
         * @return int: xank error code.
         */
        int                     SetFloat(std::string sKey, float gVal);

    private:
        typedef std::pair<std::string, SettingsValue> KeyValue;
        std::map<std::string, SettingsValue> m_Map;     /**< Map of all key-value pairs in this Settings. */
};

#endif /* XANK_SETTINGS_H */

