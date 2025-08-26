#include <iostream>
#include <chrono>
#include <fstream>

#include "Hash.h"

using namespace std;
using chrono::high_resolution_clock;
using chrono::duration_cast;
using chrono::microseconds;

typedef  chrono::time_point<high_resolution_clock> timePoint;
typedef chrono::duration<long long, ratio<1, 1000000>> duration;

extern int probeCount;


void writeVectorsToCsv(const std::vector<int>& vec1, const std::vector<double>& vec2, const std::string& filename) {
    // Open the CSV file for writing
    // std::ofstream is used for writing to files
    std::ofstream outputFile(filename);

    // Check if the file was opened successfully
    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    // Optional: Write a header row
    outputFile << "Column1,Column2" << std::endl;

    // Determine the number of rows to write (assuming both vectors have the same size)
    size_t numRows = std::min(vec1.size(), vec2.size());

    // Write data row by row
    for (size_t i = 0; i < numRows; ++i) {
        outputFile << vec1[i] << "," << vec2[i] << std::endl;
    }

    // Close the file
    outputFile.close();

    std::cout << "Data successfully written to " << filename << std::endl;
}


int main()
{

    vector<int> elemNumbers;
    //Insert test

    vector<double> times;
    Hash table;
    for (int i = 2; i < 30; i+= 1)
    {
        timePoint start = high_resolution_clock::now();
        for (int j = 1; j < i; j++)
        {
            table.insert(j);
        }
        timePoint end = high_resolution_clock::now();
        duration timeTaken = duration_cast<microseconds>(end - start);
        elemNumbers.push_back(i-1);
        times.push_back(timeTaken.count()/1000000.00);
        table.printTable();
        table.clear();
    }



    /*//Delete test
    start = high_resolution_clock::now();
    for (int i = 1; i <85; i++)
    {
        table.deleteElem(i);
    }
    end = high_resolution_clock::now();
    timeTaken = duration_cast<microseconds>(end - start);
    table.printTable();*/
    // writeVectorsToCsv(elemNumbers, times, "output.csv");
    return 0;
}