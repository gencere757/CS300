//
// Created by Arda Gencer on 16.08.2025.
//

#ifndef HASH_H
#define HASH_H



class Hash {
public:
    Hash();
    void insert(int elem);
    bool deleteElem(int elem);
    void setHashingType();
    void changeParameters(char mode);

private:
    int search(int elem);   //Search  for a particular element, return -1 if not found
    void resize();  //Resize the table
    int modulus(int key);   //Hashes the given key
    int collisionHandler(int elem);

    int* hashedElements;    //Array containing the elements
    int size;   //Size of the table
    char hashType;  //o -> modulus hashing, u -> multiplacitive hashing
    char collisionHandling; //l -> linear probing, q -> quadratic probing, s -> seperate chaining
};



#endif //HASH_H
