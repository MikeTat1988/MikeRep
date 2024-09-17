/******************************************************************************
  Project:      HOME_ASSIGNMENT
  Description:  Multithreaded Bit Packing - threadpool
  Author:       Michael
  Date:         05/14/23
  Version:      1.0
 ******************************************************************************/
#ifndef HAM_BPMULTITP_HPP
#define HAM_BPMULTITP_HPP

#include <string>   //uint8
#include <vector>   //vector

#include "compresser.hpp"
#include "threadpool.hpp"

namespace ham
{

template <typename T>
struct ComparePairTP;

class MultiThreadCompresserTP : public Compresser
{
public:
    explicit MultiThreadCompresserTP(int numOfThreads) :m_numOfThreads(numOfThreads), m_threadPool(numOfThreads) {};
    ~MultiThreadCompresserTP(){m_threadPool.Stop();};
    MultiThreadCompresserTP(const MultiThreadCompresserTP&) = default;
    const MultiThreadCompresserTP& operator=(const MultiThreadCompresserTP&) = delete;

    vector<uint8_t> Compress(const vector<int>& data) override;
    vector<int> Decompress(const vector<uint8_t>& compData) override;

private:
    int m_numOfThreads;
    ThreadPool m_threadPool;

    class CompNPushTask;
    class DecNPushTask;
};


class MultiThreadCompresserTP::CompNPushTask : public ITask 
{
public:
    CompNPushTask(vector<int>::const_iterator begin, vector<int>::const_iterator end,
        Compresser* dataInstance, 
        std::shared_ptr<WPQueue<pair<int, vector<uint8_t>>, vector<pair<int, vector<uint8_t>>>, ComparePairTP<uint8_t>>> resultQueue,
        int order);
    void Execute() override;

private:
    vector<int>::const_iterator m_begin;
    vector<int>::const_iterator m_end;

    Compresser* m_dataInstance;

    std::shared_ptr<WPQueue<pair<int, vector<uint8_t>>, vector<pair<int, vector<uint8_t>>>, ComparePairTP<uint8_t>>> m_resultQueue;

    int m_order;
};

class MultiThreadCompresserTP::DecNPushTask : public ITask 
{
public:
    DecNPushTask(vector<uint8_t>::const_iterator begin, vector<uint8_t>::const_iterator end,
        Compresser* dataInstance, 
        shared_ptr<WPQueue<pair<int, vector<int>>, vector<pair<int, vector<int>>>, ComparePairTP<int>>> resultQueue, 
        int order);
    void Execute() override;

private:
    vector<uint8_t>::const_iterator m_begin;
    vector<uint8_t>::const_iterator m_end;

    Compresser* m_dataInstance;

    shared_ptr<WPQueue<pair<int, vector<int>>, vector<pair<int, vector<int>>>, ComparePairTP<int>>> m_resultQueue;

    int m_order;
};

template <typename T>
struct ComparePairTP
{
    bool operator()(const pair<int, vector<T>>& p1, const pair<int, vector<T>>& p2)
    {
        return p1.first > p2.first;
    }
};

} // namespace ham

#endif //HAM_BPMULTITP_HPP
