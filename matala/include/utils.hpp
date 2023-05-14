/******************************************************************************
  Project:      HOME_ASSIGNMENT
  Description:  UTILS - ReturnIfBad, ComparePair, DecNPush, CompNPush
  Author:       Michael
  Date:         05/14/23
  Version:      1.0
 ******************************************************************************/

#ifndef HAM_UTILS_HPP
#define HAM_UTILS_HPP

#include "data.hpp" 
#include "w_pqueue.hpp" 
#include "logger.hpp"       // logger
#include "singleton.hpp"    // for logger

using namespace std;

namespace ham
{

inline void packBits(uint32_t& buffer, int& bitsInBuffer, int value, int bits)
{
    buffer |= (static_cast<uint32_t>(value) << bitsInBuffer);
    bitsInBuffer += bits;
}


#define RETURN_IF_BAD(IS_BAD, ERR_MSG)                   \
if (IS_BAD)                                              \
{                                                        \
    Logger* log = Singleton<Logger>::GetInstance();      \
    log->Log(LogLevel::ERROR, ERR_MSG, __LINE__, __FILE__); \
	return ;                                             \
};

}; //ham

#endif // HAM_UTILS_HPP

