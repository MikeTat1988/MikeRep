/******************************************************************************
  Project:      HOME_ASSIGNMENT
  Description:  LOGGER HPP
  Author:       Michael
  Date:         05/14/23
  Version:      1.0
 ******************************************************************************/
#ifndef HAM_LOGGER_HPP
#define HAM_LOGGER_HPP 

#include <mutex> //std::mutex
#include <fstream> //std::ofstream

#include "singleton.hpp"

namespace ham
{

enum class LogLevel
{//order from most strict to least strict
    ERROR,
    WARNING,
    INFO,
    DEBUG
};

class Logger
{

public:
    ~Logger() = default;

    Logger(const Logger& other_)= delete;
    Logger& operator=(const Logger& other_)= delete;
    
    void Log(   const LogLevel& msgLevel_ , 
                const std::string& msg_, 
                size_t line_ ,
                const std::string& file_ ,
                bool toScreen_= false);
    
    void SetLogFile(const std::string& logFilePath_);

    void SetCurrLevel(const LogLevel& level_);

private:
    std::mutex m_mutex;
    std::ofstream m_logFile;
    LogLevel m_currLevel;
 
    explicit Logger(const std::string& logFilePath_ = "./log/log.txt");

    friend Logger* Singleton<Logger>::GetInstance();

    std::string GetLevel(const LogLevel&);
    std::string GetTimestamp();
};


}//namespace ham


#endif // HAM_LOGGER_HPP

