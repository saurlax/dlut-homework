#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_

#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "hashset.cpp"
#include "hashset.h"

using namespace std;

class hash_function {
public:
  hash_function() {}

  unsigned int operator()(const string &s) const {
    unsigned int hash = 0;
    for (char c : s) {
      hash = hash * 31 + c;
    }
    return hash;
  }
};

class equality {
public:
  equality() {}
  bool operator()(const string &A, const string &B) const { return (A == B); }
};

class Dictionary : public HashSet<string, hash_function, equality> {
public:
  Dictionary(string wordlist_path) {
    ifstream infile(wordlist_path);
    if (!infile) {
      cerr << "Could not open wordlist file: " << wordlist_path << endl;
      exit(EXIT_FAILURE);
    }

    string word;
    while (infile >> word) {
      insert(word);
    }

    infile.close();
  }
};

#endif
