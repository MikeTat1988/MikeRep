/******************************************************************************
  Project:      HOME_ASSIGNMENT
  Description:  Parallel Bit Packing CPP
  Author:       Michael
  Date:         05/14/23
  Version:      1.0
 ******************************************************************************/

// #include "utils.hpp"
#include "bitpack_parallel.hpp"
#include "w_pqueue.hpp"
#include "bitpack.hpp"

#include "data.hpp"

#include <fstream>  //ofstream
#include <iostream>
#include <cstdlib>
#include <ctime>    //srand
#include <vector> 
#include <thread>

using namespace ham;
using namespace std;

template <typename T>
struct ComparePair
{
    bool operator()(const pair<int, vector<T>>& p1, const pair<int, vector<T>>& p2)
    {
        return p1.first > p2.first;
    }
};

static inline size_t GetChunk(size_t datasize_, int& numofthreads_);
void packBits(uint32_t& buffer, int& bitsInBuffer, int value, int bits);

inline void DecNPush(const vector<uint8_t>::const_iterator& begin,
    const vector<uint8_t>::const_iterator& end,
    Compresser* dataInstance,
    WPQueue<pair<int, vector<int>>, vector<pair<int, vector<int>>>, ComparePair<int>>& resultQueue,
    int order)
{
    vector<uint8_t> chunk(begin, end);
    auto result = dataInstance->Decompress(chunk);
    resultQueue.Push({ order, result });
}

inline void CompNPush(const vector<int>::const_iterator& begin,
    const vector<int>::const_iterator& end,
    Compresser* dataInstance,
    WPQueue<pair<int, vector<uint8_t>>, vector<pair<int, vector<uint8_t>>>, ComparePair<uint8_t>>& resultQueue,
    int order)
{
    vector<int> chunk(begin, end);
    auto result = dataInstance->Compress(chunk);
    resultQueue.Push({ order, result });
}

vector<uint8_t> MultiThreadCompresser::Compress(const vector<int>& data)
{
    size_t chunkSize = GetChunk(data.size(), m_numOfThreads);

    vector<thread> threads;
    WPQueue<pair<int, vector<uint8_t>>, vector<pair<int, vector<uint8_t>>>, ComparePair<uint8_t>> resultQueue;

    // split the data to threads
    for (int i = 0; i < m_numOfThreads; ++i)
    {
        BpackCompresser myCompresser;
        auto begin = data.begin() + i * chunkSize;
        auto end = (i == m_numOfThreads - 1) ? data.end() : (data.begin() + (i + 1) * chunkSize);
        threads.push_back(thread(CompNPush, begin, end, &myCompresser, ref(resultQueue), i));
    }

    // Ensure all threads have completed
    for (auto& thread : threads)
    {
        thread.join();
    }

    vector<uint8_t> compressedData;

    // combine the data back
    for (int i = 0; i < m_numOfThreads; ++i)
    {
        pair<int, vector<uint8_t>> result;
        resultQueue.Pop(result); // This will block until a result is available
        compressedData.insert(compressedData.end(), result.second.begin(), result.second.end());
    }

    return compressedData;
}


vector<int> MultiThreadCompresser::Decompress(const vector<uint8_t>& compData)
{
    size_t chunkSize = GetChunk(compData.size(), m_numOfThreads);

    vector<thread> threads;
    WPQueue<pair<int, vector<int>>, vector<pair<int, vector<int>>>, ComparePair<int>> resultQueue;

    for (int i = 0; i < m_numOfThreads; ++i)
    {
        BpackCompresser myCompresser;
        auto begin = compData.begin() + i * chunkSize;
        auto end = (i == m_numOfThreads - 1) ? compData.end() : (compData.begin() + (i + 1) * chunkSize);

        threads.push_back(thread(DecNPush, begin, end, &myCompresser, ref(resultQueue), i));
    }

    // Ensure all threads have completed
    for (auto& thread : threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }

    // Sort the results and concatenate them
    vector<int> decompressedData;
    for (int i = 0; i < m_numOfThreads; ++i)
    {
        pair<int, vector<int>> result;
        resultQueue.Pop(result); // This will block until a result is available
        decompressedData.insert(decompressedData.end(), result.second.begin(), result.second.end());
    }

    return decompressedData;
}

// HELPER FUNCS


static inline size_t GetChunk(size_t datasize_, int& numofthreads_)
{
    size_t ret = datasize_ / numofthreads_;
    // Align chunkSize to the nearest multiple of 56 - mult of 7 and 8
    ret = ret / 56 * 56;

    // Make sure chunkSize is at least 56
    if (ret < 56)
    {
        numofthreads_ = 1;
        ret = 56;
    }

    return ret;
}
