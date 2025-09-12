// MicroC 词法分析器（单遍扫描）
// 关键字：for(1) if(2) then(3) else(4) while(5) do(6) until(29) int(30) input(31) output(32)
// ID(10) = letter (letter|digit)*
// NUM(11) = digit digit*
// 运算符/分隔符：
// ':'(17) ':='(18) '+'(13) '-'(14) '*'(15) '/'(16)
// '<'(20) '<>'(21) '<='(22) '>'(23) '>='(24) '='(25)
// ';'(26) '('(27) ')'(28) '#'(0)

#include <iostream>
#include <string>
#include <unordered_map>
#include <cctype>
#include <fstream>
using namespace std;

int token = -1;
string id;
long long num = 0;

// 关键字表
static const unordered_map<string, int> keywords = {
    {"for", 1},
    {"if", 2},
    {"then", 3},
    {"else", 4},
    {"while", 5},
    {"do", 6},
    {"until", 29},
    {"int", 30},
    {"input", 31},
    {"output", 32},
};

static void lexical(istream &in)
{
  id.clear();
  num = 0;
  token = -1;

  // 跳过空白符
  while (true)
  {
    int ch = in.peek();
    if (ch == EOF)
      return;
    if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r')
    {
      in.get();
      continue;
    }
    break;
  }

  int c0 = in.peek();
  if (c0 == EOF)
  {
    // 若无 '#', 以 EOF 作为结束
    token = 0;
    id = "#";
    return;
  }

  char c = in.get();

  if (isalpha(c))
  {
    string lex;
    lex.push_back(c);
    while (true)
    {
      int ch = in.peek();
      if (ch != EOF)
      {
        if (isalpha(ch) || isdigit(ch))
        {
          lex.push_back(in.get());
          continue;
        }
      }
      break;
    }
    // 判断是否关键字
    auto it = keywords.find(lex);
    if (it != keywords.end())
    {
      token = it->second;
      id = lex;
    }
    else
    {
      token = 10; // ID
      id = lex;
    }
    return;
  }

  if (isdigit(c))
  {
    long long val = c - '0';
    while (true)
    {
      int ch = in.peek();
      if (ch != EOF && isdigit(ch))
      {
        val = val * 10 + (in.get() - '0');
      }
      else
        break;
    }
    token = 11; // NUM
    num = val;
    return;
  }

  // 运算符/分隔符
  switch (c)
  {
  case '+':
    token = 13;
    id = "+";
    return;
  case '-':
    token = 14;
    id = "-";
    return;
  case '*':
    token = 15;
    id = "*";
    return;
  case '/':
    token = 16;
    id = "/";
    return;
  case '=':
    token = 25;
    id = "=";
    return;
  case ';':
    token = 26;
    id = ";";
    return;
  case '(':
    token = 27;
    id = "(";
    return;
  case ')':
    token = 28;
    id = ")";
    return;
  case '#':
    token = 0;
    id = "#";
    return;
  case ':':
  {
    int ch = in.peek();
    if (ch == '=')
    {
      in.get();
      token = 18;
      id = ":=";
    }
    else
    {
      token = 17;
      id = ":";
    }
    return;
  }
  case '<':
  {
    int ch = in.peek();
    if (ch == '=')
    {
      in.get();
      token = 22;
      id = "<=";
    }
    else if (ch == '>')
    {
      in.get();
      token = 21;
      id = "<>";
    }
    else
    {
      token = 20;
      id = "<";
    }
    return;
  }
  case '>':
  {
    int ch = in.peek();
    if (ch == '=')
    {
      in.get();
      token = 24;
      id = ">=";
    }
    else
    {
      token = 23;
      id = ">";
    }
    return;
  }
  default:
    token = -1;
    id = string(1, c);
    return;
  }
}

int main(int argc, char *argv[])
{
  const char *path = "input.txt";
  if (argc >= 2)
  {
    path = argv[1];
  }

  ifstream fin;
  fin.open(path, ios::in);
  if (!fin.is_open())
  {
    cerr << "Cannot open file: " << path << "\n";
    return 1;
  }

  istream &in = fin;
  do
  {
    lexical(in);
    switch (token)
    {
    case 11:
      cout << "(" << token << ", " << num << ")\n";
      break;
    case 10:
      cout << "(" << token << ", '" << id << "')\n";
      break;
    case -1:
      cout << "error!\n";
      break;
    default:
      cout << "(" << token << ", " << id << ")\n";
      break;
    }
  } while (token != 0);

  return 0;
}
