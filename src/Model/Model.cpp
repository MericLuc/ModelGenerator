/*!
 * @file   Model.cpp
 * @brief  Implementations of the functions defined in \a Model.h
 * @author lhm
 * @date   16/07/2019
 */

#include <errno.h>
#include <limits>

#include "Logger.h"
#include "Model.h"
#include "Exception.h"
#include "Conf_format.h"
#include "Message.h"
#include "Header.h"
#include "State.h"
#include "Field.h"

namespace ModGen {

using namespace std;

////////////////////////////////////////////////////////////////////////
bool parsingHelper::isEqual(const char* p_1, const char* p_2)
{
    return (strncmp(p_1, p_2, MAX_NAME_SIZE) == 0);
}

map<Model::MODELSTATE, string> Model::stateString = {
    {Model::NOT_INITIALIZED, "Model not initialized"},
    {Model::INITIALIZED,     "Model initialized"    },
    {Model::RUNNING,         "Model running"        },
    {Model::STOPPED,         "Model stopped"        }
};

////////////////////////////////////////////////////////////////////////
Model::Model() :
    confFile(nullptr),
    curState(nullptr),
    nexState(nullptr),
    currentStateStr(NOT_INITIALIZED),
    modelVar(map<string,int>()),
    modelMes(map<string, Message>()),
    modelHead(map<string, Header>())
{}

////////////////////////////////////////////////////////////////////////
Model::~Model()
{
    if(confFile)
    {
        delete confFile;
    }
}

////////////////////////////////////////////////////////////////////////
State* Model::getNextState()
{
    if(!getInstance().nexState)
    {
        ERROR("Error - Unable to run the model - no valid state found.");
        throw Exception::IntegrityCheckException<Model>("Unable to run the model - no valid state found.");
    }

    return getInstance().nexState;
}

////////////////////////////////////////////////////////////////////////
State* Model::getCurrState()
{
    if(!getInstance().curState)
    {
        ERROR("Error - Unable to run the model - no valid state found.");
        throw Exception::IntegrityCheckException<Model>("Unable to run the model - no valid state found.");
    }

    return getInstance().curState;
}

////////////////////////////////////////////////////////////////////////
Model& Model::getInstance()
{
    static Model l_instance;
    return l_instance;
}

////////////////////////////////////////////////////////////////////////
const string& Model::getStateString()
{
    return stateString[getInstance().currentStateStr];
}

////////////////////////////////////////////////////////////////////////
const map<string, int>& Model::getVariables()
{
    return getInstance().modelVar;
}

////////////////////////////////////////////////////////////////////////
const map<string, Message>& Model::getMessages()
{
    return getInstance().modelMes;
}

////////////////////////////////////////////////////////////////////////
const map<string, Header>& Model::getHeaders()
{
    return getInstance().modelHead;
}

////////////////////////////////////////////////////////////////////////
const map<string, State>& Model::getStates()
{
    return getInstance().modelState;
}

////////////////////////////////////////////////////////////////////////
void Model::log()
{
    string l_return = "\n\t\t------ VARIABLES ------\n";
    for(auto& t : Model::getVariables() )
    {
        l_return += t.first + " " + to_string(t.second);
    }
    l_return += "\n\t\t------ MESSAGES ------\n";

    for(auto& t : Model::getMessages() )
    {
        l_return += t.second.getDesc();
    }
    l_return += "\n\t\t------ HEADERS ------\n";

    for(auto& t : Model::getHeaders() )
    {
        l_return += t.second.getDesc();
    }

    l_return += "\n\t\t------ STATES ------\n";
    for(auto& t : Model::getStates() )
    {
        l_return += t.second.getDesc();
    }

    INFO(l_return);
}

////////////////////////////////////////////////////////////////////////
void Model::addVariable(const string& p_name, int p_val)
{
    getInstance().modelVar[p_name] = p_val;
}

////////////////////////////////////////////////////////////////////////
void Model::operateVariable(const string&    p_name,
                            const OPERATION& p_operation,
                            int              p_value)
{
    ModelVar &l_vars = getInstance().modelVar;
    if(l_vars.find(p_name) == l_vars.end())
    {
        ERROR("Error - Trying to operate undefined model variable (" + p_name + ").");
        throw Exception::ParsingFileError("Trying to operate undefined model variable (" + p_name + ").");
    }

    switch(p_operation)
    {
        case ADD: l_vars[p_name] += p_value; break;
        case SUB: l_vars[p_name] -= p_value; break;
        case DEL: l_vars.erase(p_name);      break;
        //default:
        //    throw Exception::UnimplementedElement<OPERATION>(p_operation);
    }
}

////////////////////////////////////////////////////////////////////////
void Model::initializeVariables(const pugi::xml_document& p_doc)
{
    pugi::xml_node tools = p_doc.child(Conf_format::root.c_str()).child(Variables_format::balise.c_str());
    for (pugi::xml_node tool = tools.first_child(); tool; tool = tool.next_sibling())
    {
        string l_currVar = "";
        int    l_currVal = -1;
        for (pugi::xml_attribute attr = tool.first_attribute(); attr; attr = attr.next_attribute())
        {
            if(!strncmp(attr.name(), Variables_format::name.c_str(), MAX_NAME_SIZE))
            {
                l_currVar = attr.value();
            }
            else if(!strncmp(attr.name(), Variables_format::init.c_str(), MAX_NAME_SIZE))
            {
                errno  = 0;
                char *ptr;
                l_currVal = strtol(attr.value(), &ptr, 10);
                if ((errno == ERANGE &&
                     (l_currVal == numeric_limits<long>::max() || l_currVal == numeric_limits<long>::min())) ||
                     (errno != 0 && l_currVal == 0) ||
                     ptr == attr.value())
                {
                    ERROR("Error - Invalid format for 'init' variable model parameter.");
                    throw Exception::ParsingFileError("Invalid value for variable initialization.");
                }
            }
            else
            {
                ERROR("Error - Invalid format of the variable model");
                throw Exception::ParsingFileParamError<Model>(string(attr.name()));
            }

            if(!l_currVar.empty() && l_currVal != -1)
            {
                getInstance().modelVar[l_currVar] = l_currVal;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////
void Model::initializeMessages(const pugi::xml_document& p_doc)
{
    pugi::xml_node tools = p_doc.child(Conf_format::root.c_str()).child(Messages_format::balise.c_str());
    for (pugi::xml_node tool = tools.first_child(); tool; tool = tool.next_sibling())
    {
        Message l_currentMesg;
        for (pugi::xml_attribute attr = tool.first_attribute(); attr; attr = attr.next_attribute())
        {
            l_currentMesg.setParam(attr.name(), attr.value());
        }

        // Message valide (tous les champs obligatoires sont renseignés)
        if(!l_currentMesg.isValid())
        {
            ERROR("Error - Unable to retrieve mandatory parameters for the current message.");
            throw Exception::ParsingFileError("Unable to retrive every mandatory parameters the for current Message.");
        }
        // Message en doublon (ID déjà existant)
        else if( getInstance().modelMes.find(l_currentMesg.getId()) != getInstance().modelMes.end() )
        {
            ERROR("Error - A message with ID (" + l_currentMesg.getId() + ") already exists.");
            throw Exception::ParsingFileError("A message with ID (" + l_currentMesg.getId() + ") already exists.");
        }
        // Ajouter le message à la liste
        getInstance().modelMes[l_currentMesg.getId()] = l_currentMesg;
        DEBUG("Added a new message (" + l_currentMesg.getId() + ") to the model.");
    }
}

////////////////////////////////////////////////////////////////////////
void Model::initializeHeaders(const pugi::xml_document& p_doc)
{
    pugi::xml_node tools = p_doc.child(Conf_format::root.c_str()).child(Headers_format::balise.c_str());
    // Parcours des headers
    for (pugi::xml_node tool = tools.first_child(); tool; tool = tool.next_sibling())
    {
        Header l_currHeader;
        // Paramètres du header courant
        for (pugi::xml_attribute attr = tool.first_attribute(); attr; attr = attr.next_attribute())
        {
            l_currHeader.setParam(attr.name(),attr.value());
        }

        // Fields du header courant
        for (pugi::xml_node fields = tool.first_child(); fields; fields = fields.next_sibling())
        {
            Field* l_currField = Field_Creator::Create(fields.name());

            // Paramètres du champ courant
            for (pugi::xml_attribute attr = fields.first_attribute(); attr; attr = attr.next_attribute())
            {
                l_currField->setParam(attr.name(), attr.value());
            }

            if(!l_currField->isValid())
            {
                ERROR("Error - Unable to retrieve mandatory parameters for the current Field (" + string(fields.name()) + ").");
                throw Exception::ParsingFileError("Unable to retrive every mandatory parameters for current Field (" + string(fields.name()) + ").");
            }

            l_currHeader.addField(l_currField);
        }

        if(!l_currHeader.isValid())
        {
            ERROR("Error - Unable to retrieve mandatory parameters for the current Header.");
            throw Exception::ParsingFileError("Unable to retrive every mandatory parameters for current Header.");
        }
        // Header en doublon (ID déjà existant)
        else if( getInstance().modelHead.find(l_currHeader.getId()) != getInstance().modelHead.end() )
        {
            ERROR("Error - A header with ID (" + l_currHeader.getId() + ") already exists");
            throw Exception::ParsingFileError("A header with ID (" + l_currHeader.getId() + ") already exists.");
        }

        getInstance().modelHead[l_currHeader.getId()] = l_currHeader;
        DEBUG("Added a new header (" + l_currHeader.getId() + ") to the model.");
    }
}

////////////////////////////////////////////////////////////////////////
void Model::initializeStates(const pugi::xml_document& p_doc)
{
    pugi::xml_node tools = p_doc.child(Conf_format::root.c_str()).child(State_format::balise.c_str());
    for (pugi::xml_node tool = tools.first_child(); tool; tool = tool.next_sibling())
    {
        State l_currentState;
        if( strncmp(tool.name(), State_format::balise_2.c_str(), MAX_NAME_SIZE) )
        {
            ERROR("Error - Unable to retrieve the state balise");
            throw Exception::ParsingFileBaliseError<State>(tool.name());
        }

        for (pugi::xml_attribute attr = tool.first_attribute(); attr; attr = attr.next_attribute())
        {
            l_currentState.setParam(attr.name(), attr.value());
        }

        // State valide (tous les champs obligatoires sont renseignés)
        if(!l_currentState.isValid())
        {
            ERROR("Error - Unable to retrieve the mandatory parameters for current state.");
            throw Exception::ParsingFileError("Unable to retrive every mandatory parameters for current State.");
        }
        // State en doublon (ID déjà existant)
        else if( getInstance().modelState.find(l_currentState.getId()) != getInstance().modelState.end() )
        {
            ERROR("Error - A state with ID (" + l_currentState.getId() + ") already exists." );
            throw Exception::ParsingFileError("A state with ID (" + l_currentState.getId() + ") already exists.");
        }
        // Ajouter le message à la liste
        getInstance().modelState[l_currentState.getId()] = l_currentState;
        DEBUG("Added a new State (" + l_currentState.getId() + ") to the model.");

        // Add the first parsed state as start state
        if(!getInstance().curState && !getInstance().nexState)
        {
            getInstance().curState = &getInstance().modelState[l_currentState.getId()];
            getInstance().nexState = &getInstance().modelState[l_currentState.getId()];
        }

        for (pugi::xml_node next_lvl = tool.first_child(); next_lvl; next_lvl = next_lvl.next_sibling())
        {
            if( parsingHelper::isEqual(next_lvl.name(), StateOp_format::balise.c_str()) )
            {
                // DEBUG("BOUCLAGE SUR LES OPERATIONS");
                Operation l_currentOp;
                for (pugi::xml_node trans = next_lvl.first_child(); trans; trans = trans.next_sibling())
                {
                    // Mauvaise balise (<Op> attendue)
                    if( !parsingHelper::isEqual(trans.name(), StateOp_format::balise_2.c_str()) )
                    {
                        throw Exception::ParsingFileBaliseError<Operation>(trans.name());
                    }

                    // Parcours des paramètres
                    for (pugi::xml_attribute trans_param = trans.first_attribute(); trans_param; trans_param = trans_param.next_attribute())
                    {
                        // On vérifie que la variable référencée existe
                        if( parsingHelper::isEqual(trans_param.name(), StateOp_format::var.c_str()) )
                        {
                            if( getInstance().modelVar.find(trans_param.value()) == getInstance().modelVar.end() )
                            {
                                throw Exception::UnimplementedElement<Variables_format>(trans_param.value());
                            }

                            l_currentOp.setVariable( getInstance().modelVar[trans_param.value()] );
                        }

                        l_currentOp.setParam(trans_param.name(), trans_param.value());
                    }
                }

                if(!l_currentOp.isValid())
                {
                    ERROR("Error - Not enough informations for operation initialization.");
                    throw Exception::ParsingFileError("Not enough informations for operation initialization.");
                }
                getInstance().modelState[l_currentState.getId()].addOperation(l_currentOp);
            }
            else if( parsingHelper::isEqual(next_lvl.name(), StateTransition_format::balise.c_str()) )
            {
                // DEBUG("BOUCLAGE SUR LES TRANSITIONS");
                for (pugi::xml_node trans = next_lvl.first_child(); trans; trans = trans.next_sibling())
                {
                    Transition *l_currentTrans = nullptr;
                    string      l_currentDestName;

                    // On a soit <Loop> soit <Transit>
                    // Balise <Loop>
                    if(parsingHelper::isEqual(trans.name(), StateTransitionLoop_format::balise.c_str()))
                    {
                        l_currentTrans = new LoopTransition();

                        l_currentTrans->setDestState(&getInstance().modelState[l_currentState.getId()]);
                        l_currentTrans->setParam(StateTransition_format::dest, l_currentState.getId());

                        // Parcours des paramètres
                        for (pugi::xml_attribute trans_param = trans.first_attribute(); trans_param; trans_param = trans_param.next_attribute())
                        {
                            l_currentTrans->setParam(trans_param.name(), trans_param.value());
                        }
                    }
                    // Balise <Transit>
                    else if(parsingHelper::isEqual(trans.name(), StateTransition_format::balise_2.c_str()))
                    {
                        //State* l_destState = nullptr;
                        for (pugi::xml_attribute transit_param = trans.first_attribute(); transit_param; transit_param = transit_param.next_attribute())
                        {
                            // Mauvais paramètre (dest_state attendu)
                            if( !parsingHelper::isEqual(transit_param.name(), StateTransition_format::dest.c_str()) )
                            {
                                throw Exception::ParsingFileBaliseError<StateTransition_format>(trans.name());
                            }
                            l_currentDestName = transit_param.value();
                        }

                        // On parcourt les enfants pour voir si c'est un Delay ou un Var
                        for (pugi::xml_node trans_3 = trans.first_child(); trans_3; trans_3 = trans_3.next_sibling())
                        {
                            // Var
                            if(parsingHelper::isEqual(trans_3.name(), StateTransitionCondVar_format::balise.c_str()))
                            {
                                l_currentTrans = new VarConditionTransition();
                            }
                            // Delay
                            else if(parsingHelper::isEqual(trans_3.name(), StateTransitionCondDelay_format::balise.c_str()))
                            {
                                l_currentTrans = new DelayConditionTransition();
                            }
                            else
                            {
                                throw Exception::UnimplementedElement<StateTransition_format>(trans_3.value());
                            }
                            //l_currentTrans->setDestState(l_destState);
                            l_currentTrans->setParam(StateTransition_format::dest, l_currentDestName);

                            for (pugi::xml_attribute param_3 = trans_3.first_attribute();
                                                     param_3; param_3 = param_3.next_attribute())
                            {
                                l_currentTrans->setParam(param_3.name(), param_3.value());
                            }
                        }

                    }
                    else
                    {
                        ERROR("Error - Could not correctly parse the current transition.");
                        throw Exception::ParsingFileBaliseError<StateTransition_format>(trans.name());
                    }

                    if(!l_currentTrans)
                    {
                        ERROR("Error - Unable to parse a Transition.");
                        throw Exception::ParsingFileError("Unable to parse a Transition.");
                    }
                    else if(!l_currentTrans->isValid())
                    {
                        // ICI
                        ERROR("Error - Not enough informations for operation initialization.");
                        throw Exception::ParsingFileError("Not enough informations for Transition initialization.");
                    }

                    getInstance().modelState[l_currentState.getId()].addTransition(*l_currentTrans);
                }
            }
            else if( parsingHelper::isEqual(next_lvl.name(), StateMessage_format::balise.c_str()) )
            {
                // DEBUG("BOUCLAGE SUR LES MESSAGES");
                for (pugi::xml_node mesg = next_lvl.first_child(); mesg; mesg = mesg.next_sibling())
                {
                    // Mauvaise balise (<State_Mesg> attendue)
                    if( !parsingHelper::isEqual(mesg.name(), StateMessage_format::balise_2.c_str()) )
                    {
                        throw Exception::ParsingFileBaliseError<StateMessage_format>(mesg.name());
                    }

                    // Parcours des paramètres
                    for (pugi::xml_attribute mesg_param = mesg.first_attribute(); mesg_param; mesg_param = mesg_param.next_attribute())
                    {
                        // Pas le bon paramètre ("name" attendu)
                        if( !parsingHelper::isEqual(mesg_param.name(), StateMessage_format::name.c_str()) )
                        {
                            throw Exception::ParsingFileParamError<StateMessage_format>(mesg_param.name());
                        }

                        // Pas la bonne valeur (le message doit exister)
                        if( getInstance().modelMes.find(mesg_param.value()) == getInstance().modelMes.end() )
                        {
                            throw Exception::UnimplementedElement<StateMessage_format>(mesg_param.value());
                        }
                        else {
                            getInstance().modelState[l_currentState.getId()].addMessage(
                                        &getInstance().modelMes[mesg_param.value()]);
                        }
                    }
                }
            }
            else
            {
                throw Exception::ParsingFileBaliseError<State>(next_lvl.name());
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////
void Model::checkIntegrity()
{
    // Parcourt toutes les transitions de tous les états
    // pour mettre à jour l'état de destination
    for(auto& l_states: getInstance().modelState )
    {
        for(auto& l_transitions: l_states.second.getTransitions())
        {
            auto& l_destStateName = l_transitions->getDestStateName();
            if(getInstance().modelState.find(l_destStateName) == getInstance().modelState.end() )
            {
                ERROR("Error - Model error - Integrity check failed.");
                throw Exception::IntegrityCheckException<Transition>(l_destStateName);
            }
            l_transitions->setDestState(&getInstance().modelState[l_destStateName]);
        }
    }

    // Parcourt toutes les transitions de tous les états
    // pour mettre à jour la variable sur laquelle ils opèrent
    for(auto& l_states: getInstance().modelState )
    {
        for(auto& l_transitions: l_states.second.getTransitions())
        {
            auto l_varName = l_transitions->getVar();
            if(!l_varName.empty() && getInstance().modelVar.find(l_varName) == getInstance().modelVar.end() )
            {
                ERROR("Error - Model error - Integrity check failed.");
                throw Exception::IntegrityCheckException<Transition>(l_varName);
            }
            l_transitions->setVar(getInstance().modelVar[l_varName]);
        }
    }

    // Parcourt tous les messages du modèle
    // pour mettre à jour le Header qu'ils référencent
    for(auto& l_messages: getInstance().modelMes )
    {
        auto& l_messageHeaderName = l_messages.second.getHeaderName();
        if(getInstance().modelHead.find(l_messageHeaderName) == getInstance().modelHead.end() )
        {
            ERROR("Error - Model error - Integrity check failed.");
            throw Exception::IntegrityCheckException<Message>(l_messageHeaderName);
        }
        l_messages.second.setHeader(&getInstance().modelHead[l_messageHeaderName]);
    }

    DEBUG("Model Integrity successfully checked.");
}

////////////////////////////////////////////////////////////////////////
void Model::clear()
{
    if(getInstance().curState)
    {
        getInstance().curState = nullptr;
    }

    if(getInstance().nexState)
    {
        getInstance().nexState = nullptr;
    }

    getInstance().modelVar.clear();
    getInstance().modelMes.clear();
    getInstance().modelHead.clear();
    getInstance().modelState.clear();
}

////////////////////////////////////////////////////////////////////////
void Model::setup(const string& p_filePath)
{
    getInstance().currentStateStr = NOT_INITIALIZED;
    clear();

    if(getInstance().confFile)
    {
        delete getInstance().confFile;
    }
    getInstance().confFile = new string(p_filePath);

    if(!getInstance().confFile)
    {
        ERROR("Error - Could not create the required pointer to the configuration file.");
        throw Exception::ParsingFileError("Could not create the required pointer to the configuration file.");
    }

    pugi::xml_document	   l_doc;
    pugi::xml_parse_result l_result = l_doc.load_file(getInstance().confFile->c_str());
    DEBUG("Configuration file set to " + *getInstance().confFile);

    if (!l_result)
    {
        ERROR("Error - Configuration file parsing failed (position " + to_string(l_result.offset) + ").");
        throw Exception::ParsingFileError(l_result.description(), l_result.offset);
    }

    initializeVariables(l_doc); // Initialisation des variables du modèle
    initializeMessages(l_doc);  // Initialisation des messages du modèle
    initializeHeaders(l_doc);   // Initialisation des headers du modèle
    initializeStates(l_doc);    // Initialisation des états du modèle

    checkIntegrity();           // Controles finaux d'intégrité du modele

    getInstance().currentStateStr = INITIALIZED;
    DEBUG("Configuration file parsed successfully.");
}

} // namespace ModGen