#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

struct Production {
  string left;
  vector<string> right;
};

class Grammar {
private:
  vector<Production> productions;
  map<string, set<string>> first_sets;
  map<string, set<string>> follow_sets;
  set<string> non_terminals;
  set<string> terminals;
  int new_symbol_counter = 1;

  // 将 RHS 字符串切分为符号序列，支持如下规则：
  // - 非终结符：单个大写字母，后可跟一个撇号（如 E'）
  // - 终结符：id、e、+、*、(、) 等，连续的小写字母按标识符收集
  // - 连写情况：TE' 会被切为 T 和 E'；(E) 会被切为 (、E、)
  vector<string> tokenize(const string &s) {
    vector<string> tokens;
    size_t i = 0, n = s.size();
    auto is_upper = [](char c) { return c >= 'A' && c <= 'Z'; };
    auto is_lower = [](char c) { return c >= 'a' && c <= 'z'; };
    auto is_digit = [](char c) { return c >= '0' && c <= '9'; };

    while (i < n) {
      char c = s[i];
      if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
        ++i;
        continue;
      }

      // 单字符终结符
      if (c == '(' || c == ')' || c == '+' || c == '*' || c == '|' ||
          c == '$') {
        tokens.emplace_back(string(1, c));
        ++i;
        continue;
      }

      // 非终结符：单个大写字母，后可接一个撇号
      if (is_upper(c)) {
        string nt(1, c);
        ++i;
        if (i < n && s[i] == '\'') { // 撇号
          nt.push_back('\'');
          ++i;
        }
        tokens.emplace_back(move(nt));
        continue;
      }

      // 标识符或保留小写终结符（如 id / e 等）
      if (is_lower(c)) {
        string word;
        while (i < n && (is_lower(s[i]) || is_digit(s[i]) || s[i] == '_')) {
          word.push_back(s[i]);
          ++i;
        }
        tokens.emplace_back(move(word));
        continue;
      }

      // 其他字符，按单字符记为一个 token，保证不丢信息
      tokens.emplace_back(string(1, c));
      ++i;
    }

    return tokens;
  }

  bool is_non_terminal(const string &symbol) {
    if (symbol.empty())
      return false;
    if (symbol.length() == 1) {
      return (symbol[0] >= 'A' && symbol[0] <= 'Z');
    }
    if (symbol.length() == 2 && symbol[1] == '\'') {
      return (symbol[0] >= 'A' && symbol[0] <= 'Z');
    }
    return false;
  }

  set<string> calculate_first_of_symbol(const string &symbol) {
    set<string> result;

    if (!is_non_terminal(symbol)) {
      result.insert(symbol);
      return result;
    }

    if (first_sets.find(symbol) != first_sets.end()) {
      return first_sets[symbol];
    }

    for (const auto &prod : productions) {
      if (prod.left == symbol) {
        bool all_have_epsilon = true;

        for (const string &x : prod.right) {
          set<string> first_x = calculate_first_of_symbol(x);

          for (const string &f : first_x) {
            if (f != "e") {
              result.insert(f);
            }
          }

          if (first_x.find("e") == first_x.end()) {
            all_have_epsilon = false;
            break;
          }
        }

        if (all_have_epsilon) {
          result.insert("e");
        }
      }
    }

    first_sets[symbol] = result;
    return result;
  }

  set<string> calculate_first_of_string(const vector<string> &symbols) {
    set<string> result;

    for (const string &symbol : symbols) {
      set<string> first_symbol = calculate_first_of_symbol(symbol);

      for (const string &f : first_symbol) {
        if (f != "e") {
          result.insert(f);
        }
      }

      if (first_symbol.find("e") == first_symbol.end()) {
        return result;
      }
    }

    result.insert("e");
    return result;
  }

  void gen_first() {
    first_sets.clear();
    for (const string &nt : non_terminals) {
      calculate_first_of_symbol(nt);
    }
  }

  void gen_follow() {
    follow_sets.clear();
    for (const string &nt : non_terminals) {
      follow_sets[nt] = set<string>();
    }

    if (!non_terminals.empty()) {
      string start_symbol = *non_terminals.begin();
      follow_sets[start_symbol].insert("$");
    }

    bool changed = true;
    while (changed) {
      changed = false;

      for (const auto &prod : productions) {
        for (size_t i = 0; i < prod.right.size(); ++i) {
          string current = prod.right[i];

          if (!is_non_terminal(current))
            continue;

          size_t old_size = follow_sets[current].size();

          if (i + 1 < prod.right.size()) {
            vector<string> beta(prod.right.begin() + i + 1, prod.right.end());
            set<string> first_beta = calculate_first_of_string(beta);

            for (const string &f : first_beta) {
              if (f != "e") {
                follow_sets[current].insert(f);
              }
            }

            if (first_beta.find("e") != first_beta.end()) {
              for (const string &f : follow_sets[prod.left]) {
                follow_sets[current].insert(f);
              }
            }
          } else {
            for (const string &f : follow_sets[prod.left]) {
              follow_sets[current].insert(f);
            }
          }

          if (follow_sets[current].size() != old_size) {
            changed = true;
          }
        }
      }
    }
  }

  string generate_new_non_terminal(const string &base) {
    string new_symbol = base + to_string(new_symbol_counter++);
    while (non_terminals.find(new_symbol) != non_terminals.end()) {
      new_symbol = base + to_string(new_symbol_counter++);
    }
    non_terminals.insert(new_symbol);
    return new_symbol;
  }

  vector<string>
  find_common_prefix(const vector<vector<string>> &alternatives) {
    if (alternatives.size() < 2)
      return {};

    vector<string> common_prefix;
    size_t min_length = alternatives[0].size();

    for (const auto &alt : alternatives) {
      min_length = min(min_length, alt.size());
    }

    for (size_t i = 0; i < min_length; ++i) {
      string current_symbol = alternatives[0][i];
      bool all_same = true;

      for (const auto &alt : alternatives) {
        if (alt[i] != current_symbol) {
          all_same = false;
          break;
        }
      }

      if (all_same) {
        common_prefix.push_back(current_symbol);
      } else {
        break;
      }
    }

    return common_prefix;
  }

  void eliminate_immediate_left_recursion(const string &non_terminal) {
    vector<vector<string>> alpha_productions;
    vector<vector<string>> beta_productions;

    for (auto it = productions.begin(); it != productions.end();) {
      if (it->left == non_terminal) {
        if (!it->right.empty() && it->right[0] == non_terminal) {
          vector<string> alpha(it->right.begin() + 1, it->right.end());
          alpha_productions.push_back(alpha);
        } else {
          beta_productions.push_back(it->right);
        }
        it = productions.erase(it);
      } else {
        ++it;
      }
    }

    if (alpha_productions.empty()) {
      for (const auto &beta : beta_productions) {
        Production prod;
        prod.left = non_terminal;
        prod.right = beta;
        productions.push_back(prod);
      }
      return;
    }

    string new_symbol = generate_new_non_terminal(non_terminal + "'");

    for (const auto &beta : beta_productions) {
      Production new_prod;
      new_prod.left = non_terminal;
      new_prod.right = beta;
      new_prod.right.push_back(new_symbol);
      productions.push_back(new_prod);
    }

    for (const auto &alpha : alpha_productions) {
      Production new_prod;
      new_prod.left = new_symbol;
      new_prod.right = alpha;
      new_prod.right.push_back(new_symbol);
      productions.push_back(new_prod);
    }

    Production epsilon_prod;
    epsilon_prod.left = new_symbol;
    epsilon_prod.right.push_back("e");
    productions.push_back(epsilon_prod);
  }

  void substitute_productions(const string &ai, const string &aj) {
    vector<Production> new_productions;

    for (auto it = productions.begin(); it != productions.end();) {
      if (it->left == ai && !it->right.empty() && it->right[0] == aj) {
        vector<string> gamma(it->right.begin() + 1, it->right.end());

        for (const auto &prod : productions) {
          if (prod.left == aj) {
            Production new_prod;
            new_prod.left = ai;
            new_prod.right = prod.right;
            new_prod.right.insert(new_prod.right.end(), gamma.begin(),
                                  gamma.end());
            new_productions.push_back(new_prod);
          }
        }

        it = productions.erase(it);
      } else {
        ++it;
      }
    }

    productions.insert(productions.end(), new_productions.begin(),
                       new_productions.end());
  }

public:
  Grammar(const string &filename) {
    ifstream file(filename);
    string line;

    while (getline(file, line) && line != "end") {
      if (line.empty())
        continue;

      size_t arrow_pos = line.find(" -> ");
      if (arrow_pos == string::npos)
        continue;

      string left = line.substr(0, arrow_pos);
      string right_part = line.substr(arrow_pos + 4);

      non_terminals.insert(left);

      vector<string> alternatives;
      size_t pos = 0;
      while ((pos = right_part.find(" | ")) != string::npos) {
        alternatives.push_back(right_part.substr(0, pos));
        right_part.erase(0, pos + 3);
      }
      alternatives.push_back(right_part);

      for (const string &alt : alternatives) {
        Production prod;
        prod.left = left;

        vector<string> symbols = tokenize(alt);
        for (const string &symbol : symbols) {
          if (symbol.empty())
            continue;
          prod.right.push_back(symbol);
          if (!is_non_terminal(symbol) && symbol != "e" && symbol != "->" &&
              symbol != "|") {
            terminals.insert(symbol);
          }
        }

        productions.push_back(prod);
      }
    }
    file.close();

    gen_first();
    gen_follow();
  }

  void print() {

    map<string, vector<vector<string>>> grouped;
    for (const auto &prod : productions) {
      grouped[prod.left].push_back(prod.right);
    }

    for (const auto &group : grouped) {
      cout << group.first << " -> ";
      bool first_item = true;
      for (const auto &right : group.second) {
        if (!first_item)
          cout << " | ";
        for (size_t i = 0; i < right.size(); ++i) {
          if (i > 0)
            cout << " ";
          cout << right[i];
        }
        first_item = false;
      }
      cout << "\n";
    }
    cout << "\n";
  }

  Grammar left_recursion() {
    Grammar result = *this;
    vector<string> ordered_non_terminals(result.non_terminals.begin(),
                                         result.non_terminals.end());
    sort(ordered_non_terminals.begin(), ordered_non_terminals.end());

    for (size_t i = 0; i < ordered_non_terminals.size(); ++i) {
      string ai = ordered_non_terminals[i];

      for (size_t j = 0; j < i; ++j) {
        string aj = ordered_non_terminals[j];
        result.substitute_productions(ai, aj);
      }

      result.eliminate_immediate_left_recursion(ai);
    }

    result.gen_first();
    result.gen_follow();
    return result;
  }

  Grammar left_factor() {
    Grammar result = *this;
    bool changed = true;

    while (changed) {
      changed = false;
      map<string, vector<vector<string>>> grouped;

      for (const auto &prod : result.productions) {
        grouped[prod.left].push_back(prod.right);
      }

      for (auto &group : grouped) {
        string left_symbol = group.first;
        vector<vector<string>> &alternatives = group.second;

        if (alternatives.size() < 2)
          continue;

        vector<string> common_prefix = result.find_common_prefix(alternatives);

        if (!common_prefix.empty()) {
          changed = true;

          string new_symbol =
              result.generate_new_non_terminal(left_symbol + "'");

          result.productions.erase(
              remove_if(result.productions.begin(), result.productions.end(),
                        [&left_symbol](const Production &p) {
                          return p.left == left_symbol;
                        }),
              result.productions.end());

          Production new_main_prod;
          new_main_prod.left = left_symbol;
          new_main_prod.right = common_prefix;
          new_main_prod.right.push_back(new_symbol);
          result.productions.push_back(new_main_prod);

          for (const auto &alt : alternatives) {
            Production new_alt_prod;
            new_alt_prod.left = new_symbol;

            if (alt.size() > common_prefix.size()) {
              new_alt_prod.right =
                  vector<string>(alt.begin() + common_prefix.size(), alt.end());
            } else {
              new_alt_prod.right.push_back("e");
            }

            result.productions.push_back(new_alt_prod);
          }

          break;
        }
      }
    }

    result.gen_first();
    result.gen_follow();
    return result;
  }

  void print_first() {
    for (const string &nt : non_terminals) {
      cout << "FIRST(" << nt << ") = { ";
      bool first = true;
      for (const string &symbol : first_sets[nt]) {
        if (!first)
          cout << ", ";
        cout << symbol;
        first = false;
      }
      cout << " }\n";
    }
    cout << "\n";
  }

  void print_follow() {
    for (const string &nt : non_terminals) {
      cout << "FOLLOW(" << nt << ") = { ";
      bool first = true;
      for (const string &symbol : follow_sets[nt]) {
        if (!first)
          cout << ", ";
        cout << symbol;
        first = false;
      }
      cout << " }\n";
    }
    cout << "\n";
  }
};
int main() {
  Grammar grammar("input.txt");

  cout << "Original Grammar:\n";
  grammar.print();

  Grammar after_lr = grammar.left_recursion();
  cout << "After Eliminating Left Recursion:\n";
  after_lr.print();

  Grammar after_lf = after_lr.left_factor();
  cout << "After Left Factoring:\n";
  after_lf.print();

  cout << "FIRST Sets:\n";
  after_lf.print_first();
  cout << "FOLLOW Sets:\n";
  after_lf.print_follow();

  return 0;
}