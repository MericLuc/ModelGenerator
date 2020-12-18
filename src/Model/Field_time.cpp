/*!
 * @file   Field_time.cpp
 * @brief  Implementations of the functions defined in \a Field_time.h
 * @author lhm
 * @date   29/08/2019
 */

#include <bitset>
#include <cmath>
#include <limits.h>

#include "Logger.h"
#include "Header.h"
#include "Exception.h"
#include "Conf_format.h"
#include "time_util.h"
#include "Field_time.h"

namespace ModGen {

using namespace std;

map<Field_time::TIME_FORMAT, string> Field_time::timeString = {
    {TIME_FORMAT::TIME_UNDEFINED,    "UNDEFINED"    },
    {TIME_FORMAT::MICROSECONDS,      "MICROSECONDS" },
    {TIME_FORMAT::MILLISECONDS,      "MILLISECONDS" },
    {TIME_FORMAT::HHR_DIANE,         "HHR_DIANE"    }
};

////////////////////////////////////////////////////////////////////////
Field_time::Field_time():
    Field(),
    format(TIME_UNDEFINED)
{}

////////////////////////////////////////////////////////////////////////
Field_time::~Field_time()
{}

////////////////////////////////////////////////////////////////////////
Field_time::Field_time(const Field_time& p_other):
    Field(p_other),
    format(p_other.format)
{}

////////////////////////////////////////////////////////////////////////
Field_time& Field_time::operator = (const Field_time &p_other)
{
    name       = p_other.name;
    pos        = p_other.pos;
    size       = p_other.size;
    value      = p_other.value;
    endianness = p_other.endianness;
    swap       = p_other.swap;
    invert     = p_other.invert;
    format     = p_other.format;

  return *this;
}

////////////////////////////////////////////////////////////////////////
string Field_time::getDesc() const
{
    return (
               "Field_time "         + name                      +
               "\n\t-pos         = " + to_string(pos)            +
               "\n\t-size        = " + to_string(size)           +
               "\n\t-format      = " + timeString[format]        +
               "\n\t-endianness  = " + endianString[endianness]  +
               "\n\t-swap        = " + (swap ? "true":"false")   +
               "\n\t-invert      = " + (invert ? "true":"false") + "\n"
           );
}

////////////////////////////////////////////////////////////////////////
bool Field_time::isValid() const
{
    return ( !name.empty()               &&
             pos        != -1            &&
             size       != -1            &&
             endianness != _UNDEFINED    &&
             format     != TIME_UNDEFINED
           );
}

////////////////////////////////////////////////////////////////////////
void Field_time::setParam(const string& p_name, const string& p_value)
{
    if     (p_name.compare(Field_format::name)   == 0)
    {
        name    = p_value;
    }
    else if(p_name.compare(Field_format::pos)    == 0)
    {
        pos  = static_cast<int32_t>(stoi(p_value));
    }
    else if(p_name.compare(Field_format::size)   == 0)
    {
        size = static_cast<int32_t>(stoi(p_value));
    }
    else if(p_name.compare(Field_format::endian) == 0)
    {
        if(p_value.compare("LE") == 0)
        {
            endianness = _LITTLE_ENDIAN;
        }
        else if(p_value.compare("BE") == 0)
        {
            endianness = _BIG_ENDIAN;
        }
        else
        {
            throw Exception::UnimplementedElement<_ENDIAN>(p_value);
        }
    }
    else if(p_name.compare(Field_format::swap)   == 0)
    {
        if(p_value.compare("TRUE") == 0)
        {
            swap = true;
        }
        else if(p_value.compare("FALSE") == 0)
        {
            swap = false;
        }
        else
        {
            throw Exception::UnimplementedElement<bool>(p_value);
        }
    }
    else if(p_name.compare(Field_format::invert) == 0)
    {
        if(p_value.compare("TRUE") == 0)
        {
            invert = true;
        }
        else if(p_value.compare("FALSE") == 0)
        {
            invert = false;
        }
        else
        {
            throw Exception::UnimplementedElement<bool>(p_value);
        }
    }
    else if(p_name.compare(Field_format::format) == 0)
    {
        if     (p_value.compare("MILLISECONDS") == 0)
        {
            format = MILLISECONDS;
        }
        else if(p_value.compare("MICROSECONDS") == 0)
        {
            format = MICROSECONDS;
        }
        else if(p_value.compare("HHR_DIANE")    == 0)
        {
            format = HHR_DIANE;
        }
        else
        {
            throw Exception::UnimplementedElement<TIME_FORMAT>(p_value);
        }
    }
    else
    {
        throw Exception::ParsingFileParamError<Header>(p_name);
    }
}

////////////////////////////////////////////////////////////////////////
void Field_time::addToMessage(vector<uint8_t>& p_mesg)
{
    switch(format)
    {
        case MILLISECONDS:
            value = static_cast<int64_t>(TimeUtil::day_milliseconds());
            break;
        case MICROSECONDS:
            value = static_cast<int64_t>(TimeUtil::day_microseconds());
            break;
        case HHR_DIANE:
            value = static_cast<int64_t>(TimeUtil::hhr_diane());
            break;
        default:
            throw Exception::UnimplementedElement<TIME_FORMAT>(_UNDEFINED);
    }

    Field::addToMessage(p_mesg);
}

} // namespace ModGen