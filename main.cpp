#include <iostream>
#include <chrono>
#include <fstream>
#include <random>

#include "Hash.h"

using namespace std;
using chrono::high_resolution_clock;
using chrono::duration_cast;
using chrono::microseconds;

typedef  chrono::time_point<high_resolution_clock> timePoint;
typedef chrono::duration<long long, ratio<1, 1000000>> duration;

extern int probeCount;
extern unsigned int a;
extern unsigned int b;
extern unsigned int p;
Hash table;

void probeVsLoad();
vector<int> generateNormalInput(int size);
vector<int> generateCollisionKeys(int numKeys = 50);
void insertDeleteKeys();

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

int getRandomInteger(const int& min, const int& max) {
    // Static variables to ensure the generator and seeding happen only once
    static std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());

    // Create a distribution for the specified range
    std::uniform_int_distribution<int> dist(min, max);

    // Generate and return the random number
    return dist(rng);
}

int main()
{
    insertDeleteKeys();




    //probeVsLoad();
    return 0;
}

void insertDeleteKeys()
{
    int numberOfKeys;
    cout << "Enter number of keys to insert/ delete";
    cin >> numberOfKeys;
    bool adverserial;
    cout << "Enter adverserial or not:";
    cin >> adverserial;
    vector<int> values;
    if (adverserial)
    {
        values = generateCollisionKeys(numberOfKeys);
    }
    else
    {
        values = generateNormalInput(numberOfKeys);
    }

    timePoint start = high_resolution_clock::now();
    for (int i = 0; i < values.size(); i++)
    {
        table.insert(values.at(i));
    }
    timePoint end = high_resolution_clock::now();
    duration timeTaken = duration_cast<microseconds>(end - start);
    cout << "Total time taken for insertions: " << timeTaken.count()/1000000.00;



    /*//Delete test
    start = high_resolution_clock::now();
    for (int i = 0; i < values.size(); i++)
    {
        table.deleteElem(i);
    }
    end = high_resolution_clock::now();
    timeTaken = duration_cast<microseconds>(end - start);
    cout << "Total time taken to delete: " << timeTaken.count()/1000000.00;*/
}


void probeVsLoad()  //Compares probe count vs load factor
{
    vector<int> values = generateNormalInput(30000);

    vector<int> probes;
    vector<double> loads;
    Hash table;
    for (int i = 0; i < values.size(); i++)
    {
        table.insert(values.at(i));
        probes.push_back(probeCount);
        loads.push_back(table.getLoadFactor());
    }
    writeVectorsToCsv(loads, probes, "output.csv");
}

vector<int> generateCollisionKeys(int numKeys)    //Generates adverserial input
{
    vector<int> keys;
    int baseKey = 12345; // arbitrary starting point
    for (int i = 0; i < numKeys; i++) {
        keys.push_back(baseKey + i * p); // same residue mod p
    }
    return keys;
}

vector<int> generateNormalInput(int size)
{
    vector<int> values;
    for (int i = 0; i < size; i++)
    {
        values.push_back(getRandomInteger(1,INT_MAX));
    }
    return values;
}