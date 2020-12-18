/*!
 * @file   Message.cpp
 * @brief  Implementations of the functions defined in \a Message.h
 * @author lhm
 * @date   17/07/2019
 */

#include <stdlib.h>
#include <time.h>

#include "Logger.h"
#include "Message.h"
#include "Exception.h"
#include "Conf_format.h"
#include "Header.h"

namespace ModGen {

using namespace std;

map<Message::FILL_METHOD, string> Message::fillString = {
    {FILL_METHOD::MESG_FILL_ZERO,   "MESG_FILL_ZERO"   },
    {FILL_METHOD::MESG_FILL_RANDOM, "MESG_FILL_RANDOM" },
    {FILL_METHOD::MESG_FILL_UNSET,  "MESG_FILL_UNSET"  }
};

////////////////////////////////////////////////////////////////////////
Message::Message() :
    name(),
    header(),
    header_ptr(nullptr),
    data_size(0),
    src_port(0),
    dst_port(0),
    src_ip(),
    dst_ip(),
    interface(),
    fill(MESG_FILL_UNSET),
    mesgToSend(vector<uint8_t>())
{
    srand (static_cast<unsigned int>(time(nullptr)));
}

////////////////////////////////////////////////////////////////////////
Message::~Message()
{}

////////////////////////////////////////////////////////////////////////
bool Message::isValid() const
{
    return ( !name.empty()   &&
             !header.empty() &&
             data_size       &&
             src_port        &&
             dst_port        &&
             !src_ip.empty() &&
             !dst_ip.empty() &&
             fill != MESG_FILL_UNSET
           );
}

////////////////////////////////////////////////////////////////////////
void Message::setParam(const string& p_name, const string& p_value)
{
    if(p_name.compare(Messages_format::name) == 0)
    {
        name    = p_value;
    }
    else if(p_name.compare(Messages_format::header) == 0)
    {
        header  = p_value;
    }
    else if(p_name.compare(Messages_format::size) == 0)
    {
        data_size = static_cast<uint32_t>(stoi(p_value));
    }
    else if(p_name.compare(Messages_format::src_ip) == 0)
    {
        src_ip = p_value;
    }
    else if(p_name.compare(Messages_format::dst_ip) == 0)
    {
        dst_ip = p_value;
    }
    else if(p_name.compare(Messages_format::src_port) == 0)
    {
        src_port = static_cast<uint32_t>(stoi(p_value));
    }
    else if(p_name.compare(Messages_format::dst_port) == 0)
    {
        dst_port = static_cast<uint32_t>(stoi(p_value));
    }
    else if(p_name.compare(Messages_format::fill) == 0)
    {
        if(p_value.compare("MESG_FILL_ZERO") == 0)
        {
            fill = MESG_FILL_ZERO;
        }
        else if(p_value.compare("MESG_FILL_RANDOM") == 0)
        {
            fill = MESG_FILL_RANDOM;
        }
        else
        {
            throw Exception::UnimplementedElement<FILL_METHOD>(p_value);
        }
    }
    else if(p_name.compare(Messages_format::interface) == 0)
    {
        interface = p_value;
    }
    else
    {
        throw Exception::ParsingFileParamError<Message>(p_name);
    }
}

////////////////////////////////////////////////////////////////////////
string Message::getDesc() const
{
    return (
               "Message "           + name                  +
               "\n\t-header    = " + header                 +
               "\n\t-interface = " + interface              +
               "\n\t-data_size = " + to_string(data_size)   +
               "\n\t-src port  = " + to_string(src_port)    +
               "\n\t-dst port  = " + to_string(dst_port)    +
               "\n\t-src ip    = " + src_ip                 +
               "\n\t-dst ip    = " + dst_ip                 +
               "\n\t-fill meth = " + fillString[fill] + "\n"
           );
}

////////////////////////////////////////////////////////////////////////
vector<uint8_t>& Message::createMessage()
{
    mesgToSend.clear();
    if(!header_ptr)
    {
        ERROR("Error - The message which ID is " + name + " references a null Header.");
        throw Exception::IntegrityCheckException<Header>("The message which ID is " + name + " references a null Header." );
    }
    header_ptr->mesgSizeParams(data_size);
    header_ptr->addToMessage(mesgToSend);

    switch(fill)
    {
        case MESG_FILL_ZERO:
            for(uint32_t i = 0; i < data_size; i++) { mesgToSend.push_back(0);                                        }
            break;
        case MESG_FILL_RANDOM:
            for(uint32_t i = 0; i < data_size; i++) { mesgToSend.push_back(static_cast<uint8_t>(rand() % 255)); }
            break;
        default:
            ERROR("Error - createMessage Could not parse the filling method.");
            throw Exception::UnimplementedElement<FILL_METHOD>(fill);
    }

    //FieldHelper::printBits(mesgToSend, "Message " + name + ": ");

    return mesgToSend;
}

} // namespace ModGen