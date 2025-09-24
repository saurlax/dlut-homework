/**
 * ��дһ���ʷ�������������������ļ���ʵ�����¹��ܣ�
 * 1��ÿ�������ѧ�ţ������������֣����������Ĵ�ԭ�������
 * 2��ͳ�������ļ�����ĸ����Ŀ��
 * 3��ͳ�������ļ������ֵ���Ŀ��
 */
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main() {
  ifstream file("input.txt");

  string student_id = "20232241392";
  string student_name = "�ĺ�Ȼ";

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

    // ͳ�Ƶ��ʺ�����
    string word = "";
    for (int j = 0; j <= line.length(); j++) {
      char c = (j < line.length()) ? line[j] : ' '; // ��ĩ��Ϊ�ո�
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
  cout << "��������: " << word_count << endl;
  cout << "��������: " << number_count << endl;

  return 0;
}
