#include <iostream>

#include "Hash.h"

using namespace std;


int main()
{
    //Insert test
    Hash table;
    for (int i = 1; i < 500; i++)
    {
        table.insert(i);
    }
    table.printTable();
    return 0;
}