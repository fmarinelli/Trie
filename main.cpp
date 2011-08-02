#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

#include "Dictionary.hpp"

#include "Trie.hpp"

using namespace std;

Trie trie(5);
Dictionary dictionary(10000);
Dictionary ita(100000);

void process_word(const string &word) {
    const char * w = word.c_str();
    trie.add(w);
    ita.put(w);
    return;
}

void dictionary_word(const string &word) {
    const char * w = word.c_str();
    dictionary.put(w);
    return;
}

int main (int argc, const char * argv[]) {
    cout<<(unsigned int)(clock()/(CLOCKS_PER_SEC/1000.0f))<<"ms\n";

    ifstream words("word.lst");
	if (!words) {
		return 1;
	}
  	for_each( istream_iterator<string>( words ), istream_iterator<string>(), process_word );
  	words.close();
    cout<<(unsigned int)(clock()/(CLOCKS_PER_SEC/1000.0f))<<"ms\n";

    ifstream engwords("words.english.lst");
	if (!engwords) {
		return 1;
	}
  	for_each( istream_iterator<string>( engwords ), istream_iterator<string>(), dictionary_word );
  	engwords.close();

    unsigned int startms = clock()/(CLOCKS_PER_SEC/1000.0f);
    cout<<"Start search eng words:"<<startms<<"ms\n";
    double dict_size = dictionary.size();
    cout<<"Dict size \""<<dict_size<<"\"\n";
    for(int i = 1; i<=dict_size; i++) {
        const char * word = dictionary.get(i);
        if (trie.isWord(word)) {
            cout<<"ERRORE:"<<word<<"\n";
        }
    }

    unsigned int endms = clock()/(CLOCKS_PER_SEC/1000.0f);
    cout<<endms<<"ms\n";
    cout<<(unsigned int)((dict_size*1.0f)/(endms-startms)*1000)<<" (not in) words per sec\n";
    
    startms = clock()/(CLOCKS_PER_SEC/1000.0f);
    cout<<"Start search ita words:"<<startms<<"ms\n";
    double ita_dict_size = ita.size();
    cout<<"Dict size \""<<ita_dict_size<<"\"\n";
    for(int i = 1; i<=ita_dict_size; i++) {
        const char * word = ita.get(i);
        if (!trie.isWord(word)) {
            cout<<"ERRORE:"<<word<<"\n";
        }
    }

    endms = clock()/(CLOCKS_PER_SEC/1000.0f);
    cout<<endms<<"ms\n";
    cout<<(unsigned int)((ita_dict_size*1.0f)/(endms-startms)*1000)<<" (in) words per sec\n";
    
    unsigned int mem = trie.memUsage();
    cout<<"MemUsage:"<<mem/1024/1024<<"mb\n";
    return 0;
}
