/******************************************************************************
  Project:      HOME_ASSIGNMENT
  Description:  Bit Pack CPP
  Author:       Michael
  Date:         05/14/23
  Version:      1.0
 ******************************************************************************/

#include "utils.hpp"
#include "bitpack.hpp"

#include <fstream>  //ofstream
#include <iostream>
#include <cstdlib>
#include <ctime>    //srand
#include <vector> 
#include <thread>

using namespace ham;
using namespace std;

void packBits(uint32_t& buffer, int& bitsInBuffer, int value, int bits);

vector<uint8_t> BpackCompresser::Compress(const vector<int>& data_)
{
    vector<uint8_t> outputBuffer;

    uint32_t buffer = 0; 
    int bitsInBuffer = 0;

    for (int value : data_)
    {
        // push value to buffer , represented by 7 bits
        packBits(buffer, bitsInBuffer, value, 7);

        // when there are more than 8 bits in the buffer - flush it to the vector
        while (bitsInBuffer >= 8)
        {
            // write the lower byte from buffer to the vector
            outputBuffer.push_back(buffer & 0xFF);

            // update the values for next iteration
            buffer >>= 8;
            bitsInBuffer -= 8;
        }
    }

    // deal with leftovers
    if (bitsInBuffer > 0)
    {
        // write the lower byte from buffer to the vector
        outputBuffer.push_back(buffer & 0xFF);
    }

    return outputBuffer;
}

vector<int> BpackCompresser::Decompress(const vector<uint8_t>& inputBuffer)
{
    if (inputBuffer.empty())
    {
        Logger* log = Singleton<Logger>::GetInstance();
        log->Log(LogLevel::ERROR, "Error: input buffer is empty", __LINE__, __FILE__);
        return vector<int>{};
    }

    vector<int> ret;
    // TODO:
    // ret.reserve(inputBuffer.size()); // Reserve space for datasize values

    uint32_t buffer = 0; // Use a 32-bit buffer to hold the bits
    int bitsInBuffer = 0;

    for (uint8_t byte : inputBuffer)
    {
        packBits(buffer, bitsInBuffer, byte, 8);

        // While we have more than 7 bits left in the buffer, extract a value and adjust the buffer
        while (bitsInBuffer >= 7)
        {
            ret.push_back(buffer & 0x7F); // push 7 least significant bits

            // update the buffer and bits in buffer
            buffer >>= 7;
            bitsInBuffer -= 7;
        }
    }

    // Only interpret the remaining bits if there are 7 of them - last value
    if (bitsInBuffer >= 7)
    {
        ret.push_back(buffer & 0x7F); // push 7 least significant bits
    }

    return ret;
}

