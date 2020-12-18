/*!
 * @file   Field.cpp
 * @brief  Implementations of the functions defined in \a Field.h
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
#include "Field.h"
#include "Field_size.h"
#include "Field_time.h"
#include "Field_id.h"

namespace ModGen {

using namespace std;

map<Field::_ENDIAN, string> Field::endianString = {
    {_ENDIAN::_LITTLE_ENDIAN,   "LITTLE_ENDIAN"   },
    {_ENDIAN::_BIG_ENDIAN,      "BIG_ENDIAN"      },
    {_ENDIAN::_UNDEFINED,       "UNDEFINED"       }
};

////////////////////////////////////////////////////////////////////////
vector<uint8_t> FieldHelper::int32ToChar(int32_t &p_int)
{
     vector<uint8_t> l_return(4);
     for (int i = 0; i < 4; i++)
         l_return[3 - i] = (p_int >> (i * 8));
     return l_return;
}

////////////////////////////////////////////////////////////////////////
uint32_t FieldHelper::getMSB(int64_t p_int)
{
    uint32_t l_return = 0;
    if(p_int != 0)
    {
        while (p_int != 0)
        {
            p_int /= 2;
            l_return++;
        }
    }
    return l_return;
}

////////////////////////////////////////////////////////////////////////
void FieldHelper::addBin(uint8_t          p_value,
                         uint32_t         p_pos,
                         vector<uint8_t>& p_mesg)
{
    if( p_pos <= 8*p_mesg.size()-1 )
    {
        p_mesg[p_pos/8] |= (p_value << (7 - p_pos%8));
    }
}

////////////////////////////////////////////////////////////////////////
void FieldHelper::printBits(vector<uint8_t>& p_mesg, 
                            const string&    p_optTitle)
{
    string l_return;
    for(uint32_t i = 0; i < p_mesg.size(); i++)
    {
        l_return += std::bitset<8>(p_mesg[i]).to_string() + " ";
    }
    DEBUG(p_optTitle + l_return);
}

////////////////////////////////////////////////////////////////////////
template <typename T> T FieldHelper::swap_endian(T p_value)
{
    static_assert (CHAR_BIT == 8, "CHAR_BIT != 8");

    union
    {
        T val;
        uint8_t u8[sizeof(T)];
    } source, dest;

    source.val = p_value;

    for (size_t k = 0; k < sizeof(T); k++)
    {
        dest.u8[k] = source.u8[sizeof(T) - k - 1];
    }

    return dest.val;
}

////////////////////////////////////////////////////////////////////////
Field* Field_Creator::Create(const string& p_name)
{
    if     (p_name.compare(Field_format::balise)      == 0)
    {
        // DEBUG("Creating new simple Field");
        return new Field;
    }
    else if(p_name.compare(Field_size_format::balise) == 0)
    {
        // DEBUG("Creating new size Field");
        return new Field_size;
    }
    else if(p_name.compare(Field_time_format::balise) == 0)
    {
        // DEBUG("Creating new time Field");
        return new Field_time;
    }
    else if(p_name.compare(Field_id_format::balise)   == 0)
    {
        // DEBUG("Creating new id Field");
        return new Field_id;
    }

    throw Exception::ParsingFileParamError<Header>(p_name);
}

////////////////////////////////////////////////////////////////////////
Field::Field():
    name(),
    pos(-1),
    size(-1),
    value(-1),
    endianness(_ENDIAN::_UNDEFINED),
    swap(false),
    invert(false)
{}

////////////////////////////////////////////////////////////////////////
Field::Field(const Field& p_other) :
    name(p_other.name),
    pos(p_other.pos),
    size(p_other.size),
    value(p_other.value),
    endianness(p_other.endianness),
    swap(p_other.swap),
    invert(p_other.invert)
{}

////////////////////////////////////////////////////////////////////////
Field& Field::operator = (const Field &p_other)
{
    name       = p_other.name;
    pos        = p_other.pos;
    size       = p_other.size;
    value      = p_other.value;
    endianness = p_other.endianness;
    swap       = p_other.swap;
    invert     = p_other.invert;

  return *this;
}

////////////////////////////////////////////////////////////////////////
Field::~Field()
{}

////////////////////////////////////////////////////////////////////////
bool Field::isValid() const
{
    return ( !name.empty()   &&
             pos   != -1     &&
             size  != -1     &&
             value != -1     &&
             endianness != _UNDEFINED
           );
}

////////////////////////////////////////////////////////////////////////
void Field::setParam(const string& p_name, 
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
    else
    {
        throw Exception::ParsingFileParamError<Field>(p_name);
    }
}

////////////////////////////////////////////////////////////////////////
string Field::getDesc() const
{
    return (
               "Field "              + name                      +
               "\n\t-pos         = " + to_string(pos)            +
               "\n\t-size        = " + to_string(size)           +
               "\n\t-value       = " + to_string(value)          +
               "\n\t-endianness  = " + endianString[endianness]  +
               "\n\t-swap        = " + (swap ? "true":"false")   +
               "\n\t-invert      = " + (invert ? "true":"false") + "\n"
           );
}

////////////////////////////////////////////////////////////////////////
void Field::addToMessage(vector<uint8_t>& p_mesg)
{
    // Change endianness if needed
    switch(endianness)
    {
        case _BIG_ENDIAN:
        value = FieldHelper::swap_endian<int64_t>(value);
        break;
        case _LITTLE_ENDIAN:
        break;
        default:
        throw Exception::UnimplementedElement<_ENDIAN>(_UNDEFINED);
    }

    // Swap the value by 16-bit words if needed
    if(swap)
    {
        value = (value & 0x0000FFFF) << 16 | (value & 0xFFFF0000) >> 16;
    }

    // vector<uint8_t> l_mesgBytes = FieldHelper::int32ToChar(value);
    uint32_t          l_msbValue  = FieldHelper::getMSB(value);
    int                   l_startInd  = static_cast<int>(l_msbValue);
    int                   l_inv       = -1;

    //FieldHelper::printBits(l_mesgBytes, "Value of the field " + name + ": ");

    // Requiered size is over the specified value bit size representation
    if( static_cast<uint32_t>(size) > l_msbValue )
    {
        if(Logger::isDebugEnabled())
        {
        DEBUG("Field ID " + name + ": The specified size (" + to_string(size)
             + " bits) is over the value (" + to_string(value)
             + ") bits representation number ("+  to_string(l_msbValue) + " bits)" + "- the value has been filled "
                                                                                     "with 0s to fit the specified size" );
        }

        l_startInd = size;
    }
    // Requiered size is under the specified value bit size representation (troncate)
    else if (static_cast<uint32_t>(size) < l_msbValue )
    {
        if(Logger::isDebugEnabled())
        {
        DEBUG("Field ID " + name + ": The specified size (" + to_string(size)
             + " bits) is below the value ("+ to_string(value)
             + ") bits representation number ("+  to_string(l_msbValue) + " bits) - the value will be troncated.");
        }
        l_startInd = size;
    }

    // Extend the message if necessary
    uint32_t l_endPos = 1 + static_cast<uint32_t>(pos+size-1)/8;
    while(p_mesg.size() < l_endPos)
    {
        p_mesg.push_back(0);
    }

    // Change bits order if "invert"
    if(invert)
    {
        l_startInd = -1;
        l_inv      = 1;
    }

    // Fill the bits with the specified value
    for(uint32_t i = 0; i < static_cast<uint32_t>(size); i++)
    {
        uint8_t l_currBit = (value >> (l_startInd + l_inv*(i + 1)) ) & 1;
        FieldHelper::addBin(l_currBit,
                            static_cast<uint32_t>(pos)+i,
                            p_mesg);
    }
}

////////////////////////////////////////////////////////////////////////
void Field::updateHeaderSize(uint32_t& p_headerSize)
{
    if(p_headerSize < static_cast<uint32_t>(pos + size) )
    {
        p_headerSize = static_cast<uint32_t>(pos + size);
    }
}

} // namespace ModGen