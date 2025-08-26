//
// Created by Arda Gencer on 16.08.2025.
//

#ifndef HASH_H
#define HASH_H
#include <vector>

using std::vector;

struct Node
{
    int value;
    Node* next;
    Node();
    Node(int valueOfNode, Node* nextNode) : value(valueOfNode), next(nextNode){}

};

class Hash {
public:
    Hash();
    void insert(const int& elem, bool resizing = true);
    bool deleteElem(const int& elem);
    void printTable() const;
    int search(const int& elem) const;   //Search  for a particular element, return true if found and false if not found
    void clear();   //Clears the table's elements

    double getLoadFactor() const;   //Returns the load factor
    int getSize() const;


private:
    void resize(char type);  //Resize the table
    int modulus(const int& key) const;   //Hashes the given key according to modulus hashing
    int multiplicative(const int& key) const; // hashes given key according to multiplicative hashing


    int* hashedElements;    //Array containing the elements
    int size;   //Size of the table
    int usedSize;   //Used number of slots
    double loadFactor;  //How full the table is
    char hashType;  //o -> modulus hashing, u -> multiplicative hashing
    char collisionHandling; //l -> linear probing, q -> quadratic probing, s -> separate chaining, d -> double hashing
};



#endif //HASH_H
