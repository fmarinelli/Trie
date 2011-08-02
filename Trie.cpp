#include <iostream>
#include "Dictionary.hpp"
#include "Trie.hpp"

using namespace std;

#define AZ_LENGTH   (1 + (unsigned char)'z'-(unsigned char)'a')

Trie::Trie(int max) {
    MAX_LEN = max;
    
    unsigned int maskSize = sizeof(unsigned int)*getMaskSize();
    mask = (unsigned int*)malloc(maskSize);
    memset(mask, 0, maskSize);
    
    //dictionary cannot contains more than 1million of words
    unsigned int mapSize = 5*getMapSize();
    map = (unsigned char *)malloc(mapSize);
    memset(map, 0, mapSize);
    
    dictionary = new Dictionary(50000);
}

Trie::~Trie() {
    free(mask);
    free(map);
    delete dictionary;
}

unsigned int Trie::getMaskSize() {
    unsigned int size = 1;
    for (int i = 0; i<MAX_LEN-1; i++) {
        size *= AZ_LENGTH;
    }
    return size;
}

unsigned int Trie::getMapSize() {
    unsigned int size = 1;
    for (int i = 0; i<MAX_LEN; i++) {
        size *= AZ_LENGTH;
    }
    return size;
}

unsigned int Trie::calculateMapOffset(const char* word) {
    unsigned int power = 1;
    unsigned int offset = 0;
    for(int i = 0; i<MAX_LEN; i++) {
        offset += (word[i] - 'a')*power;
        power *= AZ_LENGTH;
    }
    return offset;
}

unsigned int Trie::calculateMaskOffset(const char* word) {
    unsigned int power = 1;
    unsigned int offset = 0;
    unsigned int len = min(strlen(word), MAX_LEN) - 1;

    for(int i = 0; i<len; i++) {
        offset += power + ((word[i] - 'a') * power);
        power *= AZ_LENGTH;
    }
    return offset;
}

void Trie::add(const char* word) {
    const unsigned long len = strlen(word);
    if(len < MAX_LEN) {
        unsigned int offset = this->calculateMaskOffset(word);
        unsigned int letter = word[len-1] - 'a';
        this->mask[offset] |= 1<<letter;
        return;
    }
    
    int idx = dictionary->put(word);
    int offset = calculateMapOffset(word);
    unsigned int low = mapGetLow(offset);
    if (low==0) {
        mapSetLow(offset, idx);
    }
    mapSetHigh(offset, idx);
}


bool Trie::isWord(const char* word) {
    const unsigned long len = strlen(word);
    if(len<MAX_LEN) {
        int offset = this->calculateMaskOffset(word);
        unsigned int letter = word[len-1] - 'a';
        return (this->mask[offset] & (1<<letter))!=0;
    }
    
    unsigned int offset = calculateMapOffset(word);
    unsigned int low = mapGetLow(offset);
    if(low == 0) {
        return false;
    }
    unsigned int high = mapGetHigh(offset);
    return dictionary->find(word, low, high);
}

unsigned int Trie::memUsage() {
    int mySize = sizeof(Trie);
    mySize += dictionary->memUsage();
    int maskSize = sizeof(unsigned int)*getMaskSize();
    int mapSize = 5*getMapSize();
    return mySize+maskSize+mapSize;
}

unsigned int Trie::min(unsigned int a, unsigned int b) {
    if(a<b) return a;
    else return b;
}

unsigned int Trie::mapGetLow(unsigned int offset) {
    unsigned char * mapOffset = map + (offset * 5);
    return (*(unsigned int*)mapOffset) & 0x000FFFFF;
}

unsigned int Trie::mapGetHigh(unsigned int offset) {
    unsigned char * mapOffset = map + (offset * 5) + 2;
    return ((*(unsigned int*)mapOffset) & 0x00FFFFF0) >> 4;
}

void Trie::mapSetLow(unsigned int offset, unsigned int val) {
    unsigned char * mapOffset = map + (offset * 5);
    *(unsigned int*)mapOffset = val & 0x000FFFFF;
}

void Trie::mapSetHigh(unsigned int offset, unsigned int val) {
    unsigned char * mapOffset = map + (offset * 5) + 1;
    *(unsigned int*)mapOffset = ((*(unsigned int*)mapOffset) & 0x00000FFF) | ((val & 0x000FFFFF) << 12);
}
