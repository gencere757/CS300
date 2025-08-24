#include <iostream>
#include <chrono>

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
    timePoint start = high_resolution_clock::now();
    for (int i = 1; i < 750; i++)
    {
        table.insert(i);
    }
    timePoint end = high_resolution_clock::now();
    duration timeTaken = duration_cast<microseconds>(end - start);
    cout << "Total time taken to insert: " << timeTaken.count() << endl;
    start = high_resolution_clock::now();
    for (int i = 1; i < 700; i++)
    {
        table.deleteElem(i);
    }
    end = high_resolution_clock::now();
    timeTaken = duration_cast<microseconds>(end - start);
    table.printTable();
    cout << "Total time taken to delete: " << timeTaken.count() << endl;
    return 0;
}