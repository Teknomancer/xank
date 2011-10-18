/** @file
 * xank - Settings, implementation.
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

#include "Settings.h"
#include "XErrors.h"

SettingsValue::SettingsValue()
    : m_ParamType(enmSettingsValueParamTypeEmpty)
{
}

SettingsValue::SettingsValue(const char *pcszVal)
{
    m_ParamType = enmSettingsValueParamTypeString;
    m_sVal = pcszVal;
}

SettingsValue::SettingsValue(std::string sVal)
{
    m_ParamType = enmSettingsValueParamTypeString;
    m_sVal = sVal;
}

SettingsValue::SettingsValue(uint64_t u64Val)
{
    m_ParamType = enmSettingsValueParamTypeUInt64;
    m_u.u64Val = u64Val;
}

SettingsValue::SettingsValue(uint32_t u32Val)
{
    m_ParamType = enmSettingsValueParamTypeUInt32;
    m_u.u32Val = u32Val;
}

SettingsValue::SettingsValue(uint16_t u16Val)
{
    m_ParamType = enmSettingsValueParamTypeUInt16;
    m_u.u16Val = u16Val;
}

SettingsValue::SettingsValue(uint8_t u8Val)
{
    m_ParamType = enmSettingsValueParamTypeUInt8;
    m_u.u8Val = u8Val;
}

SettingsValue::SettingsValue(int64_t i64Val)
{
    m_ParamType = enmSettingsValueParamTypeInt64;
    m_u.i64Val = i64Val;
}

SettingsValue::SettingsValue(int32_t i64Val)
{
    m_ParamType = enmSettingsValueParamTypeInt64;
    m_u.i64Val = i64Val;
}

SettingsValue::SettingsValue(int16_t i32Val)
{
    m_ParamType = enmSettingsValueParamTypeInt32;
    m_u.i32Val = i32Val;
}

SettingsValue::SettingsValue(int8_t i16Val)
{
    m_ParamType = enmSettingsValueParamTypeInt16;
    m_u.i16Val = i16Val;
}

SettingsValue::SettingsValue(bool fVal)
{
    m_ParamType = enmSettingsValueParamTypeBool;
    m_u.fVal = fVal;
}

SettingsValue::SettingsValue(float gVal)
{
    m_ParamType = enmSettingsValueParamTypeFloat;
    m_u.fVal = gVal;
}

SettingsValue::~SettingsValue()
{
}

SettingsValueParamType SettingsValue::Type() const
{
        return m_ParamType;
}


/* ============================================================================================================================ */

Settings::Settings()
{
}


Settings::~Settings()
{
}

#define SETTINGS_SETTYPE(typedesc, type) \
int Settings::Set##typedesc(std::string sKey, type Val) \
{ \
    m_Map.insert(KeyValue(sKey, SettingsValue(Val))); \
    return INF_SUCCESS; \
} \

SETTINGS_SETTYPE(UInt64, uint64_t)
SETTINGS_SETTYPE(UInt32, uint32_t)
SETTINGS_SETTYPE(UInt16, uint16_t)
SETTINGS_SETTYPE(UInt8, uint8_t)
SETTINGS_SETTYPE(Int64, int64_t)
SETTINGS_SETTYPE(Int32, int32_t)
SETTINGS_SETTYPE(Int16, int16_t)
SETTINGS_SETTYPE(Int8, int8_t)
SETTINGS_SETTYPE(Bool, bool)
SETTINGS_SETTYPE(String, std::string)
SETTINGS_SETTYPE(Float, float)
#undef SETTINGS_SETTYPE

#define SETTINGS_GETTYPE(typedesc, type, svalfield) \
bool Settings::Get##typedesc(std::string sKey, type *pVal) \
{ \
    std::map<std::string, SettingsValue>::iterator iterMap = m_Map.find(sKey); \
    if (iterMap == m_Map.end()) \
        return false; \
    *pVal = iterMap->second.m_u.svalfield; \
    return true; \
} \

SETTINGS_GETTYPE(UInt64, uint64_t, u64Val)
SETTINGS_GETTYPE(UInt32, uint32_t, u32Val)
SETTINGS_GETTYPE(UInt16, uint16_t, u16Val)
SETTINGS_GETTYPE(UInt8, uint8_t, u8Val)
SETTINGS_GETTYPE(Int64, int64_t, i64Val)
SETTINGS_GETTYPE(Int32, int32_t, i32Val)
SETTINGS_GETTYPE(Int16, int16_t, i16Val)
SETTINGS_GETTYPE(Int8, int8_t, i8Val)
SETTINGS_GETTYPE(Bool, bool, fVal)
SETTINGS_GETTYPE(Float, float, gVal)
#undef SETTINGS_GETTYPE

#define SETTINGS_GETTYPE_DEF(typedesc, type, svalfield) \
void Settings::Get##typedesc##Def(std::string sKey, type *pVal, type Val) \
{ \
    std::map<std::string, SettingsValue>::iterator iterMap = m_Map.find(sKey); \
    if (iterMap == m_Map.end()) \
        *pVal = Val; \
    else \
        *pVal = iterMap->second.m_u.svalfield; \
} \

SETTINGS_GETTYPE_DEF(UInt64, uint64_t, u64Val)
SETTINGS_GETTYPE_DEF(UInt32, uint32_t, u32Val)
SETTINGS_GETTYPE_DEF(UInt16, uint16_t, u16Val)
SETTINGS_GETTYPE_DEF(UInt8, uint8_t, u8Val)
SETTINGS_GETTYPE_DEF(Int64, int64_t, i64Val)
SETTINGS_GETTYPE_DEF(Int32, int32_t, i32Val)
SETTINGS_GETTYPE_DEF(Int16, int16_t, i16Val)
SETTINGS_GETTYPE_DEF(Int8, int8_t, i8Val)
SETTINGS_GETTYPE_DEF(Bool, bool, fVal)
SETTINGS_GETTYPE_DEF(Float, float, gVal)
#undef SETTINGS_GETTYPE_DEF

//void Settings::GetUInt64Def(std::string sKey, uint64_t *pu64Val, uint64_t u64Val)
//{
//    std::map<std::string, SettingsValue>::iterator iterMap = m_Map.find(sKey);
//    if (iterMap == m_Map.end())
//        *pu64Val = u64Val;
//    else
//        *pu64Val = iterMap->second.m_u.u64Val;
//}

//
//bool Settings::GetUInt64(std::string sKey, uint64_t *pu64Val)
//{
//    std::map<std::string, SettingsValue>::iterator iterMap = m_Map.find(sKey);
//    if (iterMap == m_Map.end())
//        return false;
//    *pu64Val = iterMap->second.m_u.u64Val;
//    return true;
//}
//

