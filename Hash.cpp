//
// Created by Arda Gencer on 16.08.2025.
//

#include <iostream>
#include "Hash.h"
#include <random>
#include <chrono>

using namespace std;

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
    cout << "Enter hashing type:" << endl;
    cin >> hashType;
    cout << "Enter collision handling method:";
    cin >> collisionHandling;
    if (collisionHandling == 's')   //If we use seperate chaining, we will use the linked list array
    {
        seperateChainingLists = new Node*[size];
        for (int i = 0; i < size; i++)
        {
            seperateChainingLists[i] = new Node(-1, nullptr);
        }
    }
    else   //We will use normal integer array
    {
        hashedElements = new int[size];
        for (int i = 0; i < size; i++)
        {
            hashedElements[i] = -1;
        }
    }
}

void Hash::insert(int elem)
{
    int idx;    //Place we will insert to
    //Determining idx depending on hashing type
    if (hashType == 'o')    //Modulus Hashing
    {
        idx = modulus(elem);
    }
    else if (hashType == 'u')   //Multiplicative hashing
    {
        idx = multiplicative(elem);
    }

    //Inserting and handling collisions
    if (collisionHandling == 's')   //If seperate chaining, we will use linked list array
    {

        if (seperateChainingLists[idx]->value != -1)  //If we need to do collision handling
        {
            Node* current = seperateChainingLists[idx];
            while (current->next)   //Iterate until finding last node on the chain
            {
                current = current->next;
            }
            current->next = new Node(elem, nullptr);    //Insert to the end of the chain
        }
        else   //If we can insert right away (i.e. spot is vacant)
        {
            seperateChainingLists[idx] = new Node(elem, nullptr);   //Insert to array
        }
    }
    else   //Then we will use normal integer array
    {
        if (hashedElements[idx] != -1)  //If we need to do collision handling
        {
            if (collisionHandling == 'l')   //Linear Probing
            {
                while (hashedElements[idx] != -1)
                {
                    idx++;
                }
            }
            else if (collisionHandling == 'q')  //Quadratic Probing
            {
                int iteration = 1;
                while (hashedElements[idx] != -1)
                {
                    idx+= iteration * iteration;
                }
            }
            else if (collisionHandling == 'd')  //Double Hashing
            {
                int i = 0;
                int stepForDoubleHashing = 1 + (elem %  (size - 1));
                while (hashedElements[idx] != -1) {
                    idx = (idx + (i*stepForDoubleHashing)) % size;
                    i++;
                }
            }
        }
        hashedElements[idx] = elem; //Insert the element
    }
}

bool Hash::deleteElem(int elem)
{
    if (!search(elem))
    {
        return false;
    }
}

bool Hash::search(int elem)
{
    int hashIndex;
    if (hashType == 'o')    //Modulus Hashing
        {
        hashIndex = modulus(elem);
        }
    else if (hashType == 'u')   //Multiplicative hashing
        {
        hashIndex = multiplicative(elem);
        }
    if (collisionHandling == 's') {
        Node* current = hashedElements[hashIndex];
        while (current != nullptr) {
            if (current->element == elem)
                return true;
            current = current->next;
        }
        return false;
    }
    if (collisionHandling == 'l') {
        int indexStart = hashIndex;
        while (hashedElements[hashIndex] != -1) {
            if (hashedElements[hashIndex] == elem) {
                return true;
            }
            hashIndex = (hashIndex + 1) % size; //calculate next index tried according to linear probing
            if (hashIndex == indexStart) {
                break;
            }
        }
        return false;
    }
    if (collisionHandling == 'q') {
        int indexStart = hashIndex, iteration = 1;
        while (hashedElements[hashIndex] != -1) {
            if (hashedElements[hashIndex] == elem) {
                return true;
            }
            hashIndex = (hashIndex + iteration * iteration) % size; // calculate next index tried according to quadratic probing
            iteration++;
            if (iteration > size) {
                break;
            }
        }
        return false;
    }
    if (collisionHandling == 'd') {
        int indexStart = hashIndex, i = 0;
        int doubleHashStep = 1 + (elem % (size - 1));
        while (hashedElements[hashIndex] != -1) {
            if (hashedElements[hashIndex] == elem) {
                return true;
            }
            hashIndex = (hashIndex + i * doubleHashStep) % size;
            i++;
            if (size < i) {
                break;
            }
        }
        return false;
    }
    return false;
}

void Hash::resize()
{
}

int Hash::modulus(int key)
{
    //Random values for hash function that  will be determined at the start of program
    static int a = getRandomInt(1,100);
    static int b = getRandomInt(1,100);
    static int p = getRandomInt(1,100);

    int hashedVal = (a * key + b) % p % size;
    cout << hashedVal << endl;
    return hashedVal;
}

int Hash::multiplicative(int key)
{
    static double A = getRandomInt(1,1000)/1000.00; // decided at the start of the program
    int hashedVal = double(key * A - int(key*A)) * size;
    return hashedVal;
}

void Hash::printTable()
{
    if (collisionHandling != 's')
    {
        cout << "[";
        for (int i = 0; i < size - 1; i++)
        {
            cout << hashedElements[i] << ", ";
        }
        cout << hashedElements[size - 1] << "]";
    }
    else
    {
        cout << "{" << endl;
        for (int i = 0; i < size; i++)
        {
            Node* elem = seperateChainingLists[i];
            while (elem->next)
            {
                cout << elem->value << ", ";
                elem = elem->next;
            }
            cout << elem->value << endl;
        }
    }
}
