//
// Created by Arda Gencer on 16.08.2025.
//

#include <iostream>
#include "Hash.h"
#include <random>
#include <chrono>

using namespace std;

double minResize = 0.3;
double maxResize = 0.85;

unsigned int a = 0;
unsigned int b = 0;
unsigned int p = 0;

int probeCount;

int getRandomInt(const int& min, const int& max) {
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
    loadFactor = 0;
    usedSize = 0;
    hashType = 'o';
    collisionHandling = 'l';
    cout << "Enter hashing type:" << endl;
    cin >> hashType;
    cout << "Enter collision handling method:" << endl"";
    cin >> collisionHandling;
    hashedElements = new int[size];
    for (int i = 0; i < size; i++)
    {
        hashedElements[i] = -1;
    }

}

void Hash::clear()
{
    size  = 10;
    hashedElements = new int[size];
    usedSize = loadFactor = 0;
    for (int i = 0; i < size; i++)
    {
        hashedElements[i] = -1;
    }
}


void Hash::insert(const int& elem, bool resizing)
{
    int idx = 0;    //Place we will insert to
    //Determining idx depending on hashing type
    if (hashType == 'o')    //Modulus Hashing
    {
        if (modulus(elem) < 0)
            cout << "what";
        idx = modulus(elem);
    }
    else if (hashType == 'u')   //Multiplicative hashing
    {
        idx = multiplicative(elem);
    }
    cout << "Trying to insert into position " << idx << endl;

    //Inserting and handling collisions
    int totalProbe = 0;
    if (hashedElements[idx] != -1)  //If we need to do collision handling
    {
        if (collisionHandling == 'l')   //Linear Probing
        {
            cout << "Collision happened," << "table[" << idx << "] is full." << endl;
            cout << "Applying linear probing..." << endl;
            while (hashedElements[idx] != -1)
            {
                idx = (idx + 1) % size;
                totalProbe++;
            }
        }
        else if (collisionHandling == 'q')  //Quadratic Probing
        {
            int iteration = 0;
            cout << "Collision happened," << "table[" << idx << "] is full." << endl;
            cout << "Applying quadratic probing..." << endl;
            while (hashedElements[idx] != -1)
            {
                idx = (idx + iteration * iteration) % size;
                iteration++;
                totalProbe++;
            }
        }
        else if (collisionHandling == 'd') { // Double Hashing
            int h2 = 1 + (elem % (size - 1));
            int ogIdx = idx;

            cout << "Collision happened, table[" << idx << "] is full." << endl;
            cout << "Applying double hashing..." << endl;

            for (int i = 1; i < size; i++) {  // Start from i=1 since i=0 is original position
                idx = (ogIdx + i * h2) % size;
                totalProbe++;

                if (hashedElements[idx] == -1) { // Found empty
                    break;
                }
            }
        }
        cout << "Applied a total of " << totalProbe << " probes." << endl;
        if (resizing)
        {
            probeCount = totalProbe;
        }
    }
    hashedElements[idx] = elem; //Insert the element
    cout << "Inserted to table[" << idx << "]" << endl;

    if (resizing)
    {
        usedSize++;
        loadFactor = double(usedSize) / size;
        if (loadFactor > maxResize)
        {
            resize('e');
        }
    }
    cout << endl;
}

bool Hash::deleteElem(const int& elem)
{
    int idx = search(elem);
    if (idx == -1)  //If element not in table
    {
        cout << "The element " << elem <<" could not be found!" << endl;
        return false;
    }
    hashedElements[idx] = -2;   //Delete the element
    cout << "Deleted the element: " << elem << endl;
    usedSize--;
    loadFactor = double(usedSize) / size;
    if (loadFactor < minResize)
    {
        resize('s');
    }
    return true;
}

int Hash::search(const int& elem) const
{
    int hashIndex = 0;
    if (hashType == 'o')    //Modulus Hashing
    {
        hashIndex = modulus(elem);
    }
    else if (hashType == 'u')   //Multiplicative hashing
    {
        hashIndex = multiplicative(elem);
    }
    if (collisionHandling == 'l') {
        int indexStart = hashIndex;
        while (hashedElements[hashIndex] != -1)
        {
            if (hashedElements[hashIndex] == elem)
            {
                return hashIndex;
            }
            hashIndex = (hashIndex + 1) % size; //calculate next index tried according to linear probing
            if (hashIndex == indexStart)
            {
                break;
            }
        }
        return -1;
    }
    if (collisionHandling == 'q') {
        int iteration = 1;
        while (hashedElements[hashIndex] != -1)
        {
            if (hashedElements[hashIndex] == elem)
            {
                return hashIndex;
            }
            hashIndex = (hashIndex + iteration * iteration) % size; // calculate next index tried according to quadratic probing
            iteration++;
            if (iteration > size)
            {
                break;
            }
        }
        return -1;
    }
    if (collisionHandling == 'd') {
        int h2 = 1 + (elem % (size - 1));
        int ogIdx = hashIndex;

        for (int i = 0; i < size; i++) {
            int currentIdx = (ogIdx + i * h2) % size;

            if (hashedElements[currentIdx] == elem) {
                return currentIdx;
            }
            if (hashedElements[currentIdx] == -1) {
                break;  // the element could not have been placed here.
            }
        }
        return -1;  // Not found
    }
    return -1;
}

void Hash::resize(char type)
{
    // ReSharper disable once CppDFAConstantConditions
    if (type == 'e')    //Enlarge
    {
        // ReSharper disable once CppDFAUnreachableCode
        cout << endl << "Size too small. Enlarging..." << endl << endl;

        int* copy = new int[size * 2];  //Create copy
        for (int i = 0; i < size; i++)  //Copy current elems
        {
            copy[i] = hashedElements[i];
        }
        delete[] hashedElements;
        hashedElements = new int[size*2];
        size *= 2;
        for (int i = 0; i < size; i++) //Initialize the new array
        {
            hashedElements[i] = -1;
        }
        for (int i = 0; i < size/2; i++) //Re-insert the old elements
        {
            if (copy[i] != -1 && copy[i] != -2)
            {
                insert(copy[i], false);
            }
        }
        delete[] copy;    //Clear the copy

        loadFactor = double(usedSize) / size;
        cout << "Size increased to double" << endl;
    }
    else if (type == 's')   //Shrinking
    {
        cout << endl << "Size too large. Shrinking..." << endl << endl;
        int* copy = new int[size];
        for (int i = 0; i < size; i++)
        {
            copy[i] = hashedElements[i];
        }
        delete[] hashedElements;
        hashedElements = new int[size / 2];
        size = size / 2;
        for (int i = 0; i < size; i++)  //Initialize the array again
        {
            hashedElements[i] = -1;
        }
        //Insert all non zero or tombstone elements.
        for (int i = 0; i < size * 2; i++) {
            if (copy[i] != -1 && copy[i] != -2)
            insert(copy[i], false);
        }

        delete[] copy;
        loadFactor = double(usedSize) / size;
        cout << "Size reduced to half" << endl;
    }
    cout << endl;
}

int Hash::modulus(const int& key) const
{
    //Random values for hash function that  will be determined at the start of program




    if (a == 0)
    {
        a = getRandomInt(1,INT_MAX);
    }
    if (b == 0)
    {
        b = getRandomInt(1,INT_MAX);
    }
    if (p == 0)
    {
        p = getRandomInt(1,INT_MAX);
    }

    unsigned int hashedVal = (a * key + b) % p % size;
    return hashedVal;
}

int Hash::multiplicative(const int& key) const
{
    static double A = -1;
    if (A < 0)
    {
        A = getRandomInt(1,INT_MAX)/double(INT_MAX); // decided at the start of the program
    }
    int hashedVal = int(size * (key * A - floor(key * A))); //floor function to ensure its calculated properly
    return hashedVal;
}

void Hash::printTable() const
{
    if (collisionHandling != 's')   //Not Separate  chaining
    {
        cout << "[";
        for (int i = 0; i < size - 1; i++)
        {
            cout << hashedElements[i] << ", ";
        }
        cout << hashedElements[size - 1] << "]" << endl << endl;
    }
}

double Hash::getLoadFactor() const
{
    return loadFactor;
}

int Hash::getSize() const
{
    return size;
}
