/*!
 * @file   modelGenerator_interface.cpp
 * @brief  Contains the implementation of the interface 
 *		   of the library.
 * @author lhm
 * @date   23/07/2019
 */

#include "modelGenerator_interface.h"
#include "version_util.h"
#include "Logger.h"
#include "Model.h"
#include "State.h"
#include "opt_util.h"
#include "Exception.h"
#include "time_util.h"

using namespace ModGen;

std::string ModelGeneratorAPI::VERSION::VersionCommit(void)
{
	return getVersionCommit();
}

std::string ModelGeneratorAPI::VERSION::VersionBranch(void)
{
	return getVersionBranch();
}

std::string ModelGeneratorAPI::VERSION::Version(void)
{
    return getVersion();
}

std::string ModelGeneratorAPI::UTIL::getDefaultConfFile(void)
{
    return DEFAULT_CONF_FILEPATH;
}

 void ModelGeneratorAPI::EXCEP::CommandLineError(const std::string& p_mesg)
 {
     throw Exception::CommandLineArgsError(p_mesg);
 }

std::string ModelGeneratorAPI::UTIL::getDefaultLogsFile(void)
{
    return DEFAULT_LOGS_FILEPATH;
}

int32_t ModelGeneratorAPI::UTIL::getDefaultTraceValue(void)
{
    return DEFAULT_LOGS_TRACEVAL;
}

void ModelGeneratorAPI::UTIL::displayHelp(void)
{
    display_help();
}

int ModelGeneratorAPI::UTIL::getcmd(int          p_narg,
                                    char* const* p_nargv,
                                    const char*  p_options)
{
    return getopt(p_narg, p_nargv, p_options);
}

char* ModelGeneratorAPI::UTIL::getoptArg(void)
{
    return optarg;
}

void ModelGeneratorAPI::LOGS::disable(void)
{
    Logger::setTraceLevel(Logger::TRACELEVELS::NO_LOGS);
}

void ModelGeneratorAPI::LOGS::setup(const std::string& p_logFile,
                                    const int32_t&   p_logLevel)
{
    Logger::setup(p_logFile, 
        static_cast<Logger::TRACELEVELS>(p_logLevel));
}

void ModelGeneratorAPI::LOGS::write(const std::string& p_toLog,
                                    const int32_t&     p_logLevel)
{
    WRITE(p_toLog, p_logLevel);
}

bool ModelGeneratorAPI::LOGS::isOpened(void)
{
    return Logger::isOpened();
}

std::string ModelGeneratorAPI::LOGS::getLogFile(void)
{
    return Logger::getLogFile();
}

void ModelGeneratorAPI::LOGS::close(void)
{
    Logger::closeLogs();
}

void ModelGeneratorAPI::MODEL::create(const std::string& p_confFile)
{
    Model::setup(p_confFile);
}

void ModelGeneratorAPI::MODEL::log(void)
{
    Model::log();
}

std::vector< std::vector<unsigned char> > ModelGeneratorAPI::MODEL::getMessages(void)
{
    return Model::getCurrState()->getMessages();
}

void ModelGeneratorAPI::MODEL::runOperations(void)
{
    Model::getCurrState()->runOperations();
}

void ModelGeneratorAPI::MODEL::runTransitions(void)
{
    Model::getCurrState()->runTransitions();
}

void ModelGeneratorAPI::MODEL::nextState(void)
{
    Model::nextState();
}

const std::string& ModelGeneratorAPI::MODEL::currentStateString(void)
{
    return Model::getCurrState()->getId();
}

std::vector< std::string > ModelGeneratorAPI::MODEL::getMessagesSrcIp(void)
{
    return Model::getCurrState()->getMessagesSrcIP();
}

std::vector< std::string > ModelGeneratorAPI::MODEL::getMessagesDstIp(void)
{
    return Model::getCurrState()->getMessagesDstIP();
}

std::vector< std::string > ModelGeneratorAPI::MODEL::getMessagesIntface(void)
{
    return Model::getCurrState()->getMessagesIntface();
}

std::vector< uint32_t > ModelGeneratorAPI::MODEL::getMessagesSrcPort(void)
{
    return Model::getCurrState()->getMessagesSrcPort();
}

std::vector< uint32_t > ModelGeneratorAPI::MODEL::getMessagesDstPort(void)
{
    return Model::getCurrState()->getMessagesDstPort();
}

uint64_t ModelGeneratorAPI::TIME::getDIANE(uint64_t &p_time_today_us)
{
    p_time_today_us = TimeUtil::day_microseconds();
    HHR_DIANE_Time l_diane(p_time_today_us);

    return l_diane.toInt();
}

uint64_t ModelGeneratorAPI::TIME::getDIANEValue(const uint64_t p_time_us)
{
    HHR_DIANE_Time l_diane(p_time_us);

    return l_diane.toInt();
}