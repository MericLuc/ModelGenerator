/*!
 * @file   Header.cpp
 * @brief  Implementations of the functions defined in \a Header.h
 * @author lhm
 * @date   17/07/2019
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

namespace ModGen {

using namespace std;

////////////////////////////////////////////////////////////////////////
Header::Header() :
    name(),
    fields(map<string, Field*>())
{}

////////////////////////////////////////////////////////////////////////
Header::~Header()
{}

////////////////////////////////////////////////////////////////////////
bool Header::isValid() const
{
    return ( !name.empty() &&
             !fields.empty() );
}

////////////////////////////////////////////////////////////////////////
void Header::setParam(const string& p_name, 
                      const string& p_value)
{
    if(p_name.compare(Messages_format::name) == 0)
    {
        name    = p_value;
    }
    else
    {
        throw Exception::ParsingFileParamError<Header>(p_name);
    }
}

////////////////////////////////////////////////////////////////////////
void Header::addField(Field *p_field)
{
    if( fields.find(p_field->getId()) != fields.end() )
    {
        throw Exception::ParsingFileError("A header with ID (" + p_field->getId() + ") already exists");
    }
    fields[p_field->getId()] = p_field;
}

////////////////////////////////////////////////////////////////////////
string Header::getDesc() const
{
    string l_return = "\tHeader " + name + "\n";
    for(auto& it : fields)
    {
        l_return += it.second->getDesc();
    }

    return (l_return);
}

////////////////////////////////////////////////////////////////////////
void Header::addToMessage(vector<uint8_t>& p_mesg) const
{
    for(auto& l_fieldpair: fields)
    {
        if(!l_fieldpair.second)
        {
            ERROR("The header which ID is " + name + " references a null Field.");
            throw Exception::IntegrityCheckException<Field>("The header which ID is " + name + " references a null Field." );
        }
        l_fieldpair.second->addToMessage(p_mesg);
    }
}

////////////////////////////////////////////////////////////////////////
void Header::mesgSizeParams(const uint32_t& p_sizeValue)
{
    uint32_t l_headerSize = 0;
    for(auto& l_fieldpair: fields)
    {
        if(!l_fieldpair.second)
        {
            ERROR("The header which ID is " + name + " references a null Field.");
            throw Exception::IntegrityCheckException<Field>("The header which ID is " + name + " references a null Field." );
        }
        l_fieldpair.second->mesgDataSize(p_sizeValue);
        l_fieldpair.second->updateHeaderSize(l_headerSize);
    }

    for(auto& l_fieldpair: fields)
    {
        if(!l_fieldpair.second)
        {
            ERROR("The header which ID is " + name + " references a null Field.");
            throw Exception::IntegrityCheckException<Field>("The header which ID is " + name + " references a null Field." );
        }
        l_fieldpair.second->setHeaderSize(l_headerSize);
    }

    //ERROR("HEADER SIZE = " + std::to_string(l_headerSize));
}

} // namespace ModGen