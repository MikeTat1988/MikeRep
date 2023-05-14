/******************************************************************************
  Project:      HOME_ASSIGNMENT
  Description:  Bit Packing algorithm
  Author:       Michael
  Date:         05/14/23
  Version:      1.0
 ******************************************************************************/
#ifndef HAM_BPACK_HPP
#define HAM_BPACK_HPP

#include <string>   //uint8
#include <vector>   //vector

#include "compresser.hpp"

namespace ham
{

class BpackCompresser : public Compresser
{
public:
    BpackCompresser() = default;
    ~BpackCompresser() = default;
    BpackCompresser(const BpackCompresser&) = default;
    const BpackCompresser& operator=(const BpackCompresser&) = delete;

    std::vector<uint8_t> Compress(const std::vector<int>& data) override;
    std::vector<int> Decompress(const std::vector<uint8_t>& compData) override;
};

} // namespace ham

#endif //HAM_BPACK_HPP
