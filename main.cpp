#include <iostream>

#include "Hash.h"

using namespace std;


int main()
{
    //Insert test
    Hash table;
    table.insert(3);
    table.insert(5);
    table.insert(13);
    table.insert(654231);
    table.printTable();
    return 0;
}