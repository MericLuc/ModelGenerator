/*!
 * @file   Field_id.cpp
 * @brief  Implementations of the functions defined in \a Field_id.h
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
#include "Field_id.h"

namespace ModGen {

using namespace std;

////////////////////////////////////////////////////////////////////////
Field_id::Field_id(): Field()
{}

////////////////////////////////////////////////////////////////////////
Field_id::~Field_id()
{}

////////////////////////////////////////////////////////////////////////
Field_id::Field_id(const Field_id& p_other): Field(p_other)
{}

////////////////////////////////////////////////////////////////////////
bool Field_id::isValid() const
{
    return ( !name.empty()   &&
             pos   != -1     &&
             size  != -1     &&
             endianness != _UNDEFINED
           );
}

////////////////////////////////////////////////////////////////////////
string Field_id::getDesc() const
{
    return (
               "Field_id "           + name                      +
               "\n\t-pos         = " + to_string(pos)            +
               "\n\t-size        = " + to_string(size)           +
               "\n\t-endianness  = " + endianString[endianness]  +
               "\n\t-swap        = " + (swap ? "true":"false")   +
               "\n\t-invert      = " + (invert ? "true":"false") + "\n"
           );
}

////////////////////////////////////////////////////////////////////////
void Field_id::addToMessage(vector<uint8_t>& p_mesg)
{
    // TODO: Voir avec Anthony la méthode de calcul de l'ID numérique
    Field::addToMessage(p_mesg);
}

} // namespace ModGen