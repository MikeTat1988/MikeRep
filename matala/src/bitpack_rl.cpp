/******************************************************************************
  Project:      HOME_ASSIGNMENT
  Description:  Bit Pack with Run Length CPP
  Author:       Michael
  Date:         05/14/23
  Version:      1.0
 ******************************************************************************/

#include "utils.hpp"
#include "bitpack_rl.hpp"

#include <fstream>  //ofstream
#include <iostream>
#include <cstdlib>
#include <ctime>    //srand
#include <vector> 
#include <thread>

using namespace ham;
using namespace std;

void packBits(uint32_t& buffer, int& bitsInBuffer, int value, int bits);

vector<uint8_t> BpackRLCompresser::Compress(const vector<int>& data_)
{
    vector<uint8_t> outputBuffer;

    uint32_t buffer = 0;
    int bitsInBuffer = 0;
    const int escapeValue = 127;
    int count = 0;

    for (size_t i = 0; i < data_.size(); i += count)
    {
        int value = data_[i];
        count = 1;
        while (i + count < data_.size() && data_[i + count] == value)
        {
            ++count;
        }

        // if there are repetitions - push escval, value and count
        if (count > 1)
        {
            packBits(buffer, bitsInBuffer, escapeValue, 7);
            packBits(buffer, bitsInBuffer, value, 7);
            packBits(buffer, bitsInBuffer, count, 7);
        }

        else
        {
            packBits(buffer, bitsInBuffer, value, 7);
        }

        while (bitsInBuffer >= 8)
        {
            outputBuffer.push_back(buffer & 0xFF);
            buffer >>= 8;
            bitsInBuffer -= 8;
        }
    }

    // push leftovers
    if (bitsInBuffer > 0)
    {
        outputBuffer.push_back(buffer & 0xFF);
    }

    return outputBuffer;
}

vector<int> BpackRLCompresser::Decompress(const vector<uint8_t>& inputBuffer)
{
    if (inputBuffer.empty())
    {
        Logger* log = Singleton<Logger>::GetInstance();
        log->Log(LogLevel::ERROR, "Error: input buffer is empty", __LINE__, __FILE__);
        return vector<int>{};
    }

    vector<int> values;

    uint32_t buffer = 0;
    int bitsInBuffer = 0;
    const int escapeValue = 127;
    int value = 0;

    for (uint8_t byte : inputBuffer)
    {
        packBits(buffer, bitsInBuffer, byte, 8);

        while (bitsInBuffer >= 7)
        {
            value = buffer & 0x7F;

            if (value == escapeValue)
            {
                // Check if there are enough bits to handle the escape sequence
                if (bitsInBuffer < 21)
                { // Not enough bits for runValue
                    break;
                }
                buffer >>= 7;

                int runValue = buffer & 0x7F;
                buffer >>= 7;

                int runLength = buffer & 0x7F;
                buffer >>= 7;

                bitsInBuffer -= 21; // after reading two values

                for (int i = 0; i < runLength; ++i)
                {
                    values.push_back(runValue);
                }
            }

            else
            {
                buffer >>= 7;
                bitsInBuffer -= 7;
                if (values.size() < m_datasize)
                {
                    values.push_back(value);
                }
            }
        }
    }

    // Only interpret the remaining bits if there are at least 7 of them
    while (bitsInBuffer >= 7)
    {
        values.push_back(buffer & 0x7F);
    }

    return values;
}

// HELPER FUNCS

