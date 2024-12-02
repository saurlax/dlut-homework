#include "hashset.h"

using namespace std;

template <typename key_type, typename hash_func, typename key_equal>
bool HashSet<key_type, hash_func, key_equal>::search(const key_type &k) {
  int m = table_size();
  int hash_value = hf(k) % m;

  for (int i = 0; i < m; ++i) {
    int idx = (hash_value + i) % m;
    if ((*ht)[idx].used == 0) {
      return false; // 空槽，表示未找到
    }
    if ((*ht)[idx].used == 1 && eq((*ht)[idx].key, k)) {
      return true; // 找到匹配的键
    }
  }
  return false; // 整个表都查找完了，未找到
}

template <typename key_type, typename hash_func, typename key_equal>
void HashSet<key_type, hash_func, key_equal>::remove(const key_type &k) {
  int m = table_size();
  int hash_value = hf(k) % m;

  for (int i = 0; i < m; ++i) {
    int idx = (hash_value + i) % m;
    if ((*ht)[idx].used == 0) {
      return; // 空槽，表示未找到
    }
    if ((*ht)[idx].used == 1 && eq((*ht)[idx].key, k)) {
      (*ht)[idx].used = -1; // 标记为删除
                            // 使用-1标记删除防止影响线性探测
      --entries;
      return;
    }
  }
}

template <typename key_type, typename hash_func, typename key_equal>
void HashSet<key_type, hash_func, key_equal>::insert(const key_type &k) {
  // 如果负载因子超过 0.5，则扩展哈希表
  if (entries >= table_size() / 2) {
    resize();
  }

  int m = table_size();
  int hash_value = hf(k) % m;

  for (int i = 0; i < m; ++i) {
    int idx = (hash_value + i) % m;
    if ((*ht)[idx].used == 0 || (*ht)[idx].used == -1) {
      (*ht)[idx].key = k;
      (*ht)[idx].used = 1;
      ++entries;
      return;
    }
    if ((*ht)[idx].used == 1 && eq((*ht)[idx].key, k)) {
      return; // 键已存在，不插入重复键
    }
  }
}

template <typename key_type, typename hash_func, typename key_equal>
int HashSet<key_type, hash_func, key_equal>::resize() {

  if (prime == num_primes - 1) {
    cerr << "maximal table size reached, aborting ... " << endl;
    exit(2);
  }

  int mm = prime_list[prime];
  prime++;
  int m = prime_list[prime];
  vector<Entry> *ptr = new vector<Entry>(m);

  for (int i = 0; i < mm; ++i) {

    if ((*ht)[i].used == 1) {
      key_type kk = (*ht)[i].key;

      int p = hf(kk) % m;

      while (p < m && (*ptr)[p].used == 1) {
        p++;
      }
      if (p == m) {
        p = 0;
      }
      while ((*ptr)[p].used == 1) {
        p++;
      }

      (*ptr)[p].key = kk;
      (*ptr)[p].used = 1;
    }
  }

  delete ht;
  ht = ptr;
  return m;
}
