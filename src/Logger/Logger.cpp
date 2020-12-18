/*!
 * @file   Logger.cpp
 * @brief  Implementations of the functions defined in \a Logger.h
 * @author lhm
 * @date   16/07/2019
 */

#include <iostream>

#include "Logger.h"
#include "Exception.h"
#include "time_util.h"

namespace ModGen {

using namespace std;

/////////////////////////////////////////////////////////////////////////////////
void ERROR(const string& p_msg)
{
    Logger::append(p_msg, Logger::ERRORS_ONLY);
}

/////////////////////////////////////////////////////////////////////////////////
void INFO(const string& p_msg)
{
    Logger::append(p_msg, Logger::ERRORS_INFO);
}

/////////////////////////////////////////////////////////////////////////////////
void DEBUG(const string &p_msg)
{
    Logger::append(p_msg, Logger::ERRORS_INFO_DEBUG);
}

/////////////////////////////////////////////////////////////////////////////////
void DEBUG(const vector<uint8_t>& p_byteArray)
{
    Logger::append(p_byteArray, Logger::ERRORS_INFO_DEBUG);
}

/////////////////////////////////////////////////////////////////////////////////
void WRITE(const string& p_msg, const int32_t& p_traceLevel)
{
    if(p_traceLevel >= 0)
    {
        Logger::append(p_msg, static_cast<Logger::TRACELEVELS>(p_traceLevel));
    }
}

/////////////////////////////////////////////////////////////////////////////////
void Logger::setLogFile(const string& p_filePath)
{
    if(getInstance().logFile)
    {
        if(getInstance().logFile->is_open())
        {
            getInstance().logFile->close();
        }

        delete getInstance().logFile;
        getInstance().logFile = nullptr;
    }

    if(getInstance().logName)
    {
        delete getInstance().logName;
        getInstance().logName = nullptr;
    }

    // Do not declare any log file if empty
    if(!p_filePath.empty())
    {
        getInstance().logFile = new ofstream(p_filePath, ios::out | ios::app);
        getInstance().logName = new string(p_filePath);
        if (!getInstance().logFile->is_open())
        {
            throw Exception::LoggerError(TimeUtil::currentTime() + ":: Error - Logger::setLogFile - Unable to open the specified log file("
                                         + p_filePath + ").");
        }

        INFO("Log file set to " + p_filePath);
    }
}

/////////////////////////////////////////////////////////////////////////////////
void Logger::setTraceLevel(TRACELEVELS p_traceLevel)
{
    getInstance().traceLevel = p_traceLevel;
}

/////////////////////////////////////////////////////////////////////////////////
bool Logger::isDebugEnabled()
{
    return getInstance().traceLevel >= ERRORS_INFO_DEBUG;
}

/////////////////////////////////////////////////////////////////////////////////
bool Logger::isOpened()
{
    bool l_return = false;

    if(getInstance().logFile)
    {
        l_return = getInstance().logFile->is_open();
    }

    return l_return;
}

/////////////////////////////////////////////////////////////////////////////////
string Logger::getLogFile()
{
    string l_return("");

    if(getInstance().logName)
    {
        l_return = *(getInstance().logName);
    }

    return l_return;
}

/////////////////////////////////////////////////////////////////////////////////
Logger::Logger() :
    logFile(nullptr),
    logName(nullptr),
    traceLevel(NO_LOGS)
{}

/////////////////////////////////////////////////////////////////////////////////
Logger::~Logger()
{
    if(logFile)
    {
        logFile->close();
        delete logFile;
    }

    if(logName)
    {
        delete logName;
    }
}

/////////////////////////////////////////////////////////////////////////////////
Logger& Logger::getInstance()
{
    static Logger l_instance;
    return l_instance;
}

/////////////////////////////////////////////////////////////////////////////////
void Logger::append(const string &p_msg, TRACELEVELS p_traceLevel)
{
    if(getInstance().isOpened() && 
       getInstance().traceLevel >= p_traceLevel)
    {
        string l_currenTime = TimeUtil::currentTime();
        switch(p_traceLevel)
        {
            case ERRORS_ONLY: cerr << l_currenTime << ":: " << p_msg << "\n"; break;
            default:break;
        }

        if(!getInstance().logFile)
        {
            throw Exception::LoggerError(l_currenTime + ":: Error - Logger::append - Unspecified log file.");
        }

        *getInstance().logFile << l_currenTime << ":: " << p_msg << "\n";
    }
}

/////////////////////////////////////////////////////////////////////////////////
void Logger::append(const vector<uint8_t> &p_byteArray, TRACELEVELS p_traceLevel)
{
    if(getInstance().isOpened() && 
       getInstance().traceLevel >= p_traceLevel)
    {
        switch(p_traceLevel)
        {
            case ERRORS_ONLY:
                for(auto& l_char : p_byteArray) { cerr << l_char << " "; }
                break;
            default:
                // for(auto& l_char : p_byteArray) { cout << l_char << " "; }
                break;
        }

        if(!getInstance().logFile)
        {
            throw Exception::LoggerError(TimeUtil::currentTime() + ":: Error - Logger::append - Unspecified log file.");
        }

        for(auto& l_char : p_byteArray) { *getInstance().logFile << l_char << " "; }
    }
}

/////////////////////////////////////////////////////////////////////////////////
void Logger::setup(const string& p_filePath, TRACELEVELS p_traceLevel)
{
    getInstance().setTraceLevel(p_traceLevel);
    getInstance().setLogFile   (p_filePath);
}

/////////////////////////////////////////////////////////////////////////////////
void Logger::closeLogs()
{
    if(getInstance().logFile)
    {
        getInstance().logFile->close();
        delete getInstance().logFile;
        getInstance().logFile = nullptr;
    }

    if(getInstance().logName)
    {
        delete getInstance().logName;
        getInstance().logName = nullptr;
    }
}

} // namespace ModGen