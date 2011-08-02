#include <iostream>
#include "Dictionary.hpp"

using namespace std;

Dictionary::Dictionary(int capacity) {
    this->capacity = capacity;
    this->counter = 0;
    this->next = NULL;
    this->words = new char*[capacity];
}

Dictionary::~Dictionary() {
    if(this->next!=NULL) {
        delete this->next;
    }
    delete this->words;
}

int Dictionary::put(const char *word) {
    if(this->counter < this->capacity) {
        this->words[this->counter] = strdup(word);
        return ++this->counter;
    }
    if(this->next == NULL) {
        this->next = new Dictionary(this->capacity);
    }
    return this->capacity + this->next->put(word);
}

const char* Dictionary::get(int idx) {
    if(idx>0 && idx<=this->counter) {
        return this->words[idx-1];
    }
    if(this->next == NULL) {
        return NULL;
    }
    return this->next->get(idx - this->capacity);
}

bool Dictionary::find(const char* word, unsigned int low, unsigned int high) {
    if(low>high) {
        return NULL;
    }
    if(low <= this->capacity) {
        for(int i=low; i<=this->capacity && i<=high; i++) {
            if(strcmp(this->get(i), word)==0) {
                return true;
            }
        }
    }
    if(high > this->capacity && this->next!=NULL) {
        return this->next->find(word, low>capacity?low-capacity:1, high-this->capacity);
    }
    return false;
}

int Dictionary::size() {
    return this->counter + (this->next==NULL ? 0 : this->next->size());
}

unsigned long Dictionary::memUsage() {
    unsigned long mySize = sizeof(Dictionary);
    mySize += capacity * sizeof(char**);
    for(int i = 0; i<this->counter; i++) {
        mySize += strlen(this->words[i])+1;
    }
        
    if(this->next!=NULL) {
        mySize += this->next->memUsage();
    }
    return mySize;
}
