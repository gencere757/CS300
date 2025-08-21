#include <iostream>

#include "Hash.h"

using namespace std;


int main()
{
    //Insert test
    Hash table;
    for (int i = 1; i < 100; i++)
    {
        table.insert(i);
    }
    table.printTable();
    return 0;
}