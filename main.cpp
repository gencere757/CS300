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
extern unsigned static int a;
extern unsigned static int b;
extern unsigned static int p;


void probeVsLoad();


void writeVectorsToCsv(const std::vector<double>& vec1, const std::vector<int>& vec2, const std::string& filename) {
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

    /*vector<int> probes;
    //Insert test

    vector<double> loads;
    Hash table;
    for (int i = 2; i < 30000; i+= 100)
    {
        table.insert(i);
        // timePoint start = high_resolution_clock::now();
        // timePoint end = high_resolution_clock::now();
        // duration timeTaken = duration_cast<microseconds>(end - start);
        probes.push_back(probeCount);
        loads.push_back(table.getLoadFactor());
    }*/



    /*//Delete test
    timePoint start = high_resolution_clock::now();
    for (int i = 1; i <85; i++)
    {
        table.deleteElem(i);
    }
    timePoint end = high_resolution_clock::now();
    duration timeTaken = duration_cast<microseconds>(end - start);
    table.printTable();*/
    // writeVectorsToCsv(loads, probes, "output.csv");
    probeVsLoad();
    return 0;
}

void probeVsLoad()
{
    vector<int> probes;
    //Insert test

    vector<double> loads;
    Hash table;
    for (int i = 2; i < 30000; i+= 100)
    {
        table.insert(i);
        probes.push_back(probeCount);
        loads.push_back(table.getLoadFactor());
    }
    writeVectorsToCsv(loads, probes, "output.csv");
}

std::vector<int> generateCollisionKeys(int targetBucket, int size, int numKeys = 50) {
    std::vector<int> collisionKeys;

    // Generate keys that should all hash to targetBucket
    for (int attempt = 0; attempt < numKeys * 10 && collisionKeys.size() < numKeys; attempt++) {
        // Try key = attempt and see if it hashes to targetBucket
        int key = attempt + 1;
        unsigned int hashedVal = ((unsigned long long)a * key + b) % p % size;

        if (hashedVal == targetBucket) {
            collisionKeys.push_back(key);
        }
    }

    return collisionKeys;
}