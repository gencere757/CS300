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
            separateChainingLists[i] = nullptr;
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

void Hash::insert(const int& elem, bool resizing)
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
        if (!separateChainingLists[idx])    //Inserting right away
        {
            separateChainingLists[idx] = new Node(elem, nullptr);   //Insert to array
            usedSize++;
        }
        else   //Need to append to list
        {
            Node* current = separateChainingLists[idx];
            while (current->next)   //Iterate until finding last node on the chain
            {
                current = current->next;
            }
            current->next = new Node(elem, nullptr);    //Insert to the end of the chain
            usedSize++;
        }
    }
    else   //Then we will use normal integer array
    {
        if (hashedElements[idx] != -1)  //If we need to do collision handling
        {
            int totalProbe = 0;
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
            else if (collisionHandling == 'd')  //Double Hashing
            {
                int i = 0, h2 = elem % 7; // we picked m = 7 for h2(k)= k mod m
                cout << "Collision happened," << "table[" << idx << "] is full." << endl;
                cout << "Applying double hashing..." << endl;
                while (hashedElements[idx] != -1)
                {
                    idx = (idx + i * h2) % size; // double hashing  = h(k) + i(h2(k))
                    i++;
                    totalProbe++;
                }
            }
            cout << "Applied a total of " << totalProbe << " probes." << endl;
        }
        hashedElements[idx] = elem; //Insert the element
        cout << "Inserted to table[ " << idx << "]" << endl;
    }

    if (resizing)
    {
        usedSize++;
        loadFactor = double(usedSize) / size;
        if (loadFactor > 0.6)
        {
            resize('e');
        }
    }
}

bool Hash::deleteElem(const int& elem)
{
    int idx = search(elem);
    if (idx == -1)  //If element not in table
    {
        cout << "The element" << elem <<"could not be found!" << endl;
        return false;
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
        hashedElements[idx] = -2;   //Delete the element
    }
    cout << "Deleted the element: " << elem << endl;
    usedSize--;
    loadFactor = double(usedSize) / size;
    if (loadFactor < 0.3)
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
    if (collisionHandling == 's')
    {
        Node* current = separateChainingLists[hashIndex];
        while (current != nullptr)
        {
            if (current->value == elem)
                return hashIndex;
            current = current->next;
        }
        return -1;
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
    if (collisionHandling == 'd')
    {
        int i = 0;
        int doubleHashStep = 1 + (elem % (size - 1));
        while (hashedElements[hashIndex] != -1)
        {
            if (hashedElements[hashIndex] == elem)
            {
                return hashIndex;
            }
            hashIndex = (hashIndex + i * doubleHashStep) % size;
            i++;
            if (size < i)
            {
                break;
            }
        }
        return -1;
    }
    return -1;
}

void Hash::resize(char type)
{
    if (type == 'e')    //Enlarge
    {
        cout << "Size too small. Enlarging..." << endl;
        if (collisionHandling == 's')   //Separate Chaining
        {
            // Save reference to old data
            Node** oldLists = separateChainingLists;
            int oldSize = size;

            // Create new larger table
            size *= 2;
            separateChainingLists = new Node*[size];
            for (int i = 0; i < size; i++) {
                separateChainingLists[i] = nullptr;
            }

            // Reset usedSize for reinsertion
            usedSize = 0;

            // Re-insert all elements from old table
            for (int i = 0; i < oldSize; i++) {
                Node* current = oldLists[i];
                while (current != nullptr) {
                    insert(current->value, false);
                    current = current->next;
                }
            }

            // Clean up old table
            for (int i = 0; i < oldSize; i++) {
                Node* current = oldLists[i];
                while (current != nullptr) {
                    Node* temp = current;
                    current = current->next;
                    delete temp;
                }
            }
            delete[] oldLists;
        }
        else   //Not separate chaining
        {
            int* copy = new int[size * 2];  //Create copy
            for (int i = 0; i < size; i++)  //Copy current elems
            {
                copy[i] = hashedElements[i];
            }
            delete[] hashedElements;
            hashedElements = new int[size*2];
            size *= 2;
            loadFactor = double(usedSize) / size;
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
        }
        loadFactor = double(usedSize) / size;
        cout << "Size increased to double" << endl;
    }
    else if (type == 's')   //Shrinking
    {
        cout << "Size too large. Shrinking..." << endl;
        if (collisionHandling == 's')
        {
            Node** oldArray = new Node*[size];
            for (int i = 0; i < size; i++)  //Iterate over each element in array
            {
                Node* head = separateChainingLists[i];
                Node* current = head;
                Node* newCurrent = oldArray[i];
                if (!current)
                {
                    newCurrent = nullptr;
                    continue;
                }
                while (current->next)   //Iterate over each element in linked list
                {
                    newCurrent->next = new Node(current->value, current->next);
                    current = current->next;
                    newCurrent = newCurrent->next;
                }
                newCurrent->next = new Node(current->value, nullptr);
            }
            //Delete old array
            for (int i = 0; i < size; i++)  //Iterate over each element in array
            {
                Node* head = separateChainingLists[i];
                Node* current = head;
                if (!current)
                {
                    delete current;
                    continue;
                }
                while (current->next)   //Iterate over each element in linked list
                {
                    Node* temp = current;
                    current = current->next;
                    delete temp;
                }
                delete current;
            }
            delete[] separateChainingLists;
            separateChainingLists = new Node*[size / 2 + 1];

            // Re-insert elements into resized table
            for (int i = 0; i < size; i++) {
                Node* head = oldArray[i];
                Node* current = head;
                Node* newCurrent = separateChainingLists[i];
                if (!current) // null node case
                {
                    newCurrent = nullptr;
                    continue;
                }
                while (current->next)   //Iterate over each element in linked list
                {
                    insert(current->value, false);
                    current = current->next;
                }
                insert(current->value, false);
            }

            // Delete oldArray
            for (int i = 0; i < size; i++)  //Iterate over each element in array
            {
                Node* head = oldArray[i];
                Node* current = head;
                if (!current)
                {
                    delete current;
                    continue;
                }
                while (current->next)   //Iterate over each element in linked list
                {
                    Node* temp = current;
                    current = current->next;
                    delete temp;
                }
                delete current;
            }
        }
        else // resizing for hashing methods other than separate chaining (array)
        {
            int* newArray = new int[size];
            for (int i = 0; i < size; i++) {
                newArray[i] = hashedElements[i];
            }
            delete[] hashedElements;
            hashedElements = new int[size / 2];
            for (int i = 0; i < size/2; i++)  //Initialize the array again
            {
                hashedElements[i] = -1;
            }
            //Insert all non zero or tombstone elements.
            for (int i = 0; i < size; i++) {
                if (newArray[i] != -1 && newArray[i] != -2)
                insert(newArray[i], false);
            }

            delete[] newArray;
        }
        size /= 2;
        loadFactor = double(usedSize) / size;
        cout << "Size reduced to half" << endl;
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
    if (collisionHandling != 's')   //Not Separate  chaining
    {
        cout << "[";
        for (int i = 0; i < size - 1; i++)
        {
            cout << hashedElements[i] << ", ";
        }
        cout << hashedElements[size - 1] << "]" << endl;
    }
    else
    {
        cout << "{" << endl;
        for (int i = 0; i < size; i++)
        {
            Node* elem = separateChainingLists[i];
            if (!elem)
            {
                cout << "NULL" << endl;
                continue;
            }
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