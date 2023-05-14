/******************************************************************************
  Project:      HOME_ASSIGNMENT
  Description:  DATA
  Author:       Michael
  Date:         05/14/23
  Version:      1.0
 ******************************************************************************/

#include "data.hpp"
#include "utils.hpp"
#include "w_pqueue.hpp"

#include <fstream>  //ofstream
#include <iostream>
#include <cstdlib>
#include <ctime>    //srand
#include <vector> 
#include <thread>

using namespace ham;
using namespace std;

Data::Data(const string& dataFilePath, const size_t& dataSize) :
    m_dataPath(dataFilePath), m_datasize(dataSize)
{}

void Data::Create()
{
    srand(time(0)); // for random generator

    ofstream file(m_dataPath, ios::binary);

    RETURN_IF_BAD(!file, "Error opening file in Create");

    for (size_t i = 0; i < m_datasize; ++i)
    {
        // generate a random number between 0 and 100 and write it to a file
        int randomValue = rand() % 101;
        file.write(reinterpret_cast<const char*>(&randomValue), sizeof(int));
    }
}

vector<int> Data::Read()
{
    // create an ifstream object for reading data
    ifstream file(m_dataPath, ios::binary);

    if (!file)
    {
        cerr << "Error: opening file for reading." << endl;
        return vector<int>{};
    }

    vector<int> ret;
    int value;

    // read to ret the values
    while (file.read(reinterpret_cast<char*>(&value), sizeof(int)))
    {
        ret.push_back(value);
    }

    return ret;
}

void Data::Clean()
{
    ofstream file;

    // truncate the file from the beginning
    file.open(m_dataPath, ofstream::out | ofstream::trunc);
}

void Data::UpdateSize(size_t new_size_)
{
    m_datasize = new_size_;
}

void Data::WriteCompressed(const vector<uint8_t>& compressedData, const string& compFile)
{
    ofstream outputFile(compFile, ios::binary);

    RETURN_IF_BAD(!outputFile, "Error opening file for writing.");

    outputFile.write(reinterpret_cast<const char*>(compressedData.data()), compressedData.size());
}

