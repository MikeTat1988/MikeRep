/******************************************************************************
  Project:      HOME_ASSIGNMENT
  Description:  Multithreaded Bit Packing
  Author:       Michael
  Date:         05/14/23
  Version:      1.0
 ******************************************************************************/
#ifndef HAM_BPMULTI_HPP
#define HAM_BPMULTI_HPP

#include <string>   //uint8
#include <vector>   //vector

#include "compresser.hpp"

namespace ham
{

class MultiThreadCompresser : public Compresser
{
public:
    explicit MultiThreadCompresser(int numOfThreads) :m_numOfThreads(numOfThreads) {};
    ~MultiThreadCompresser() = default;
    MultiThreadCompresser(const MultiThreadCompresser&) = default;
    const MultiThreadCompresser& operator=(const MultiThreadCompresser&) = delete;

    std::vector<uint8_t> Compress(const std::vector<int>& data) override;
    std::vector<int> Decompress(const std::vector<uint8_t>& compData) override;

private:
    int m_numOfThreads;
};

} // namespace ham

#endif //HAM_BPMULTI_HPP
