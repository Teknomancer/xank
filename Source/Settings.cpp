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


Settings::Settings()
{
}

Settings::~Settings()
{
}


int Settings::SetUInt64(std::string sKey, uint64_t u64Val)
{
    m_Map.insert(KeyValue(sKey, SettingsValue(u64Val)));
    return INF_SUCCESS;
}

int Settings::SetUInt32(std::string sKey, uint32_t u32Val)
{
    m_Map.insert(KeyValue(sKey, SettingsValue(u32Val)));
    return INF_SUCCESS;
}

int Settings::SetUInt16(std::string sKey, uint16_t u16Val)
{
    m_Map.insert(KeyValue(sKey, SettingsValue(u16Val)));
    return INF_SUCCESS;
}

int Settings::SetUInt8(std::string sKey, uint8_t u8Val)
{
    m_Map.insert(KeyValue(sKey, SettingsValue(u8Val)));
    return INF_SUCCESS;
}

int Settings::SetInt64(std::string sKey, int64_t i64Val)
{
    m_Map.insert(KeyValue(sKey, SettingsValue(i64Val)));
    return INF_SUCCESS;
}

int Settings::SetInt32(std::string sKey, int32_t i32Val)
{
    m_Map.insert(KeyValue(sKey, SettingsValue(i32Val)));
    return INF_SUCCESS;
}

int Settings::SetInt16(std::string sKey, int16_t i16Val)
{
    m_Map.insert(KeyValue(sKey, SettingsValue(i16Val)));
    return INF_SUCCESS;
}

int Settings::SetInt8(std::string sKey, int8_t i8Val)
{
    m_Map.insert(KeyValue(sKey, SettingsValue(i8Val)));
    return INF_SUCCESS;
}

int Settings::SetBool(std::string sKey, bool fVal)
{
    m_Map.insert(KeyValue(sKey, SettingsValue(fVal)));
    return INF_SUCCESS;
}

int Settings::SetString(std::string sKey, std::string sVal)
{
    m_Map.insert(KeyValue(sKey, SettingsValue(sVal)));
    return INF_SUCCESS;
}

int Settings::SetFloat(std::string sKey, float gVal)
{
    m_Map.insert(KeyValue(sKey, SettingsValue(gVal)));
    return INF_SUCCESS;
}

