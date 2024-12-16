/**
 * 实现Huffman编码-译码算法
 */

#include <algorithm>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
using namespace std;

class HuffmanNode {
public:
  char data;
  int freq;
  HuffmanNode *left, *right;

  HuffmanNode(char data, int freq) {
    left = right = nullptr;
    this->data = data;
    this->freq = freq;
  }
};

class HuffmanCode {
  HuffmanNode *root;
  unordered_map<char, string> huffmanCode;

public:
  HuffmanCode(unordered_map<char, int> freqMap) {
    vector<HuffmanNode *> nodes;
    for (auto pair : freqMap) {
      nodes.push_back(new HuffmanNode(pair.first, pair.second));
    }

    while (nodes.size() > 1) {
      sort(nodes.begin(), nodes.end(),
           [](HuffmanNode *a, HuffmanNode *b) { return a->freq < b->freq; });

      HuffmanNode *left = nodes[0];
      HuffmanNode *right = nodes[1];

      HuffmanNode *newNode = new HuffmanNode('\0', left->freq + right->freq);
      newNode->left = left;
      newNode->right = right;

      nodes.erase(nodes.begin());
      nodes.erase(nodes.begin());
      nodes.push_back(newNode);
    }

    root = nodes[0];
    generateHuffmanCode(root, "");
  }

  ~HuffmanCode() { clear(); }

  void clear() {
    if (!root)
      return;
    queue<HuffmanNode *> q;
    q.push(root);
    while (!q.empty()) {
      HuffmanNode *node = q.front();
      q.pop();
      if (node->left) {
        q.push(node->left);
      }
      if (node->right) {
        q.push(node->right);
      }
      delete node;
    }
    root = nullptr;
  }

  static unordered_map<char, int> generateFreq(const string &text) {
    unordered_map<char, int> freqMap;
    for (char ch : text) {
      freqMap[ch]++;
    }
    return freqMap;
  }

  void generateHuffmanCode(HuffmanNode *node, string str) {
    if (!node)
      return;

    if (!node->left && !node->right) {
      huffmanCode[node->data] = str;
    }

    generateHuffmanCode(node->left, str + "0");
    generateHuffmanCode(node->right, str + "1");
  }

  string encode(const string &text) {
    string encodedString = "";
    for (char ch : text) {
      encodedString += huffmanCode[ch];
    }
    return encodedString;
  }

  string decode(const string &binaryString) {
    string decodedString = "";
    HuffmanNode *current = root;
    for (char bit : binaryString) {
      if (bit == '0') {
        current = current->left;
      } else {
        current = current->right;
      }

      if (!current->left && !current->right) {
        decodedString += current->data;
        current = root;
      }
    }
    return decodedString;
  }
};

int main() {
  string text = "this is an example for huffman encoding";
  unordered_map<char, int> freqMap = HuffmanCode::generateFreq(text);

  HuffmanCode huffman(freqMap);

  string encodedString = huffman.encode(text);
  cout << "Encoded string: " << encodedString << endl;

  string decodedString = huffman.decode(encodedString);
  cout << "Decoded string: " << decodedString << endl;

  return 0;
}