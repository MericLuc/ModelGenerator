/*!
 * @file   Conf_format.cpp
 * @brief  Implementations of the functions defined in \a Conf_format.h
 * @author lhm
 * @date   16/07/2019
 */

#include "Conf_format.h"

namespace ModGen {

using namespace std;

const string Conf_format::root          = "Conf";

const string Variables_format::balise   = "Variables";
const string Variables_format::name     = "name";
const string Variables_format::init     = "init";

const string Messages_format::balise    = "Messages";
const string Messages_format::name      = "name";
const string Messages_format::header    = "header";
const string Messages_format::size      = "size";
const string Messages_format::src_ip    = "ip_src";
const string Messages_format::dst_ip    = "ip_dst";
const string Messages_format::src_port  = "port_src";
const string Messages_format::dst_port  = "port_dst";
const string Messages_format::fill      = "fill";
const string Messages_format::interface = "interface";

const string Headers_format::balise     = "Headers";
const string Headers_format::name       = "name";

const string Field_format::balise       = "Field";
const string Field_format::name         = "name";
const string Field_format::pos          = "pos";
const string Field_format::size         = "size";
const string Field_format::value        = "value";
const string Field_format::endian       = "endianness";
const string Field_format::swap         = "swap";
const string Field_format::invert       = "invert";
const string Field_format::part         = "part";
const string Field_format::format       = "format";

const string Field_id_format::balise    = "Field_id";

const string Field_size_format::balise  = "Field_size";

const string Field_time_format::balise  = "Field_time";

const string State_format::balise       = "States";
const string State_format::balise_2     = "State";
const string State_format::name         = "name";

const string StateOp_format::balise     = "Operations";
const string StateOp_format::balise_2   = "Op";
const string StateOp_format::var        = "var";
const string StateOp_format::operande   = "operande";
const string StateOp_format::value      = "value";

const string StateMessage_format::balise   = "State_messages";
const string StateMessage_format::balise_2 = "State_mesg";
const string StateMessage_format::name     = "name";

const string StateTransition_format::balise   = "Transitions";
const string StateTransition_format::balise_2 = "Transit";
const string StateTransition_format::dest     = "dest_state";

const string StateTransitionLoop_format::balise   = "Loop";
const string StateTransitionLoop_format::times    = "times";
const string StateTransitionLoop_format::delay    = "delay";

const string StateTransitionCondVar_format::balise   = "Condition";
const string StateTransitionCondVar_format::name     = "name";
const string StateTransitionCondVar_format::value    = "value";
const string StateTransitionCondVar_format::operande = "operande";

const string StateTransitionCondDelay_format::balise   = "Delay";
const string StateTransitionCondDelay_format::value    = "value";

} // namespace ModGen