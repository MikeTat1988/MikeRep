/******************************************************************************
  Project:      HOME_ASSIGNMENT
  Description:  Bit Packing With Run LEngth
  Author:       Michael
  Date:         05/14/23
  Version:      1.0
 ******************************************************************************/
#ifndef HAM_BPRL_HPP
#define HAM_BPRL_HPP

#include <string>   //uint8
#include <vector>   //vector

#include "compresser.hpp"

namespace ham
{

class BpackRLCompresser : public Compresser
{
public:
    explicit BpackRLCompresser(size_t originalSize) : m_datasize(originalSize) {};
    // using generated Dtor and CCtor

    std::vector<uint8_t> Compress(const std::vector<int>& data) override;
    std::vector<int> Decompress(const std::vector<uint8_t>& compData) override;
private:
    size_t m_datasize;
};

} //namespace ham

#endif //HAM_BPRL_HPP
