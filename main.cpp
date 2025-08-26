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
extern int resizeCount;
extern unsigned int a;
extern unsigned int b;
extern unsigned int p;
Hash table;

void probeVsLoad();
vector<int> generateNormalInput(int size);
vector<int> generateCollisionKeys(int numKeys = 50);
void insertDeleteKeys();
void elemNumberAndTimeTaken();

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
    //elemNumberAndTimeTaken();
    insertDeleteKeys();
    //probeVsLoad();
    return 0;
}

void insertDeleteKeys()
{
    table.clear();
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
    cout << "Total time taken for insertions: " << timeTaken.count()/1000000.00 << endl;



    //Delete test
    start = high_resolution_clock::now();
    for (int i = 0; i < values.size(); i++)
    {
        table.deleteElem(values.at(i));
    }
    end = high_resolution_clock::now();
    timeTaken = duration_cast<microseconds>(end - start);
    cout << "Total time taken to delete: " << timeTaken.count()/1000000.00 << endl;
    table.printTable();
}


void probeVsLoad()  //Compares probe count vs load factor
{
    table.clear();
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

void elemNumberAndTimeTaken()
{
    int limit;
    cout << "Enter number of elements( upper limit):" << endl;
    cin >> limit;
    vector<double> times;
    vector<int> elemNumbers;
    for (int i = 2; i < limit; i+= 100)
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
        table.clear();
    }
    writeVectorsToCsv(elemNumbers, times, "output.csv");
}