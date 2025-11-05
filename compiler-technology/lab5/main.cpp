/*
  SLR 分析器实现（C++）
  - 支持文法（默认包含 + - * / 四则运算）:
    E -> E + T
    E -> E - T
    E -> T
    T -> T * F
    T -> T / F
    T -> F
    F -> ( E )
    F -> id
  - 从文法构造 LR(0) 项目集并生成 SLR(1) 表 (ACTION/GOTO)
  - 从 input.txt 读取输入串（如: id+id*(id+id)），末尾可加 $ 或 #
  - 打印 shift/reduce/accept/action 序列
  - 遇错时尝试单 token 删除或单 token 插入恢复（优先删除），并输出提示
*/

#include <bits/stdc++.h>
using namespace std;

struct Production {
  string lhs;
  vector<string> rhs;
  Production() {}
  Production(string L, vector<string> R) : lhs(L), rhs(R) {}
};

// LR(0) item: production index and dot position
struct Item {
  int p;
  int dot;
  bool operator<(Item const &o) const {
    if (p != o.p)
      return p < o.p;
    return dot < o.dot;
  }
  bool operator==(Item const &o) const { return p == o.p && dot == o.dot; }
};

string join(const vector<string> &v) {
  string s;
  for (size_t i = 0; i < v.size(); ++i) {
    if (i)
      s += " ";
    s += v[i];
  }
  return s;
}

// tokenize input: treat sequences of letters/digits as "id" (统一为 token "id")
// tokens: id + - * / ( ) $
vector<string> tokenize(const string &s) {
  vector<string> res;
  int n = s.size();
  for (int i = 0; i < n;) {
    if (isspace((unsigned char)s[i])) {
      ++i;
      continue;
    }
    char c = s[i];
    if (isalpha((unsigned char)c) || isdigit((unsigned char)c)) {
      // read identifier / number -> token "id"
      int j = i;
      while (j < n && (isalnum((unsigned char)s[j]) || s[j] == '_'))
        ++j;
      res.push_back("id");
      i = j;
    } else {
      string t;
      t.push_back(c);
      if (c == '#' || c == '$') {
        res.push_back("$");
      } else {
        res.push_back(t);
      }
      ++i;
    }
  }
  if (res.empty() || res.back() != "$")
    res.push_back("$");
  return res;
}

// Grammar builder: default grammar with + - * / per扩展要求
void buildDefaultGrammar(vector<Production> &prods, set<string> &V,
                         set<string> &T) {
  prods.clear();
  // Augmented: S' -> E
  // productions order matters; we'll keep 0 as S'->E
  prods.push_back(Production("S'", {"E"}));
  // E rules
  prods.push_back(Production("E", {"E", "+", "T"}));
  prods.push_back(Production("E", {"E", "-", "T"}));
  prods.push_back(Production("E", {"T"}));
  // T rules
  prods.push_back(Production("T", {"T", "*", "F"}));
  prods.push_back(Production("T", {"T", "/", "F"}));
  prods.push_back(Production("T", {"F"}));
  // F rules
  prods.push_back(Production("F", {"(", "E", ")"}));
  prods.push_back(Production("F", {"id"}));

  // nonterminals
  V = {"S'", "E", "T", "F"};
  // terminals
  T = {"+", "-", "*", "/", "(", ")", "id", "$"};
}

// closure of an item set
set<Item> closure(const set<Item> &I, const vector<Production> &prods) {
  set<Item> C = I;
  bool added = true;
  while (added) {
    added = false;
    for (auto it = C.begin(); it != C.end(); ++it) {
      Item item = *it;
      int p = item.p;
      int dot = item.dot;
      if (dot < (int)prods[p].rhs.size()) {
        string B = prods[p].rhs[dot];
        // if B is a nonterminal, add its productions with dot=0
        bool isNon = false;
        for (auto &pr : prods) {
          // simple check: if lhs equals B then B is nonterminal
          if (pr.lhs == B) {
            isNon = true;
            break;
          }
        }
        if (isNon) {
          for (int i = 0; i < (int)prods.size(); ++i) {
            if (prods[i].lhs == B) {
              Item ni{i, 0};
              if (!(C.count(ni))) {
                C.insert(ni);
                added = true;
              }
            }
          }
        }
      }
    }
  }
  return C;
}

// goto(I,X)
set<Item> goto_set(const set<Item> &I, const string &X,
                   const vector<Production> &prods) {
  set<Item> J;
  for (auto it = I.begin(); it != I.end(); ++it) {
    Item itx = *it;
    int p = itx.p;
    int dot = itx.dot;
    if (dot < (int)prods[p].rhs.size() && prods[p].rhs[dot] == X) {
      J.insert(Item{p, dot + 1});
    }
  }
  return closure(J, prods);
}

// compute FIRST (only for terminals and nonterminals convenience in FOLLOW)
void computeFirst(const vector<Production> &prods, set<string> &V,
                  set<string> &T, unordered_map<string, set<string>> &FIRST) {
  // terminals: FIRST(t) = {t}
  for (auto &t : T)
    FIRST[t].insert(t);
  for (auto &v : V)
    FIRST[v]; // create
  bool changed = true;
  while (changed) {
    changed = false;
    for (auto &p : prods) {
      auto &A = p.lhs;
      auto &beta = p.rhs;
      // for each production A -> X1 X2 ... Xk
      for (size_t i = 0; i < beta.size(); ++i) {
        string Xi = beta[i];
        size_t before = FIRST[A].size();
        // add FIRST(Xi) \ {epsilon} ; we do not have epsilon in this grammar
        for (auto &x : FIRST[Xi]) {
          if (x != "ε")
            FIRST[A].insert(x);
        }
        if (FIRST[A].size() != before)
          changed = true;
        // if Xi cannot derive epsilon, break (no epsilon handling needed here)
        break;
      }
    }
  }
}

// compute FOLLOW sets (SLR uses FOLLOW of nonterminals)
void computeFollow(const vector<Production> &prods, set<string> &V,
                   set<string> &T, unordered_map<string, set<string>> &FIRST,
                   unordered_map<string, set<string>> &FOLLOW) {
  for (auto &A : V)
    FOLLOW[A];              // init
  FOLLOW["S'"].insert("$"); // S' is start
  bool changed = true;
  while (changed) {
    changed = false;
    for (auto &p : prods) {
      string A = p.lhs;
      auto &beta = p.rhs;
      for (int i = 0; i < (int)beta.size(); ++i) {
        string B = beta[i];
        // if B is nonterminal
        if (V.count(B)) {
          // compute FIRST of beta[i+1..]
          set<string> first_of_rest;
          if (i + 1 >= (int)beta.size()) {
            // add FOLLOW(A) to FOLLOW(B)
            size_t before = FOLLOW[B].size();
            for (auto &x : FOLLOW[A])
              FOLLOW[B].insert(x);
            if (FOLLOW[B].size() != before)
              changed = true;
          } else {
            string X1 = beta[i + 1];
            // add FIRST(X1) excluding epsilon to FOLLOW(B)
            size_t before = FOLLOW[B].size();
            for (auto &t : FIRST[X1]) {
              if (t != "ε")
                FOLLOW[B].insert(t);
            }
            if (FOLLOW[B].size() != before)
              changed = true;
            // (we ignore epsilon chain because grammar has no epsilon)
          }
        }
      }
    }
  }
}

// Build canonical collection of LR(0) items and ACTION/GOTO tables (SLR)
void buildParsingTable(const vector<Production> &prods, set<string> &V,
                       set<string> &T, vector<set<Item>> &states,
                       map<pair<int, string>, pair<string, int>>
                           &ACTION, // ("s",t) or ("r",prodIdx) or ("acc",0)
                       map<pair<int, string>, int> &GOTO) {
  // construct C = { closure({S'->.E}) ...}
  states.clear();
  set<Item> I0;
  I0.insert(Item{0, 0});
  states.push_back(closure(I0, prods));
  bool added = true;
  while (added) {
    added = false;
    for (int i = 0; i < (int)states.size(); ++i) {
      auto &I = states[i];
      // for every grammar symbol X
      set<string> symbols;
      for (auto &it : I) {
        const Production &p = prods[it.p];
        if (it.dot < (int)p.rhs.size())
          symbols.insert(p.rhs[it.dot]);
      }
      for (auto &X : symbols) {
        set<Item> J = goto_set(I, X, prods);
        if (J.empty())
          continue;
        // check if J is in states
        int idx = -1;
        for (int k = 0; k < (int)states.size(); ++k) {
          if (states[k] == J) {
            idx = k;
            break;
          }
        }
        if (idx == -1) {
          idx = states.size();
          states.push_back(J);
          added = true;
        }
        // if X is terminal: ACTION[i, X] = shift idx
        if (T.count(X)) {
          ACTION[{i, X}] = make_pair("s", idx);
        } else {
          // nonterminal: GOTO
          GOTO[{i, X}] = idx;
        }
      }
    }
  }

  // compute FIRST/FOLLOW for SLR reductions
  unordered_map<string, set<string>> FIRST, FOLLOW;
  computeFirst(prods, V, T, FIRST);
  computeFollow(prods, V, T, FIRST, FOLLOW);

  // reductions: for each state i, for each item A->alpha. in I, for each a in
  // FOLLOW(A) set ACTION[i,a]=reduce A->alpha
  for (int i = 0; i < (int)states.size(); ++i) {
    auto &I = states[i];
    for (auto &it : I) {
      const Production &p = prods[it.p];
      if (it.dot == (int)p.rhs.size()) {
        // A->alpha.
        if (p.lhs == "S'") {
          // accept on $
          ACTION[{i, "$"}] = make_pair("acc", 0);
        } else {
          // for all a in FOLLOW(A)
          for (auto &a : FOLLOW[p.lhs]) {
            // if no entry or entry is same reduce, we set reduce
            // In case shift/reduce conflict, our simple implementation keeps
            // existing shift (so S/R priority to shift), but textbook SLR may
            // differ; for typical expr grammar SLR should be conflict-free.
            auto key = make_pair(i, a);
            if (ACTION.find(key) == ACTION.end()) {
              ACTION[key] = make_pair("r", it.p);
            } else {
              // conflict (skip or keep existing)
              // keep existing (prefer shift)
              // no-op
            }
          }
        }
      }
    }
  }
}

// simulate parsing from a given stack and position without printing, to test
// recovery options
bool simulateParsing(const vector<int> &stateStack,
                     const vector<string> &tokens, int pos,
                     const vector<Production> &prods,
                     map<pair<int, string>, pair<string, int>> &ACTION,
                     map<pair<int, string>, int> &GOTO) {
  vector<int> st = stateStack;
  int p = pos;
  int steps = 0;
  while (steps < 10000) {
    int s = st.back();
    string a = (p < (int)tokens.size() ? tokens[p] : "$");
    auto it = ACTION.find({s, a});
    if (it == ACTION.end())
      return false;
    auto op = it->second;
    if (op.first == "s") {
      st.push_back(op.second);
      ++p;
    } else if (op.first == "r") {
      int prodIndex = op.second;
      int rhslen = prods[prodIndex].rhs.size();
      for (int k = 0; k < rhslen; ++k)
        st.pop_back();
      int t = st.back();
      string A = prods[prodIndex].lhs;
      // goto
      auto gt = GOTO.find({t, A});
      if (gt == GOTO.end())
        return false;
      st.push_back(gt->second);
    } else if (op.first == "acc") {
      return true;
    } else
      return false;
    ++steps;
  }
  return false;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  // read input.txt fully
  ifstream ifs("input.txt");
  if (!ifs) {
    cerr << "Cannot open input.txt\n";
    return 1;
  }
  string content;
  {
    string line;
    bool first = true;
    while (getline(ifs, line)) {
      if (!first)
        content += "\n";
      content += line;
      first = false;
    }
  }

  // build grammar
  vector<Production> prods;
  set<string> V, T;
  buildDefaultGrammar(prods, V, T);

  // construct parsing table
  vector<set<Item>> states;
  map<pair<int, string>, pair<string, int>> ACTION;
  map<pair<int, string>, int> GOTO;
  buildParsingTable(prods, V, T, states, ACTION, GOTO);

  // tokenize input
  vector<string> tokens = tokenize(content);
  // accept both '#' or '$' by mapping to '$' in tokenizer already
  // print initial information
  cout << "Input tokens: ";
  for (auto &tk : tokens)
    cout << tk << ' ';
  cout << "\n\n";

  // parse with standard LR algorithm + recovery
  vector<int> stackStates;
  stackStates.push_back(0);
  int pos = 0;
  int step = 0;
  while (true) {
    ++step;
    if (step > 10000) {
      cout << "Too many steps, aborting\n";
      break;
    }
    int s = stackStates.back();
    string a = (pos < (int)tokens.size() ? tokens[pos] : "$");
    auto key = make_pair(s, a);
    auto it = ACTION.find(key);
    if (it != ACTION.end()) {
      auto op = it->second;
      if (op.first == "s") {
        cout << "Step " << step << ": shift '" << a << "'\n";
        stackStates.push_back(op.second);
        pos++;
        continue;
      } else if (op.first == "r") {
        int prodIndex = op.second;
        auto &pr = prods[prodIndex];
        cout << "Step " << step << ": reduce by " << pr.lhs << " -> "
             << join(pr.rhs) << "\n";
        int rhslen = pr.rhs.size();
        for (int i = 0; i < rhslen; ++i)
          stackStates.pop_back();
        int t = stackStates.back();
        auto gt = GOTO.find({t, pr.lhs});
        if (gt == GOTO.end()) {
          cout << "Error: no goto from state " << t << " on " << pr.lhs << "\n";
          break;
        }
        stackStates.push_back(gt->second);
        continue;
      } else if (op.first == "acc") {
        cout << "Step " << step << ": accept\n";
        break;
      }
    }
    // Error handling: try recovery by deletion or insertion of a single token
    cout << "Step " << step << ": syntax error at token '" << a << "' (state "
         << s << ")\n";
    // 1) try deletion of current token
    vector<string> tokens_deleted = tokens;
    if (pos < (int)tokens_deleted.size())
      tokens_deleted.erase(tokens_deleted.begin() + pos);
    bool okDelete =
        simulateParsing(stackStates, tokens_deleted, pos, prods, ACTION, GOTO);
    if (okDelete) {
      cout << "Recovery: deleting token '" << a << "' and continuing\n";
      tokens = tokens_deleted;
      // do not advance pos (since we deleted current), continue loop
      continue;
    }
    // 2) try insertion: try inserting each terminal that might be expected
    // (from ACTION table keys in this state) gather candidate terminals where
    // ACTION[state,terminal] exists
    set<string> candidates;
    for (auto &entry : ACTION) {
      if (entry.first.first == s)
        candidates.insert(entry.first.second);
    }
    bool inserted = false;
    for (auto &cand : candidates) {
      // do not insert endmarker
      vector<string> tokens_ins = tokens;
      tokens_ins.insert(tokens_ins.begin() + pos, cand);
      if (simulateParsing(stackStates, tokens_ins, pos, prods, ACTION, GOTO)) {
        cout << "Recovery: inserting token '" << cand << "' before '" << a
             << "' and continuing\n";
        tokens = tokens_ins;
        inserted = true;
        break;
      }
    }
    if (inserted)
      continue;
    // else cannot recover
    cout << "Cannot recover from syntax error at token '" << a
         << "'. Analysis aborted.\n";
    break;
  }

  return 0;
}