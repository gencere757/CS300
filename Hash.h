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
    void insert(int elem);
    bool deleteElem(int elem);
    void setHashingType();
    void changeParameters(char hashType);

private:
    int search(int elem);   //Search  for a particular element, return -1 if not found
    void resize();  //Resize the table
    int modulus(int key);   //Hashes the given key according to modulus hashing
    int multiplicative(int key); // hashes given key according to multiplicative hashing
    void printTable();

    int* hashedElements;    //Array containing the elements
    Node** seperateChainingLists;    //An array of linked lists to be used when using seperate chaining method
    int size;   //Size of the table
    char hashType;  //o -> modulus hashing, u -> multiplacitive hashing
    char collisionHandling; //l -> linear probing, q -> quadratic probing, s -> seperate chaining, d -> double hashing
    char workMode;  // r -> random, a -> adversarial
};



#endif //HASH_H
