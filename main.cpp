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


int main()
{


    //Insert test
    Hash table;
    vector<int>& probes = table.probes;
    vector<double> loads;
    timePoint start = high_resolution_clock::now();
    for (int i = 1; i < 120; i++)
    {
        table.insert(i);
        loads.push_back(table.getLoadFactor());
    }
    timePoint end = high_resolution_clock::now();
    duration timeTaken = duration_cast<microseconds>(end - start);
    cout << "Total time taken to insert: " << timeTaken.count()/1000000.00 << endl;
    table.printTable();


    //Delete test
    start = high_resolution_clock::now();
    for (int i = 1; i <85; i++)
    {
        table.deleteElem(i);
    }
    end = high_resolution_clock::now();
    timeTaken = duration_cast<microseconds>(end - start);
    table.printTable();
    cout << "Total time taken to delete: " << timeTaken.count()/1000000.00 << endl;
    return 0;
}