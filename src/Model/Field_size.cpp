/*!
 * @file   Field_size.cpp
 * @brief  Implementations of the functions defined in \a Field_size.h
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
#include "Field_size.h"

namespace ModGen {

using namespace std;

map<Field_size::FIELD_FORMAT, string> Field_size::formatString = {
    {FIELD_FORMAT::SIZE_FORMAT_U8,     "SIZE_FORMAT_U8"     },
    {FIELD_FORMAT::SIZE_FORMAT_U16,    "SIZE_FORMAT_U16"    },
    {FIELD_FORMAT::SIZE_FORMAT_U32,    "SIZE_FORMAT_U32"    },
    {FIELD_FORMAT::SIZE_FORMAT_UNDEF,  "UNDEFINED"          }
};

map<Field_size::FIELD_SIZE_PART, string> Field_size::partString = {
    {FIELD_SIZE_PART::SIZE_EXCLUDING_HEADER,    "SIZE_EXCLUDING_HEADER" },
    {FIELD_SIZE_PART::SIZE_INCLUDING_HEADER,    "SIZE_INCLUDING_HEADER" },
    {FIELD_SIZE_PART::SIZE_PART_UNDEF,          "UNDEFINED"             }
};

////////////////////////////////////////////////////////////////////////
Field_size::Field_size(const Field_size& p_other):
    Field(p_other),
    format(p_other.format),
    size_part(p_other.size_part)
{}

////////////////////////////////////////////////////////////////////////
Field_size::Field_size():
    Field(),
    format(SIZE_FORMAT_UNDEF),
    size_part(SIZE_PART_UNDEF),
    mesg_data_size(0),
    header_size(0)
{}

////////////////////////////////////////////////////////////////////////
Field_size& Field_size::operator = (const Field_size &p_other)
{
    name            = p_other.name;
    pos             = p_other.pos;
    size            = p_other.size;
    value           = p_other.value;
    endianness      = p_other.endianness;
    swap            = p_other.swap;
    invert          = p_other.invert;
    format          = p_other.format;
    size_part       = p_other.size_part;
    mesg_data_size  = p_other.mesg_data_size;
    header_size     = p_other.header_size;

  return *this;
}

////////////////////////////////////////////////////////////////////////
Field_size::~Field_size()
{}

////////////////////////////////////////////////////////////////////////
bool Field_size::isValid() const
{
    return ( !name.empty()                   &&
             pos        != -1                &&
             size       != -1                &&
             endianness != _UNDEFINED         &&
             format     != SIZE_FORMAT_UNDEF &&
             size_part  != SIZE_PART_UNDEF );
}

////////////////////////////////////////////////////////////////////////
void Field_size::setParam(const string& p_name, 
                          const string& p_value)
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
    else if(p_name.compare(Field_format::value)  == 0)
    {
        value = static_cast<int32_t>(stoi(p_value));
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
        if(p_value.compare("SIZE_FORMAT_U8") == 0)
        {
            format = SIZE_FORMAT_U8;
        }
        else if(p_value.compare("SIZE_FORMAT_U16") == 0)
        {
            format = SIZE_FORMAT_U16;
        }
        else if(p_value.compare("SIZE_FORMAT_U32") == 0)
        {
            format = SIZE_FORMAT_U32;
        }
        else
        {
            throw Exception::UnimplementedElement<FIELD_FORMAT>(p_value);
        }
    }
    else if(p_name.compare(Field_format::part)   == 0)
    {
        if(p_value.compare("SIZE_EXCLUDING_HEADER") == 0)
        {
            size_part = SIZE_EXCLUDING_HEADER;
        }
        else if(p_value.compare("SIZE_INCLUDING_HEADER") == 0)
        {
            size_part = SIZE_INCLUDING_HEADER;
        }
        else
        {
            throw Exception::UnimplementedElement<FIELD_SIZE_PART>(p_value);
        }
    }
    else
    {
        throw Exception::ParsingFileParamError<Field>(p_name);
    }
}

////////////////////////////////////////////////////////////////////////
string Field_size::getDesc() const
{
    return ( "Field_size "         + name                      +
             "\n\t-pos         = " + to_string(pos)            +
             "\n\t-size        = " + to_string(size)           +
             "\n\t-endianness  = " + endianString[endianness]  +
             "\n\t-swap        = " + (swap ? "true":"false")   +
             "\n\t-invert      = " + (invert ? "true":"false") +
             "\n\t-format      = " + formatString[format] +
             "\n\t-part        = " + partString[size_part] + "\n" );
}

////////////////////////////////////////////////////////////////////////
void Field_size::addToMessage(vector<uint8_t>& p_mesg)
{
    uint32_t l_mesgSize = 8 * mesg_data_size;

    switch(size_part)
    {
        case SIZE_EXCLUDING_HEADER:
            break;
        case SIZE_INCLUDING_HEADER:
            // size in bits of the header
            // NB: The header will fill the last incomplete byte if needed
            // DEBUG("*****header_size = " + std::to_string(header_size));
            l_mesgSize += 8 * (ceil(header_size/8.f));
            // DEBUG("*****l_mesgSize = " + std::to_string(l_mesgSize));
            break;
        default:
            throw Exception::UnimplementedElement<FIELD_SIZE_PART>(SIZE_PART_UNDEF);
    }

    switch(format)
    {
        case SIZE_FORMAT_U8:
            value = static_cast<int32_t>(ceil(l_mesgSize/8.f));
            // DEBUG("*****value = " + std::to_string(value));
            break;
        case SIZE_FORMAT_U16:
            value =  static_cast<int32_t>(ceil(l_mesgSize/16.f));
            break;
        case SIZE_FORMAT_U32:
            value =  static_cast<int32_t>(ceil(l_mesgSize/32.f));
            break;
        default:
            throw Exception::UnimplementedElement<FIELD_FORMAT>(SIZE_FORMAT_UNDEF);
    }

    Field::addToMessage(p_mesg);
}

////////////////////////////////////////////////////////////////////////
void Field_size::mesgDataSize(const uint32_t& p_sizeValue)
{
    mesg_data_size = p_sizeValue;
}

////////////////////////////////////////////////////////////////////////
void Field_size::setHeaderSize(uint32_t& p_headerSize)
{
    header_size = p_headerSize;
}

} // namespace ModGen