/**
 * 定义二叉搜索树类，封装查找、插入、删除操作（包括合并删除和复制删除）
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>
using namespace std;

template <typename T> class TreeNode {
public:
  T val;
  TreeNode *left;
  TreeNode *right;
  TreeNode(T x) : val(x), left(nullptr), right(nullptr) {}
};

template <typename T> class BinarySearchTree {
  TreeNode<T> *root;

  TreeNode<T> *mergeRemove(TreeNode<T> *node, T val) {
    if (!node)
      return nullptr;
    if (val < node->val) {
      node->left = mergeRemove(node->left, val);
    } else if (val > node->val) {
      node->right = mergeRemove(node->right, val);
    } else {
      if (!node->left) {
        TreeNode<T> *rightNode = node->right;
        delete node;
        return rightNode;
      } else if (!node->right) {
        TreeNode<T> *leftNode = node->left;
        delete node;
        return leftNode;
      }
      TreeNode<T> *maxNode = getMax(node->left);
      node->val = maxNode->val;
      node->left = mergeRemove(node->left, maxNode->val);
    }
    return node;
  }

  TreeNode<T> *copyRemove(TreeNode<T> *node, T val) {
    if (!node)
      return nullptr;
    if (val < node->val) {
      node->left = copyRemove(node->left, val);
    } else if (val > node->val) {
      node->right = copyRemove(node->right, val);
    } else {
      if (!node->left) {
        TreeNode<T> *rightNode = node->right;
        delete node;
        return rightNode;
      } else if (!node->right) {
        TreeNode<T> *leftNode = node->left;
        delete node;
        return leftNode;
      }
      TreeNode<T> *minNode = getMin(node->right);
      node->val = minNode->val;
      node->right = copyRemove(node->right, minNode->val);
    }
    return node;
  }

  TreeNode<T> *getMin(TreeNode<T> *node) {
    while (node->left)
      node = node->left;
    return node;
  }

  TreeNode<T> *getMax(TreeNode<T> *node) {
    while (node->right)
      node = node->right;
    return node;
  }

  void insert(TreeNode<T> *node, T val) {
    if (val < node->val) {
      if (node->left) {
        insert(node->left, val);
      } else {
        node->left = new TreeNode<T>(val);
      }
    } else {
      if (node->right) {
        insert(node->right, val);
      } else {
        node->right = new TreeNode<T>(val);
      }
    }
  }

public:
  BinarySearchTree() : root(nullptr) {}
  ~BinarySearchTree() { clear(); }

  void insert(T val) {
    if (root == nullptr) {
      root = new TreeNode<T>(val);
      return;
    }
    insert(root, val);
  }

  TreeNode<T> *find(T val) {
    TreeNode<T> *cur = root;
    while (cur != nullptr) {
      if (cur->val == val) {
        return cur;
      } else if (val < cur->val) {
        cur = cur->left;
      } else {
        cur = cur->right;
      }
    }
    return nullptr;
  }

  void mergeRemove(T val) { root = mergeRemove(root, val); }

  void copyRemove(T val) { root = copyRemove(root, val); }

  void clear() {
    queue<TreeNode<T> *> q;
    q.push(root);
    while (!q.empty()) {
      TreeNode<T> *cur = q.front();
      q.pop();
      if (cur == nullptr) {
        continue;
      }
      q.push(cur->left);
      q.push(cur->right);
      delete cur;
    }
    root = nullptr;
  }

  void print() {
    if (root == nullptr) {
      return;
    }
    queue<TreeNode<T> *> q;
    q.push(root);
    while (!q.empty()) {
      TreeNode<T> *cur = q.front();
      q.pop();
      if (cur == nullptr) {
        continue;
      }
      cout << cur->val << " ";
      q.push(cur->left);
      q.push(cur->right);
    }
    cout << endl;
  }
};

int main() {
  BinarySearchTree<int> bst;
  bst.insert(5);
  bst.insert(3);
  bst.insert(7);
  bst.insert(2);
  bst.insert(4);
  bst.insert(6);
  bst.insert(8);
  bst.print();
  cout << "find 3 at " << bst.find(3) << endl;
  cout << "find 9 at " << bst.find(9) << endl;
  bst.mergeRemove(7);
  cout << "merge remove 7" << endl;
  bst.print();
  bst.copyRemove(6);
  cout << "copy remove 6" << endl;
  bst.print();
  return 0;
}
