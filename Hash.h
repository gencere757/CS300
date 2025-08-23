//
// Created by Arda Gencer on 16.08.2025.
//

#ifndef HASH_H
#define HASH_H

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
    void insert(const int& elem);
    bool deleteElem(const int& elem);
    void printTable() const;

    void setHashingType();
    void changeParameters(char hashType);

private:
    bool search(const int& elem) const;   //Search  for a particular element, return true if found and false if not found
    void resize(char type);  //Resize the table
    int modulus(const int& key) const;   //Hashes the given key according to modulus hashing
    int multiplicative(const int& key) const; // hashes given key according to multiplicative hashing


    int* hashedElements;    //Array containing the elements
    Node** separateChainingLists;    //An array of linked lists to be used when using separate chaining method
    int size;   //Size of the table
    int usedSize;   //Used number of slots
    double loadFactor;  //How full the table is
    char hashType;  //o -> modulus hashing, u -> multiplicative hashing
    char collisionHandling; //l -> linear probing, q -> quadratic probing, s -> separate chaining, d -> double hashing
};



#endif //HASH_H
