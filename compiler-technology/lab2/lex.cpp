/**
 * 编写一个词法分析器，它针对输入文件，实现以下功能：
 * 1）每遇到你的学号，就输出你的名字，对于其他的串原样输出。
 * 2）统计输入文件中字母的数目。
 * 3）统计输入文件中数字的数目。
 */
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main() {
  ifstream file("input.txt");

  string student_id = "20232241392";
  string student_name = "文浩然";

  string line;
  int word_count = 0;
  int number_count = 0;

  while (getline(file, line)) {
    string output = "";
    int i = 0;

    while (i < line.length()) {
      if (i + student_id.length() <= line.length() &&
          line.substr(i, student_id.length()) == student_id) {
        output += student_name;
        i += student_id.length();
      } else {
        output += line[i];
        i++;
      }
    }

    cout << output << endl;

    // 统计单词和数字
    string word = "";
    for (int j = 0; j <= line.length(); j++) {
      char c = (j < line.length()) ? line[j] : ' '; // 行末视为空格
      if (c != ' ' && c != '\t' && c != '\n') {
        word += c;
      } else {
        if (!word.empty()) {
          bool isNumber = true;
          bool hasLetter = false;
          for (char ch : word) {
            if (!isdigit(ch)) {
              isNumber = false;
            }
            if (isalpha(ch)) {
              hasLetter = true;
            }
          }
          if (isNumber) {
            number_count++;
          } else if (hasLetter) {
            word_count++;
          }
          word = "";
        }
      }
    }
  }

  file.close();
  cout << "单词总数: " << word_count << endl;
  cout << "数字总数: " << number_count << endl;

  return 0;
}
