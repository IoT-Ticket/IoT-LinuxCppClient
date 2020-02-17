/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Wapice Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
 * to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */


#include "IOT_ReadData.h"
#include "IOT_Base64.h"

#include <iostream>
#include <sstream>

IOT_ReadData::IOT_ReadData(): m_dataType(IOTAPI::IOT_no_type)
{
}

IOT_ReadData::~IOT_ReadData()
{

}

std::string IOT_ReadData::GetName() const
{
    return m_name;
}

std::string IOT_ReadData::GetPath() const
{
    return m_path;
}

std::string IOT_ReadData::GetUnit() const
{
    return m_unit;
}

IOTAPI::IOT_DataType IOT_ReadData::GetDatatype() const
{
    return m_dataType;
}

size_t IOT_ReadData::ProcessValues()
{
    return m_processData.size();
}

bool IOT_ReadData::GetTimestamp(size_t index, unsigned long& ts) const
{
    if(index >= m_processData.size())
        return false;

    ts = m_processData.at(index).first;
    return true;
}

bool IOT_ReadData::GetConvertedValue(size_t index, long& value) const
{
    if(index >= m_processData.size() || m_dataType != IOTAPI::IOT_long)
        return false;

    std::stringstream ss(m_processData.at(index).second);
    ss >> value;

    return value;
}

bool IOT_ReadData::GetConvertedValue(size_t index, double& value) const
{
    if(index >= m_processData.size() || m_dataType != IOTAPI::IOT_double)
        return false;

    std::stringstream ss(m_processData.at(index).second);
    ss >> value;

    return value;
}

bool IOT_ReadData::GetConvertedValue(size_t index, bool& value) const
{
    if(index >= m_processData.size() || m_dataType != IOTAPI::IOT_bool)
        return false;

    if(m_processData.at(index).second == "true")
        value = true;
    else if(m_processData.at(index).second == "false")
        value = false;
    else
        return false;

    return true;
}

bool IOT_ReadData::GetConvertedValue(size_t index, std::vector<uint8_t>& decodedBytes) const
{
    if(index >= m_processData.size() || m_dataType != IOTAPI::IOT_binary)
        return false;

    return IOT_Base64::decode(m_processData.at(index).second, decodedBytes);
}

bool IOT_ReadData::GetConvertedValue(size_t index, std::string& value) const
{
    if(index >= m_processData.size())
        return false;

    value = m_processData.at(index).second;
    return true;
}

bool IOT_ReadData::FromJSON(const Json::Value& json)
{
    try {
        m_name = json["name"].asString();
        m_dataType = ConvertToDatatype( json["dataType"].asString()) ;
    } catch(...) {
        return false;
    }

    m_path = "";
    if(json.isMember("path") && json["path"].isString()) {
        m_path = json["path"].asString();
    }

    m_unit = "";
    if(json.isMember("unit") && json["unit"].isString()) {
        m_unit = json["unit"].asString();
    }

    if(json.isMember("values") && json["values"].isArray())
    {
        Json::Value::iterator it = json["values"].begin();
        while(it != json["values"].end())
        {
            try {
                process_value_t val;
                val.first = (*it)["ts"].asUInt64();
                val.second = (*it)["v"].asString();
                m_processData.push_back(val);
            } catch(...) {
                return false;
            }
            ++it;
        }
    }

    return true;
}


IOTAPI::IOT_DataType IOT_ReadData::ConvertToDatatype(const std::string& type)
{
    if(type == "double")
        return IOTAPI::IOT_double;
    if(type == "long")
        return IOTAPI::IOT_long;
    if(type == "string")
        return IOTAPI::IOT_string;
    if(type == "boolean")
        return IOTAPI::IOT_bool;
    if(type == "binary")
        return IOTAPI::IOT_binary;

    return IOTAPI::IOT_no_type;
}

