/******************************************************************************
  Project:      HOME_ASSIGNMENT
  Description:  COMPRESSER ABSTRACT CLASS
  Author:       Michael
  Date:         05/14/23
  Version:      1.0
 ******************************************************************************/
#ifndef HAM_COMPRESSER_HPP
#define HAM_COMPRESSER_HPP

#include <string>   //uint8
#include <vector>   //vector

namespace ham
{

class Compresser
{
public:
    // using generated Ctor, Dtor and CCtor

    virtual std::vector<uint8_t> Compress(const std::vector<int>& data) = 0;
    virtual std::vector<int> Decompress(const std::vector<uint8_t>& compData) = 0;
};

} //namespace ham

#endif //HAM_COMPRESSER_HPP
