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
        friend class            Settings;
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

        /**
         * Get an unsigned 64-bit value for a key.
         *
         * @param sKey          The key.
         * @param pu64Val       Where to store the found value.
         *
         * @return bool: true if found, false otherwise.
         */
        bool                    GetUInt64(std::string sKey, uint64_t *pu64Val);

        /**
         * Get an unsigned 32-bit value for a key.
         *
         * @param sKey          The key.
         * @param pu32Val       Where to store the found value.
         *
         * @return bool: true if found, false otherwise.
         */
        bool                    GetUInt32(std::string sKey, uint32_t *pu32Val);

        /**
         * Get an unsigned 16-bit value for a key.
         *
         * @param sKey          The key.
         * @param pu16Val       Where to store the found value.
         *
         * @return bool: true if found, false otherwise.
         */
        bool                    GetUInt16(std::string sKey, uint16_t *pu16Val);

        /**
         * Get an unsigned 8-bit value for a key.
         *
         * @param sKey          The key.
         * @param pu8Val        Where to store the found value.
         *
         * @return bool: true if found, false otherwise.
         */
        bool                    GetUInt8(std::string sKey, uint8_t *pu8Val);

        /**
         * Get a signed 64-bit value for a key.
         *
         * @param sKey          The key.
         * @param pi64Val       Where to store the found value.
         *
         * @return bool: true if found, false otherwise.
         */
        bool                    GetInt64(std::string sKey, int64_t *pi64Val);

        /**
         * Get a signed 32-bit value for a key.
         *
         * @param sKey          The key.
         * @param pi32Val       Where to store the found value.
         *
         * @return bool: true if found, false otherwise.
         */
        bool                    GetInt32(std::string sKey, int32_t *pi32Val);

        /**
         * Get a signed 16-bit value for a key.
         *
         * @param sKey          The key.
         * @param pi16Val       Where to store the found value.
         *
         * @return bool: true if found, false otherwise.
         */
        bool                    GetInt16(std::string sKey, int16_t *pi16Val);

        /**
         * Get a signed 8-bit value for a key.
         *
         * @param sKey          The key.
         * @param pi8Val        Where to store the found value.
         *
         * @return bool: true if found, false otherwise.
         */
        bool                    GetInt8(std::string sKey, int8_t *pi8Val);

        /**
         * Get a boolean value for a key.
         *
         * @param sKey          The key.
         * @param pfVal         Where to store the found value.
         *
         * @return bool: true if found, false otherwise.
         */
        bool                    GetBool(std::string sKey, bool *pfVal);

        /**
         * Get a string value for a key.
         *
         * @param sKey          The key.
         * @param psVal        Where to store the found value.
         *
         * @return bool: true if found, false otherwise.
         */
        bool                    GetString(std::string sKey, std::string *psVal);

        /**
         * Get a floating point value for a key.
         *
         * @param sKey          The key.
         * @param pgVal         Where to store the found value.
         *
         * @return bool: true if found, false otherwise.
         */
        bool                    GetFloat(std::string sKey, float *pgVal);

        /**
         * Get an unsigned 64-bit value for a key, if the key is not found get a
         * supplied default value.
         *
         * @param sKey          The key.
         * @param pu64Val       Where to store the found value.
         * @param u64Def        The default value to store if the key isn't found.
         */
        void                    GetUInt64Def(std::string sKey, uint64_t *pu64Val, uint64_t u64Def);

        /**
         * Get an unsigned 32-bit value for a key, if the key is not found get a
         * supplied default value.
         *
         * @param sKey          The key.
         * @param pu32Val       Where to store the found value.
         * @param u32Def        The default value to store if the key isn't found.
         */
        void                    GetUInt32Def(std::string sKey, uint32_t *pu32Val, uint32_t u32Def);

        /**
         * Get an unsigned 16-bit value for a key, if the key is not found get a
         * supplied default value.
         *
         * @param sKey          The key.
         * @param pu16Val       Where to store the found value.
         * @param u16ef         The default value to store if the key isn't found.
         */
        void                    GetUInt16Def(std::string sKey, uint16_t *pu16Val, uint16_t u16Def);

        /**
         * Get an unsigned 8-bit value for a key, if the key is not found get a
         * supplied default value.
         *
         * @param sKey          The key.
         * @param pu8Val        Where to store the found value.
         * @param u8Def         The default value to store if the key isn't found.
         */
        void                    GetUInt8Def(std::string sKey, uint8_t *pu8Val, uint8_t u8Def);

        /**
         * Get a signed 64-bit value for a key, if the key is not found get a supplied
         * default value.
         *
         * @param sKey          The key.
         * @param pi64Val       Where to store the found value.
         * @param i64Def        The default value to store if the key isn't found.
         */
        void                    GetInt64Def(std::string sKey, int64_t *pi64Val, int64_t i64Def);

        /**
         * Get a signed 32-bit value for a key, if the key is not found get a supplied
         * default value.
         *
         * @param sKey          The key.
         * @param pi32Val       Where to store the found value.
         * @param i32Def        The default value to store if the key isn't found.
         */
        void                    GetInt32Def(std::string sKey, int32_t *pi32Val, int32_t i32Def);

        /**
         * Get a signed 16-bit value for a key, if the key is not found get a supplied
         * default value.
         *
         * @param sKey          The key.
         * @param p16Val        Where to store the found value.
         * @param i16Def        The default value to store if the key isn't found.
         */
        void                    GetInt16Def(std::string sKey, int16_t *pi16Val, int16_t i16Def);

        /**
         * Get a signed 8-bit value for a key, if the key is not found get a supplied
         * default value.
         *
         * @param sKey          The key.
         * @param pi8Val        Where to store the found value.
         * @param i8Def         The default value to store if the key isn't found.
         */
        void                    GetInt8Def(std::string sKey, int8_t *pi8Val, int8_t i8Def);

        /**
         * Get a boolean value for a key, if the key is not found get a supplied default
         * value.
         *
         * @param sKey          The key.
         * @param pfVal         Where to store the found value.
         * @param fDef          The default value to store if the key isn't found.
         */
        void                    GetBoolDef(std::string sKey, bool *pfVal, bool fDef);

        /**
         * Get a string value for a key, if the key is not found get a supplied default
         * value.
         *
         * @param sKey          The key.
         * @param psVal         Where to store the found value.
         * @param sDef          The default value to store if the key isn't found.
         */
        void                    GetStringDef(std::string sKey, std::string *psVal, std::string sDef);

        /**
         * Get a floating point value for a key, if the key is not found get a supplied
         * default value.
         *
         * @param sKey          The key.
         * @param pgVal         Where to store the found value.
         * @param gDef          The default value to store if the key isn't found.
         */
        void                    GetFloatDef(std::string sKey, float *pgVal, float gDef);

    private:
        std::string             m_sFilePath;            /**< Path to a settings file. */

        typedef std::pair<std::string, SettingsValue> KeyValue;
        std::map<std::string, SettingsValue> m_Map;     /**< Map of all key-value pairs in this Settings. */
};

#endif /* XANK_SETTINGS_H */

