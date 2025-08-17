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
    int search(int elem);
    void resize();
    int modulus(int key);   //Hashes the given key

    int* hashedElements;
    int size;
    char hashType;

};



#endif //HASH_H
