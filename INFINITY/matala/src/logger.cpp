/******************************************************************************
  Project:      HOME_ASSIGNMENT
  Description:  LOGGER
  Author:       Michael
  Date:         05/14/23
  Version:      1.0
 ******************************************************************************/

#include <mutex>        //std::mutex
#include <fstream>      //std::ofstream
#include <fcntl.h>      // open 
#include <iomanip>      //time

#include "logger.hpp"

namespace ham
{

Logger::Logger(const std::string& logFilePath_):
    m_mutex(), m_logFile(logFilePath_, std::ofstream::app), m_currLevel(LogLevel::DEBUG)
{
    if (!m_logFile.good())
    {
        std::cout << "couldnt open the file" << std::endl;
    }
}

void Logger::Log(const LogLevel& msgLevel_,
    const std::string& msg_,
    size_t line_,
    const std::string& file_,
    bool toScreen_)
{
    if (msgLevel_ > m_currLevel)
    {
        return;
    }

    {// critical section
        std::lock_guard<std::mutex> lock(m_mutex);

        m_logFile << "[" << GetTimestamp() << "] " << GetLevel(msgLevel_) << " [" << file_ << ":" << line_ << "] " << msg_ << "\n";
    } // end of critical section

    if (toScreen_)
    {
        std::cout << "[" << GetTimestamp() << "] " << GetLevel(msgLevel_) << " [" << file_ << ":" << line_ << "] " << msg_ << std::endl;
    }

}

std::string Logger::GetLevel(const LogLevel& level_)
{
    switch (level_)
    {
    case LogLevel::ERROR:
        return "ERROR";
    case LogLevel::WARNING:
        return "WARNING";
    case LogLevel::INFO:
        return "INFO";
    case LogLevel::DEBUG:
        return "DEBUG";
    default:
        return "NO SUCH LEVEL";
    }
}


void Logger::SetLogFile(const std::string& logFilePath_)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_logFile.good())
    {
        m_logFile.close();
    }

    m_logFile.open(logFilePath_, std::ios::app);

    if (m_logFile.good())
    {
        std::cout << "Log file Reset " << logFilePath_ << std::endl;
    }

    else
    {
        std::cerr << "Failed to open log file: " << logFilePath_ << std::endl;
    }

}

void Logger::SetCurrLevel(const LogLevel& level_)
{
    m_currLevel = level_;
}


std::string Logger::GetTimestamp()
{
    const auto now = std::chrono::system_clock::now();
    const auto nowAsTimeT = std::chrono::system_clock::to_time_t(now);
    const auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::string ret;
    std::stringstream nowSs(ret);

    nowSs << std::put_time(std::localtime(&nowAsTimeT), "%T")
        << '.' << std::setfill('0') << std::setw(3) << nowMs.count();
    return nowSs.str();
}

}//namespace hrd27




