#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "dictionary.h"

using namespace std;

void lower(string &s);
string strip_punct(const string &s);
void check_spelling(ifstream &in, Dictionary &dict);
vector<string> create_suggestions(Dictionary &dict, string word);

int main(int argc, char *argv[]) {

  // Output usage message if improper command line args were given.
  if (argc != 3) {
    cerr << "Usage: " << argv[0] << " wordlist_filename input_file\n";
    return EXIT_FAILURE;
  }

  ifstream inf(argv[2]);
  if (!inf) {
    cerr << "Could not open " << argv[2] << "\n";
    return EXIT_FAILURE;
  }

  // Read dictionary, but let user know what we are working on.
  cout << "Loading dictionary, this may take awhile...\n";

  Dictionary d(argv[1]);

  check_spelling(inf, d);

  inf.close();

  return EXIT_SUCCESS;
}

void check_spelling(ifstream &in, Dictionary &dict) {

  int line_number = 0;

  while (in) {

    line_number++;

    string line;
    getline(in, line);

    stringstream ss(stringstream::in | stringstream::out);
    ss << line;

    string word;
    while (ss >> word) {
      // 将单词转换为小写并去除标点符号
      lower(word);
      word = strip_punct(word);

      // 检查单词是否在字典中
      if (!dict.search(word)) {
        cout << "Line " << line_number << ": '" << word << "' is misspelled."
             << endl;

        // 生成拼写建议
        vector<string> suggestions = create_suggestions(dict, word);
        if (!suggestions.empty()) {
          cout << "Suggestions: ";
          for (const string &suggestion : suggestions) {
            cout << suggestion << " ";
          }
          cout << endl;
        } else {
          cout << "No suggestions available." << endl;
        }
      }
    }
  }
}

vector<string> create_suggestions(Dictionary &dict, string word) {

  vector<string> suggestions;

  unsigned int i;
  char c;

  // transpose all adjacent letters
  for (i = 0; i < word.length() - 1; i++) {

    string new_word(word);
    char temp = new_word[i];
    new_word[i] = new_word[i + 1];
    new_word[i + 1] = temp;

    if (dict.search(new_word)) {
      suggestions.push_back(new_word);
    }
  }

  // remove each letter
  for (i = 0; i < word.length(); i++) {

    string new_word(word);
    new_word.erase(i, 1);
    if (dict.search(new_word)) {
      suggestions.push_back(new_word);
    }
  }

  // replace each letter
  for (i = 0; i < word.length(); i++) {
    for (c = 'a'; c <= 'z'; c++) {

      string new_word(word);
      new_word.replace(i, 1, 1, c);
      if (dict.search(new_word)) {
        suggestions.push_back(new_word);
      }
    }
  }

  // insert a letter at each position
  for (i = 0; i < word.length(); i++) {
    for (c = 'a'; c <= 'z'; c++) {

      string new_word(word);
      new_word.insert(i, 1, c);
      if (dict.search(new_word)) {
        suggestions.push_back(new_word);
      }
    }
  }

  vector<string> unique_suggestions;
  insert_iterator<vector<string>> ins(unique_suggestions,
                                      unique_suggestions.begin());

  unique_copy(suggestions.begin(), suggestions.end(), ins);

  return unique_suggestions;
}

void lower(string &s) {

  // Ensures that a word is lowercase
  for (unsigned int i = 0; i < s.length(); i++) {
    s[i] = tolower(s[i]);
  }
}

string strip_punct(const string &s) {

  // Remove any single trailing
  // punctuation character from a word.
  if (ispunct(s[s.length() - 1])) {
    return s.substr(0, s.length() - 1);
  } else {
    return s;
  }
}
