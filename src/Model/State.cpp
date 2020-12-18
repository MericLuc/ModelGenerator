/*!
 * @file   State.cpp
 * @brief  Implementations of the functions defined in \a State.h
 * @author lhm
 * @date   17/07/2019
 */

#include "Logger.h"
#include "State.h"
#include "Exception.h"
#include "Conf_format.h"
#include "Message.h"
#include "time_util.h"
#include "Model.h"
#include <thread>

namespace ModGen {

using namespace std;

map<VarConditionTransition::OPERANDE, string> VarConditionTransition::operandeString = {
    {OPERANDE::OP_OVER,   "> (over)"   },
    {OPERANDE::OP_EQUAL,  "== (equal)" },
    {OPERANDE::OP_UNDER,  "< (under)"  },
    {OPERANDE::UNKNOWN,   "UNDEFINED"  }
};

map<Operation::OPERANDE, string> Operation::operandeString = {
    {OPERANDE::OP_ADD,      "+   (Addition)"     },
    {OPERANDE::OP_SUB,      "-   (Substraction)" },
    {OPERANDE::OP_DEL,      "del (Suppression)"  },
    {OPERANDE::OP_ASSIGN,   "=   (Asignement)"   },
    {OPERANDE::OP_UNKNOWN,  "UNKNOWN"            }
};

////////////////////////////////////////////////////////////////////////
Operation::Operation():
    var_name(),
    var(nullptr),
    operande(OP_UNKNOWN),
    value(-1)
{}

////////////////////////////////////////////////////////////////////////
Operation::~Operation()
{}

////////////////////////////////////////////////////////////////////////
void Operation::run()
{
    if(!var)
    {
        throw Exception::UnimplementedElement<Operation>(var_name);
    }

    switch (operande)
    {
    case OP_ADD:
        *var += value;
        break;
    case OP_SUB:
        *var -= value;
        break;
    case OP_DEL:
        delete var;
        var = nullptr;
        break;
    case OP_ASSIGN:
        *var = value;
        break;
    default:
        throw Exception::UnimplementedElement<OPERANDE>(operande);
    }
}

////////////////////////////////////////////////////////////////////////
bool Operation::isValid() const
{
    return ( !var_name.empty()      &&
             var                    &&
             operande != OP_UNKNOWN);
}

////////////////////////////////////////////////////////////////////////
string Operation::getDesc() const
{
    return("\t\t" + var_name              +
           " " + operandeString[operande] +
           " " + to_string(value)         + "\n");
}

////////////////////////////////////////////////////////////////////////
void Operation::setParam(const string& p_name, const string& p_value)
{
    if     (p_name.compare(StateOp_format::var)      == 0)
    {
        var_name    = p_value;
    }
    else if(p_name.compare(StateOp_format::operande) == 0)
    {
        if(p_value.compare("+") == 0)
        {
            operande = OP_ADD;
        }
        else if(p_value.compare("-") == 0)
        {
            operande = OP_SUB;
        }
        else if(p_value.compare("del") == 0)
        {
            operande = OP_DEL;
        }
        else if(p_value.compare("=") == 0)
        {
            operande = OP_ASSIGN;
        }
        else
        {
            throw Exception::UnimplementedElement<OPERANDE>(p_value);
        }
    }
    else if(p_name.compare(StateOp_format::value)    == 0)
    {
        value = static_cast<int32_t>(stoi(p_value));
    }
    else
    {
        throw Exception::ParsingFileParamError<Operation>(p_name);
    }
}

////////////////////////////////////////////////////////////////////////
LoopTransition::LoopTransition():
    delay(-1),
    times(-1),
    cur_cmpt(0)
{
    dest_state = nullptr;
    dest_name  = "";
}

////////////////////////////////////////////////////////////////////////
Transition::~Transition()
{}

////////////////////////////////////////////////////////////////////////
LoopTransition::~LoopTransition()
{}

////////////////////////////////////////////////////////////////////////
bool LoopTransition::isValid() const
{
    return (//dest_state &&
            !dest_name.empty() &&
            delay >= 0 &&
            times >  0);
}

////////////////////////////////////////////////////////////////////////
string LoopTransition::getDesc() const
{
    return("\t\tLoop      -> " + dest_name        +
           " " + to_string(times) + " times"      +
           " (delay " + to_string(delay)          + " us)\n");
}

////////////////////////////////////////////////////////////////////////
void LoopTransition::setParam(const string &p_name,
                              const string &p_value)
{
    if     (p_name.compare(StateTransitionLoop_format::times) == 0)
    {
        times    = static_cast<int32_t>(stoi(p_value));
    }
    else if(p_name.compare(StateTransitionLoop_format::delay) == 0)
    {
        delay    = static_cast<int32_t>(stoi(p_value));
    }
    else if(p_name.compare(StateTransition_format::dest)      == 0)
    {
        dest_name = p_value;
    }
    else
    {
        throw Exception::ParsingFileParamError<LoopTransition>(p_name);
    }
}

////////////////////////////////////////////////////////////////////////
bool LoopTransition::run()
{
    // Par défaut, la boucle est vérifiée
    bool l_return = true;
    cur_cmpt ++;

    // Si on arrive a la fin de la boucle, on n'effectue pas d'iteration supplémentaire
    if( cur_cmpt == times )
    {
        cur_cmpt = 0;
        l_return = false;
    }
    else
    {
        std::this_thread::sleep_for(std::chrono::microseconds(delay));
    }

    return l_return;
}

////////////////////////////////////////////////////////////////////////
VarConditionTransition::VarConditionTransition():
    var(nullptr),
    var_name(),
    value(-1),
    operande(UNKNOWN),
    defaut(false)
{
    dest_state = nullptr;
    dest_name  = "";
}

////////////////////////////////////////////////////////////////////////
VarConditionTransition::~VarConditionTransition()
{}

////////////////////////////////////////////////////////////////////////
bool VarConditionTransition::isValid() const
{
    return (
            !dest_name.empty() &&
            //var               && // will be checked by Model::checkIntegrity
            !var_name.empty() &&
            operande != UNKNOWN);
}

////////////////////////////////////////////////////////////////////////
string VarConditionTransition::getDesc() const
{
    return("\t\tVarCond   -> "    + dest_name                        +
           " (test " + var_name   + " "   + operandeString[operande] +
           " " + to_string(value) + ")\n");
}

////////////////////////////////////////////////////////////////////////
void VarConditionTransition::setParam(const string &p_name,
                                      const string &p_value)
{
    if     (p_name.compare(StateTransitionCondVar_format::name)     == 0)
    {
        var_name    = p_value;
    }
    else if(p_name.compare(StateTransitionCondVar_format::operande) == 0)
    {
        if(p_value.compare(">") == 0)
        {
            operande = OP_OVER;
        }
        else if(p_value.compare("<") == 0)
        {
            operande = OP_UNDER;
        }
        else if(p_value.compare("==") == 0)
        {
            operande = OP_EQUAL;
        }
        else
        {
            throw Exception::UnimplementedElement<OPERANDE>(p_value);
        }
    }
    else if(p_name.compare(StateTransitionCondVar_format::value)    == 0)
    {
        if(p_value.compare("DEFAULT") == 0)
        {
            defaut = true;
            value  = 1;
        }
        else
        {
            value = static_cast<int32_t>(stoi(p_value));
        }
    }
    else if(p_name.compare(StateTransition_format::dest)            == 0)
    {
        dest_name = p_value;
    }
    else
    {
        throw Exception::ParsingFileParamError<VarConditionTransition>(p_name);
    }
}

////////////////////////////////////////////////////////////////////////
bool VarConditionTransition::run()
{
    // Always execute a default condition
    if(defaut)
    {
        return true;
    }

    if(!var)
    {
        throw Exception::IntegrityCheckException<VarConditionTransition>("The var condition is pointing to a null model variable");
    }

    switch(operande)
    {
    case OP_OVER:  return (*var > value);
    case OP_UNDER: return (*var < value);
    case OP_EQUAL: return (*var == value);
    default:
        throw Exception::UnimplementedElement<OPERANDE>(operande);
    }
}

////////////////////////////////////////////////////////////////////////
DelayConditionTransition::DelayConditionTransition():
    delay_value(-1)
{
    dest_state = nullptr;
    dest_name = "";
}

////////////////////////////////////////////////////////////////////////
DelayConditionTransition::~DelayConditionTransition()
{}

////////////////////////////////////////////////////////////////////////
bool DelayConditionTransition::isValid() const
{
    return (//dest_state &&
            !dest_name.empty() &&
            delay_value >= 0);
}

////////////////////////////////////////////////////////////////////////
string DelayConditionTransition::getDesc() const
{
    return("\t\tDelayCond -> "    + dest_name              +
           " (delay " + to_string(delay_value) + " us)\n");
}

////////////////////////////////////////////////////////////////////////
void DelayConditionTransition::setParam(const string &p_name,
                                        const string &p_value)
{
    if     (p_name.compare(StateTransitionCondDelay_format::value)      == 0)
    {
        delay_value    = static_cast<int32_t>(stoi(p_value));
    }
    else if(p_name.compare(StateTransition_format::dest)            == 0)
    {
        dest_name = p_value;
    }
    else
    {
        throw Exception::ParsingFileParamError<DelayConditionTransition>(p_name);
    }
}

////////////////////////////////////////////////////////////////////////
bool DelayConditionTransition::run()
{
    std::this_thread::sleep_for(std::chrono::microseconds(delay_value));
    return true;
}

////////////////////////////////////////////////////////////////////////
State::State() :
    name()
{}

////////////////////////////////////////////////////////////////////////
State::~State()
{}

////////////////////////////////////////////////////////////////////////
bool State::isValid() const
{
    // TODO
    return ( !name.empty() ); //&&
             // !transitions.empty() );
}

////////////////////////////////////////////////////////////////////////
void State::setParam(const string& p_name, const string& p_value)
{
    if(p_name.compare(State_format::name) == 0)
    {
        name    = p_value;
    }
    else
    {
        throw Exception::ParsingFileParamError<State>(p_name);
    }
}

////////////////////////////////////////////////////////////////////////
string State::getDesc() const
{
    string l_return = "State " + name + "\n";

    if(!operations.empty())
    {
        l_return += "\tOperations:\n";
        for(auto& i_op : operations)
        {
            l_return += i_op.getDesc();
        }
    }

    if(!messages.empty())
    {
        l_return += "\tMessages:\n";
        for(auto& i_msg : messages)
        {
            if(i_msg)
            {
                l_return += "\t\tID - " + i_msg->getId() +"\n";
            }
        }
    }

    if(!transitions.empty())
    {
        l_return += "\tTransitions:\n";
        for(auto& i_trans : transitions)
        {
            l_return += i_trans->getDesc();
        }
    }

    return (l_return);
}

////////////////////////////////////////////////////////////////////////
vector<vector<uint8_t> > State::getMessages(void)
{
    // Build the message(s) to send
    vector< vector<uint8_t> > l_mesgToSend(0);
    for(auto& l_mesg: messages)
    {
        if(!l_mesg)
        {
            ERROR("The Message which ID's " + name + " references a null Message.");
            throw Exception::IntegrityCheckException<Message>("The Message which ID's " + name + " references a null Message.");
        }
        l_mesgToSend.push_back(l_mesg->createMessage());
    }

    return l_mesgToSend;
}

////////////////////////////////////////////////////////////////////////
vector< string >   State::getMessagesSrcIP(void)
{
    vector< string > l_return;
    for(auto& l_mesg: messages)
    {
        if(!l_mesg)
        {
            ERROR("The Message which ID's " + name + " references a null Message.");
            throw Exception::IntegrityCheckException<Message>("The Message which ID's " + name + " references a null Message.");
        }
        l_return.push_back(l_mesg->getSrcIP());
    }
    return l_return;
}

////////////////////////////////////////////////////////////////////////
vector< string >   State::getMessagesDstIP(void)
{
    vector< string > l_return;
    for(auto& l_mesg: messages)
    {
        if(!l_mesg)
        {
            ERROR("The Message which ID's " + name + " references a null Message.");
            throw Exception::IntegrityCheckException<Message>("The Message which ID's " + name + " references a null Message.");
        }
        l_return.push_back(l_mesg->getDstIP());
    }
    return l_return;
}

////////////////////////////////////////////////////////////////////////
vector< string >   State::getMessagesIntface(void)
{
    vector< string > l_return;
    for(auto& l_mesg: messages)
    {
        if(!l_mesg)
        {
            ERROR("The Message which ID's " + name + " references a null Message.");
            throw Exception::IntegrityCheckException<Message>("The Message which ID's " + name + " references a null Message.");
        }
        l_return.push_back(l_mesg->getIntface());
    }
    return l_return;
}

////////////////////////////////////////////////////////////////////////
vector< uint32_t > State::getMessagesSrcPort(void)
{
    vector< uint32_t > l_return;
    for(auto& l_mesg: messages)
    {
        if(!l_mesg)
        {
            ERROR("The Message which ID's " + name + " references a null Message.");
            throw Exception::IntegrityCheckException<Message>("The Message which ID's " + name + " references a null Message.");
        }
        l_return.push_back(l_mesg->getSrcPort());
    }
    return l_return;
}

////////////////////////////////////////////////////////////////////////
vector< uint32_t > State::getMessagesDstPort(void)
{
    vector< uint32_t > l_return;
    for(auto& l_mesg: messages)
    {
        if(!l_mesg)
        {
            ERROR("The Message which ID's " + name + " references a null Message.");
            throw Exception::IntegrityCheckException<Message>("The Message which ID's " + name + " references a null Message.");
        }
        l_return.push_back(l_mesg->getDstPort());
    }
    return l_return;
}

////////////////////////////////////////////////////////////////////////
void State::runOperations()
{
    // Run every operation associated to the state
    for(auto& l_operations: operations)
    {
        l_operations.run();
    }
}

////////////////////////////////////////////////////////////////////////
void State::runTransitions()
{
    for(auto& l_trans: transitions)
    {
        if(!l_trans)
        {
            ERROR("The State which ID's " + name + " references a null Transition.");
            throw Exception::IntegrityCheckException<State>("The State which ID's " + name + " references a null Transition.");
        }

        if(l_trans->run())
        {
            Model::setCurrState(Model::getNextState());
            Model::setNextState(l_trans->getDestState());
            return;
        }
    }

}

} // namespace ModGen