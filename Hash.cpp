//
// Created by Arda Gencer on 16.08.2025.
//

#include <iostream>
#include "Hash.h"
#include <random>
#include <chrono>

using namespace std;

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
    cout << "Enter collision handling method:";
    cin >> collisionHandling;
    if (collisionHandling == 's')   //If we use seperate chaining, we will use the linked list array
    {
        separateChainingLists = new Node*[size];
        for (int i = 0; i < size; i++)
        {
            separateChainingLists[i] = new Node(-1, nullptr);
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

void Hash::insert(const int& elem)
{
    int idx = 0;    //Place we will insert to
    //Determining idx depending on hashing type
    if (hashType == 'o')    //Modulus Hashing
    {
        idx = modulus(elem);
    }
    else if (hashType == 'u')   //Multiplicative hashing
    {
        idx = multiplicative(elem);
    }
    cout << "Trying to insert into position " << idx << endl;

    //Inserting and handling collisions
    if (collisionHandling == 's')   //If seperate chaining, we will use linked list array
    {

        if (separateChainingLists[idx]->value != -1)  //If we need to do collision handling
        {
            Node* current = separateChainingLists[idx];
            while (current->next)   //Iterate until finding last node on the chain
            {
                current = current->next;
            }
            current->next = new Node(elem, nullptr);    //Insert to the end of the chain
        }
        else   //If we can insert right away (i.e. spot is vacant)
        {
            separateChainingLists[idx] = new Node(elem, nullptr);   //Insert to array
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
                    cout << "Collision happened," << "table[" << idx << "] is full." << endl;
                    cout << "Applying linear probing..." << endl;
                    idx = (idx + 1) % size;
                }
            }
            else if (collisionHandling == 'q')  //Quadratic Probing
            {
                int iteration = 0;
                while (hashedElements[idx] != -1)
                {
                    cout << "Collision happened," << "table[" << idx << "] is full." << endl;
                    cout << "Applying quadratic probing..." << endl;
                    idx = (idx + iteration * iteration) % size;
                    iteration++;
                }
            }
            else if (collisionHandling == 'd')  //Double Hashing
            {
                int i = 0, h2 = elem % 7; // we picked m = 7 for h2(k)= k mod m
                while (hashedElements[idx] != -1)
                {
                    cout << "Collision happened," << "table[" << idx << "] is full." << endl;
                    cout << "Applying double hashing..." << endl;
                    idx = (idx + i * h2) % size; // double hashing  = h(k) + i(h2(k))
                    i++;
                }
            }
        }
        hashedElements[idx] = elem; //Insert the element
    }
    usedSize++;
    loadFactor = double(usedSize) / size;
    if (loadFactor > 0.7)
    {
        resize();
    }
}

bool Hash::deleteElem(const int& elem)
{
    if (!search(elem))  //If element not in table
    {
        cout << "The element could not be found!" << endl;
        return false;
    }
    //If element is found, get its index
    int idx = 0;
    if (hashType == 'o')
    {
        idx = modulus(elem);
    }
    else if (hashType == 'u')
    {
        idx = multiplicative(elem);
    }
    //Depending on collision handling type, delete the element
    if (collisionHandling == 's')
    {
        Node* head = separateChainingLists[idx];    //Head of the current linked list
        Node* current = head;   //Current node we're looking at
        while (current->next->value != elem)  //Iterate until finding the correct node
        {
            current = current->next;
        }
        //Rewire the nodes and delete the correct node
        Node* toBeDeleted = current->next;
        current->next = current->next->next;
        toBeDeleted->next = nullptr;
        delete toBeDeleted;
    }
    else   //Linear probing etc.
    {
        hashedElements[idx] = -1;   //Delete the element
    }
    return true;
}

bool Hash::search(const int& elem) const
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
    if (collisionHandling == 's')
    {
        Node* current = separateChainingLists[hashIndex];
        while (current != nullptr)
        {
            if (current->value == elem)
                return true;
            current = current->next;
        }
        return false;
    }
    if (collisionHandling == 'l') {
        int indexStart = hashIndex;
        while (hashedElements[hashIndex] != -1)
        {
            if (hashedElements[hashIndex] == elem)
            {
                return true;
            }
            hashIndex = (hashIndex + 1) % size; //calculate next index tried according to linear probing
            if (hashIndex == indexStart)
            {
                break;
            }
        }
        return false;
    }
    if (collisionHandling == 'q') {
        int iteration = 1;
        while (hashedElements[hashIndex] != -1)
        {
            if (hashedElements[hashIndex] == elem)
            {
                return true;
            }
            hashIndex = (hashIndex + iteration * iteration) % size; // calculate next index tried according to quadratic probing
            iteration++;
            if (iteration > size)
            {
                break;
            }
        }
        return false;
    }
    if (collisionHandling == 'd')
    {
        int i = 0;
        int doubleHashStep = 1 + (elem % (size - 1));
        while (hashedElements[hashIndex] != -1)
        {
            if (hashedElements[hashIndex] == elem)
            {
                return true;
            }
            hashIndex = (hashIndex + i * doubleHashStep) % size;
            i++;
            if (size < i)
            {
                break;
            }
        }
        return false;
    }
    return false;
}

void Hash::resize(char type)
{
    if (type == 'e')    //Enlarge
    {
        if (collisionHandling == 's')
        {
            Node** newArray = new Node*[size * 2];  //Create a new array of double size
            for (int i = 0; i < size; i++)  //Copy current elems
            {
                newArray[i] = separateChainingLists[i];
            }
            for (int i = size; i < size*2; i++) //Initialize the rest
            {
                newArray[i] = nullptr;
            }
            delete separateChainingLists;
            separateChainingLists = newArray;
        }
        else
        {
            int* newArray = new int[size * 2];  //Create a new array of double size
            for (int i = 0; i < size; i++)  //Copy current elems
            {
                newArray[i] = hashedElements[i];
            }
            for (int i = size; i < size*2; i++) //Initialize the rest
            {
                newArray[i] = -1;
            }
            delete hashedElements;
            hashedElements = newArray;
        }
        size *= 2;
    }
    else if (type == 's')   //Shrink
    {

    }
}

int Hash::modulus(const int& key) const
{
    //Random values for hash function that  will be determined at the start of program
    static int a = getRandomInt(1,100);
    static int b = getRandomInt(1,100);
    static int p = getRandomInt(1, 100);

    int hashedVal = (a * key + b) % p % size;
    return hashedVal;
}

int Hash::multiplicative(const int& key) const
{
    static double A = getRandomInt(1,1000)/1000.00; // decided at the start of the program
    int hashedVal = double(key * A - int(key*A)) * size;
    return hashedVal;
}

void Hash::printTable() const
{
    if (collisionHandling != 's')   //Separate  chaining
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
            Node* elem = separateChainingLists[i];
            while (elem->next)
            {
                cout << elem->value << "->";
                elem = elem->next;
            }
            cout << elem->value << endl;
        }
        cout << "}";
    }
}