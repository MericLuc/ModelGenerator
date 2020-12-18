#include <iostream>
#include <csignal>
#include <pugixml.hpp>
#include <Exception.h>
#include <Logger.h>
#include <opt_util.h>
#include <Model.h>
#include <version.h>
#include <State.h>

void signal_handler(int32_t p_signalNum)
{
    ModGen::INFO("Prgramm ended by the user (signal - " + std::to_string(p_signalNum) + ")");
    exit(p_signalNum);
}

int main(int argc, char **argv)
{
    int         l_cmd_value;
    std::string l_conf_file      {DEFAULT_CONF_FILEPATH};
    std::string l_logs_file      {DEFAULT_LOGS_FILEPATH};
    int         l_logs_traceLevel{DEFAULT_LOGS_TRACEVAL};

    // Register the signals and the signal handler to the app
    std::signal(SIGINT, signal_handler);

    while((l_cmd_value = getopt(argc, argv, "c:l:h:t:")) != -1)
    {
        switch(l_cmd_value)
        {
        default:
        case 'h':
            ModGen::display_help();
            exit(0);
        case 'c':
            if(optarg)
            {
                l_conf_file = optarg;
            }
            else
            {
                throw ModGen::Exception::CommandLineArgsError("(-c) Configuration file not properly set");
            }
            break;
        case 'l':
            if(optarg)
            {
                l_logs_file = optarg;
            }
            else
            {
                throw ModGen::Exception::CommandLineArgsError("(-l) Logs file not properly set");
            }
            break;
        case 't':
            if(!optarg)
            {
                throw ModGen::Exception::CommandLineArgsError("(-t) Logs trace value not properly set");
            }
            l_logs_traceLevel = strtol(optarg, static_cast<char **>(nullptr), 10);
            break;
        }
    }

    ModGen::Logger::setup(l_logs_file, static_cast<ModGen::Logger::TRACELEVELS>(l_logs_traceLevel));
    ModGen::Model::setup(l_conf_file);
    ModGen::Model::log();

    while(1)
    {
        ModGen::State* l_currState = ModGen::Model::getNextState();
        ModGen::INFO(l_currState->getId());
        auto l_currMesgs = l_currState->getMessages();
        l_currState->runOperations();
        l_currState->runTransitions();
    }
}
