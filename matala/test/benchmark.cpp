/******************************************************************************
  Project:      HOME_ASSIGNMENT
  Description:  DATA_TEST
  Author:       Michael
  Date:         05/14/23
  Version:      1.0
 ******************************************************************************/

#include <vector>
#include <iostream>
#include <unistd.h> 
#include <algorithm>
#include <chrono>
#include <ctime>
#include <functional>
#include <sys/times.h>
#include <limits>

#include "data.hpp"         
#include "utils.hpp"        // ReturnIfBad
#include "logger.hpp"       // logger
#include "singleton.hpp"    // for logger
#include "bitpack.hpp"
#include "compresser.hpp"
#include "bitpack_parallel.hpp"
#include "bitpack_rl.hpp"

using namespace std;
using namespace ham;

void Print(const vector<int>& vec_);

void CheckNWrite(const vector<int>& data_, const vector<int>& decompData_, const vector<uint8_t>& compData_, string type);

void ReadWriteTest(const vector<int>&);
void CompNDecompBpackTest(const vector<int>&);
void CompNDecompBpackRLTest(const vector<int>& myData_);
void CompNDecompMulticoreTest(const vector<int>&);
void CompNDecompBpackRLSortedTest(const vector<int>& myData_);

void TimeCheck(const vector<int>& myData_);

void PerformCompressionBenchmark(Compresser& comp, const vector<int>& data, string type);
void PerformDecompressionBenchmark(Compresser& comp, const vector<uint8_t>& data, string type);
void PrintTImes(double t_benchmark, double t_cpu, double t_user, double t_system, string type);


// *****************    DEFAULT CONFIGURATIONS   *******************************
size_t g_datasize = 10000000;
size_t g_ThreadCount = 6;
bool g_toWrite = false;
// *****************************************************************************

Data g_data("./data/mydata", g_datasize);
Logger* g_log = Singleton<Logger>::GetInstance();

int main(int argc, char* argv[])
{
    g_log->SetCurrLevel(LogLevel::INFO);

    g_log->Log(LogLevel::INFO, " ", __LINE__, __FILE__);
    g_log->Log(LogLevel::INFO, "   NEW BENCHMARK", __LINE__, __FILE__);

    // Recieve the arguments (if any) amd update the data
    if (argc > 1)
    {
        int arg = atoi(argv[1]);
        if (arg > 0 && arg < numeric_limits<int>::max())
        {
            g_datasize = arg;
            g_data.UpdateSize(g_datasize);
        }
    }

    if (argc > 2)
    {
        int arg = atoi(argv[2]);
        if (arg > 0 && arg < 16)
        {
            g_ThreadCount = arg;
        }
    }

    if (argc > 3)
    {
        g_toWrite = atoi(argv[3]) != 0;
    }

    g_log->Log(LogLevel::INFO, " Number of elements : " + to_string(g_datasize), __LINE__, __FILE__);

    try
    {
        g_data.Clean();
        g_data.Create();

        vector<int> myData = g_data.Read();

        // Perform the tests 
        ReadWriteTest(myData);
        CompNDecompBpackTest(myData);
        CompNDecompBpackRLTest(myData);
        CompNDecompBpackRLSortedTest(myData);
        CompNDecompMulticoreTest(myData);
        cout << endl;

        g_toWrite = false;

        TimeCheck(myData);

        cout << "Benchmark ended successfuly" << endl;
    }

    catch (const exception& e)
    {
        cerr << e.what() << '\n';
    }

    return 0;
}

void TimeCheck(const vector<int>& myData_)
{
    BpackCompresser myComp_BP;
    MultiThreadCompresser myComp_MTBP(g_ThreadCount);

    auto compData = myComp_MTBP.Compress(myData_); // compress the data for decompression benchmark

    // compress benchmark
    PerformCompressionBenchmark(myComp_BP, myData_, "One Thread Bitpacking Compress");
    PerformCompressionBenchmark(myComp_MTBP, myData_, to_string(g_ThreadCount) + " Threads Bitpacking Compress: ");

    // decompress
    PerformDecompressionBenchmark(myComp_BP, compData, "One Thread Bitpacking Decompress");
    PerformDecompressionBenchmark(myComp_MTBP, compData, to_string(g_ThreadCount) + " Threads Bitpacking Decompress: ");
}

void ReadWriteTest(const vector<int>& myData_)
{
    RETURN_IF_BAD(myData_.size() != g_datasize, "WRONG SIZE");

    for (auto val : myData_)
    {
        RETURN_IF_BAD((val < 0 || val > 100), "VALUE NOT IN RANGE");
    }

    g_data.Clean();
    sleep(1);
    g_data.Create();

    Data data2("./data/mydata", g_datasize);
    vector<int> ret2 = g_data.Read();

    RETURN_IF_BAD(ret2.size() != g_datasize, "WRONG SIZE");

    for (auto val : ret2)
    {
        RETURN_IF_BAD((val < 0 || val > 100), "VALUE NOT IN RANGE");
    }

    g_log->Log(LogLevel::INFO, "ReadWrite Test Passed", __LINE__, __FILE__);
}

void CompNDecompBpackTest(const vector<int>& myData_)
{
    BpackCompresser myComp;

    auto compData = myComp.Compress(myData_);

    auto decompData = myComp.Decompress(compData);

    CheckNWrite(myData_, decompData, compData, "bitPacking");

}

void CompNDecompBpackRLTest(const vector<int>& myData_)
{
    BpackRLCompresser myComp(g_datasize);

    auto compData = myComp.Compress(myData_);

    auto decompData = myComp.Decompress(compData);

    CheckNWrite(myData_, decompData, compData, "bitPacking_and_RL");

}

void CompNDecompBpackRLSortedTest(const vector<int>& myData_)
{
    vector<int> data = myData_;

    // partially sort the data 
    if (g_datasize > 2000)
    {
        for (size_t i = 0; i < g_datasize - 2000; i += 2000)
        {
            sort(data.begin() + i, data.begin() + i + 2000);
        }
    }

    BpackRLCompresser myComp(g_datasize);

    auto compData = myComp.Compress(data);

    auto decompData = myComp.Decompress(compData);

    CheckNWrite(data, decompData, compData, "bitPack_and_RL_Sorted");

}

void CompNDecompMulticoreTest(const vector<int>& myData_)
{
    MultiThreadCompresser myComp(g_ThreadCount);

    auto compData = myComp.Compress(myData_);

    auto decompData = myComp.Decompress(compData);

    CheckNWrite(myData_, decompData, compData, "multithread_BitPack");

}

void CheckNWrite(const vector<int>& data_, const vector<int>& decompData_, const vector<uint8_t>& compData_, string type)
{
    RETURN_IF_BAD(decompData_.size() != g_datasize, "WRONG DATA SIZE -" + type);

    // Iterate over the elements of both vectors and compare them
    for (size_t i = 0; i < g_datasize; ++i)
    {
        RETURN_IF_BAD(decompData_[i] != data_[i], "DATA READ WRONG -" + type);
    }

    if (g_toWrite)
    {
        g_data.WriteCompressed(compData_, "./data/" + type);
    }

    g_log->Log(LogLevel::INFO, "Compress + Decompress Test " + type + " Passed", __LINE__, __FILE__);
}

void Print(const vector<int>& vec_)
{
    for (int val : vec_)
    {
        cout << val << " ";
    }

    cout << endl;
}

void PerformCompressionBenchmark(Compresser& comp, const vector<int>& data, string type)
{
    auto start_benchmark_time = chrono::high_resolution_clock::now();
    clock_t start_cpu_time = clock();

    struct tms start_times, end_times;

    // Get CPU time at start
    times(&start_times);

    comp.Compress(data);

    auto end_benchmark_time = chrono::high_resolution_clock::now();
    clock_t end_cpu_time = clock();

    // Get CPU time at end
    times(&end_times);

    double t_benchmark = chrono::duration<double, milli>(end_benchmark_time - start_benchmark_time).count();
    double t_cpu = 1000.0 * (end_cpu_time - start_cpu_time) / CLOCKS_PER_SEC;

    long ticks_per_second = sysconf(_SC_CLK_TCK);
    double t_user = (end_times.tms_utime - start_times.tms_utime) * 1000.0 / ticks_per_second;
    double t_system = (end_times.tms_stime - start_times.tms_stime) * 1000.0 / ticks_per_second;

    PrintTImes(t_benchmark, t_cpu, t_user, t_system, type);
}


void PerformDecompressionBenchmark(Compresser& comp, const vector<uint8_t>& data, string type)
{
    auto start_benchmark_time = chrono::high_resolution_clock::now();
    clock_t start_cpu_time = clock();

    struct tms start_times, end_times;

    // Get CPU time at start
    times(&start_times);

    comp.Decompress(data);

    auto end_benchmark_time = chrono::high_resolution_clock::now();
    clock_t end_cpu_time = clock();

    // Get CPU time at end
    times(&end_times);

    double t_benchmark = chrono::duration<double, milli>(end_benchmark_time - start_benchmark_time).count();
    double t_cpu = 1000.0 * (end_cpu_time - start_cpu_time) / CLOCKS_PER_SEC;

    long ticks_per_second = sysconf(_SC_CLK_TCK);
    double t_user = (end_times.tms_utime - start_times.tms_utime) * 1000.0 / ticks_per_second;
    double t_system = (end_times.tms_stime - start_times.tms_stime) * 1000.0 / ticks_per_second;

    PrintTImes(t_benchmark, t_cpu, t_user, t_system, type);
}

void PrintTImes(double t_benchmark, double t_cpu, double t_user, double t_system, string type)
{
    g_log->Log(LogLevel::INFO, " ", __LINE__, __FILE__);
    g_log->Log(LogLevel::INFO, type, __LINE__, __FILE__);
    g_log->Log(LogLevel::INFO, "Benchmark time:   " + to_string(t_benchmark) + " ms", __LINE__, __FILE__);
    g_log->Log(LogLevel::INFO, "CPU run time:     " + to_string(t_cpu) + " ms", __LINE__, __FILE__);
    g_log->Log(LogLevel::INFO, "User mode time:   " + to_string(t_user) + " ms", __LINE__, __FILE__);
    g_log->Log(LogLevel::INFO, "Kernel mode time: " + to_string(t_system) + " ms", __LINE__, __FILE__);
}
