#ifndef Trie_Dictionary_hpp
#define Trie_Dictionary_hpp

class Dictionary {
private:
    int counter;
    int capacity;
    char** words;
    Dictionary* next;
    
public:
    Dictionary(int capacity);
    ~Dictionary();
    int put(const char* word);
    const char* get(int idx);
    bool find(const char* word, unsigned int low, unsigned int high);
    int size();
    unsigned long memUsage();
};

#endif
