#ifndef Trie_Trie_hpp
#define Trie_Trie_hpp

#include "Dictionary.hpp"

class Trie {
private:
    int MAX_LEN;
    unsigned int* mask;
    unsigned char* map;
    Dictionary* dictionary;
    
    unsigned int min(unsigned int a, unsigned int b);

    unsigned int mapGetLow(unsigned int offset);
    unsigned int mapGetHigh(unsigned int offset);

    void mapSetLow(unsigned int offset, unsigned int val);
    void mapSetHigh(unsigned int offset, unsigned int val);
    
    unsigned int getMaskSize();
    unsigned int getMapSize();
    unsigned int calculateMaskOffset(const char* word);
    unsigned int calculateMapOffset(const char* word);
public:
    Trie(int max);
    ~Trie();
    void add(const char* word);
    bool isWord(const char* word);
    unsigned int memUsage();
};

#endif
