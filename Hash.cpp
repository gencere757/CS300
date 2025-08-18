//
// Created by Arda Gencer on 16.08.2025.
//

#include "Hash.h"
#include <random>
#include <chrono>

int getRandomInt(int min, int max) {
    // Static variables to ensure the generator and seeding happen only once
    static std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());

    // Create a distribution for the specified range
    std::uniform_int_distribution<int> dist(min, max);

    // Generate and return the random number
    return dist(rng);
}

Hash::Hash()
{
    size = 10;
    hashedElements = new int[size];
    for (int i = 0; i < size; i++)
    {
        hashedElements[i] = -1;
    }
}

void Hash::insert(int elem)
{
    if (hashType == 'o')    //Modulus Hashing
    {
        int idx = modulus(elem);
        if (hashedElements[idx] == -1)  //If the location is vacant
        {
            hashedElements[idx] = elem;
        }
        else   //We need to do collision handling
        {
            idx = collisionHandling(idx);
        }
    }
    else if (hashType == 'u')   //Multiplicative hashing
    {

    }
}

int Hash::collisionHandler(int invalidIdx)
{
    int tempIdx = invalidIdx;
    if (collisionHandling == 'l')
    {
        while (hashedElements[tempIdx] == -1)
        {
            tempIdx++;
        }
    }
    else if (collisionHandling == 'q')
    {
        int iteration = 1;
        while (hashedElements[tempIdx] == -1)
        {
            tempIdx+= iteration * iteration;
        }
    }
    else if (collisionHandling == 's')
    {

    }
}


bool Hash::deleteElem(int elem)
{
}

int Hash::search(int elem)
{
}

void Hash::resize()
{
}

int Hash::modulus(int key)
{
    //Random values for hash function that  will be determined at the start of program
    static int a;
    static int b;
    static int p;
    a = getRandomInt(1,100);
    b = getRandomInt(1,100);
    p = getRandomInt(1,100);

    int hashedVal = (a * key + b) % p % size;
    return hashedVal;
}


