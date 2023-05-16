/******************************************************************************
  Project:      HOME_ASSIGNMENT
  Description:  Parallel Bit Packing CPP
  Author:       Michael
  Date:         05/14/23
  Version:      1.0
 ******************************************************************************/

// #include "utils.hpp"
#include "bitpack_parallel_tp.hpp"
#include "w_pqueue.hpp"
#include "bitpack.hpp"
#include "threadpool.hpp"

#include "data.hpp"

#include <fstream>  //ofstream
#include <iostream>
#include <cstdlib>
#include <ctime>    //srand
#include <vector> 
#include <thread>

using namespace ham;
using namespace std;


static inline size_t GetChunk(size_t datasize_, int& numofthreads_);
void packBits(uint32_t& buffer, int& bitsInBuffer, int value, int bits);


vector<uint8_t> MultiThreadCompresserTP::Compress(const vector<int>& data)
{
    int threadnum = m_numOfThreads;

    size_t chunkSize = GetChunk(data.size(), m_numOfThreads);

    if (threadnum != m_numOfThreads)
    {
        m_threadPool.SetSize(m_numOfThreads);
    }

    auto resultQueue =
        make_shared<WPQueue<pair<int, vector<uint8_t>>, vector<pair<int, vector<uint8_t>>>, ComparePairTP<uint8_t>>>();


        // split the data to threads
    for (int i = 0; i < m_numOfThreads; ++i)
    {
        BpackCompresser myCompresser;
        auto begin = data.begin() + i * chunkSize;
        auto end = (i == m_numOfThreads - 1) ? data.end() : (data.begin() + (i + 1) * chunkSize);
        m_threadPool.PushTask(make_shared<CompNPushTask>(begin, end, &myCompresser, resultQueue, i));
    }

    // Wait for all tasks to complete
    while (resultQueue->Size() != static_cast<size_t>(m_numOfThreads))
    {
    }

    vector<uint8_t> compressedData;

    // combine the data back
    while (!resultQueue->IsEmpty())
    {
        pair<int, vector<uint8_t>> result;
        resultQueue->Pop(result);
        compressedData.insert(compressedData.end(), result.second.begin(), result.second.end());
    }

    return compressedData;
}

vector<int> MultiThreadCompresserTP::Decompress(const vector<uint8_t>& compData)
{
    int threadnum = m_numOfThreads;

    size_t chunkSize = GetChunk(compData.size(), m_numOfThreads);

    if (threadnum != m_numOfThreads)
    {
        m_threadPool.SetSize(m_numOfThreads);
    }

    auto resultQueue =
        make_shared<WPQueue<pair<int, vector<int>>, vector<pair<int, vector<int>>>, ComparePairTP<int>>>();

    for (int i = 0; i < m_numOfThreads; ++i)
    {
        BpackCompresser myCompresser;
        auto begin = compData.begin() + i * chunkSize;
        auto end = (i == m_numOfThreads - 1) ? compData.end() : (compData.begin() + (i + 1) * chunkSize);

        m_threadPool.PushTask(make_shared<DecNPushTask>(begin, end, &myCompresser, resultQueue, i));
    }

    // Wait for all tasks to complete
    while (resultQueue->Size() != static_cast<size_t>(m_numOfThreads))
    {
    }

  // Sort the results and concatenate them
    vector<int> decompressedData;
    while (!resultQueue->IsEmpty())
    {
        pair<int, vector<int>> result;
        resultQueue->Pop(result);
        decompressedData.insert(decompressedData.end(), result.second.begin(), result.second.end());
    }

    return decompressedData;
}

MultiThreadCompresserTP::CompNPushTask::CompNPushTask(
    vector<int>::const_iterator begin,
    vector<int>::const_iterator end,
    Compresser* dataInstance,
    shared_ptr<WPQueue<pair<int, vector<uint8_t>>, vector<pair<int, vector<uint8_t>>>, ComparePairTP<uint8_t>>> resultQueue,
    int order) :
    m_begin(begin),
    m_end(end),
    m_dataInstance(dataInstance),
    m_resultQueue(resultQueue),
    m_order(order)
{
}

void MultiThreadCompresserTP::CompNPushTask::Execute()
{
    vector<int> chunk(m_begin, m_end);
    auto result = m_dataInstance->Compress(chunk);
    m_resultQueue->Push({ m_order, result });
}

MultiThreadCompresserTP::DecNPushTask::DecNPushTask(vector<uint8_t>::const_iterator begin, vector<uint8_t>::const_iterator end,
    Compresser* dataInstance,
    shared_ptr<WPQueue<pair<int, vector<int>>, vector<pair<int, vector<int>>>, ComparePairTP<int>>> resultQueue,
    int order) :
    m_begin(begin),
    m_end(end),
    m_dataInstance(dataInstance),
    m_resultQueue(resultQueue),
    m_order(order)
{}

void MultiThreadCompresserTP::DecNPushTask::Execute()
{
    vector<uint8_t> chunk(m_begin, m_end);
    auto result = m_dataInstance->Decompress(chunk);
    m_resultQueue->Push({ m_order, result });
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
