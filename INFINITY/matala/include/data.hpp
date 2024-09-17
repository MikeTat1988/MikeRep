/******************************************************************************
  Project:      HOME_ASSIGNMENT
  Description:  DATA HEADER
  Author:       Michael
  Date:         05/14/23
  Version:      1.0
 ******************************************************************************/

#ifndef HAM_DATA_HPP
#define HAM_DATA_HPP

#include <string>
#include <vector> 

namespace ham
{

class Data
{
public:
    explicit Data(const std::string& dataFilePath, const size_t& dataSize);
    ~Data() = default;

    Data(const Data&) = delete;
    Data& operator=(const Data&) = delete;

    void UpdateSize(size_t new_size_);
    void Create();
    
    std::vector<int> Read();
    void WriteCompressed(const std::vector<uint8_t>& compData, const std::string& compFile);

    // bitpacking
    std::vector<uint8_t> CompressBpack(const std::vector<int>& data);
    std::vector<int> DecompressBpack(const std::vector<uint8_t>& inputBuffer);

    // bitpacking with Run-Length Encoding
    std::vector<uint8_t> CompressBpackRL(const std::vector<int>& data_);
    std::vector<int> DecompressBpackRL(const std::vector<uint8_t>& inputBuffer);

    // multithreaded bitpacking
    std::vector<uint8_t> MultiThreadCompress(int numOfThreads, const std::vector<int>& data);
    std::vector<int> MultiThreadDecompress(int numOfThreads, const std::vector<uint8_t>& compData);

    void Clean();

private:
    std::string m_dataPath;
    size_t m_datasize;
};

}//namespace ham

#endif //HAM_DATA_HPP
