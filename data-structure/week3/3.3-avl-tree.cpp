#include <algorithm>
#include <iostream>
using namespace std;

template <typename T> class AVLTreeNode {
public:
  T val;
  AVLTreeNode *left;
  AVLTreeNode *right;
  int height;

  AVLTreeNode(T x) : val(x), left(nullptr), right(nullptr), height(1) {}
};

template <typename T> class AVLTree {
  AVLTreeNode<T> *root;

  int height(AVLTreeNode<T> *node) { return node ? node->height : 0; }

  int getBalance(AVLTreeNode<T> *node) {
    return node ? height(node->left) - height(node->right) : 0;
  }

  AVLTreeNode<T> *rightRotate(AVLTreeNode<T> *y) {
    AVLTreeNode<T> *x = y->left;
    AVLTreeNode<T> *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
  }

  AVLTreeNode<T> *leftRotate(AVLTreeNode<T> *x) {
    AVLTreeNode<T> *y = x->right;
    AVLTreeNode<T> *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
  }

  AVLTreeNode<T> *insert(AVLTreeNode<T> *node, T val) {
    if (!node)
      return new AVLTreeNode<T>(val);

    if (val < node->val)
      node->left = insert(node->left, val);
    else if (val > node->val)
      node->right = insert(node->right, val);
    else
      return node;

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && val < node->left->val)
      return rightRotate(node);

    if (balance < -1 && val > node->right->val)
      return leftRotate(node);

    if (balance > 1 && val > node->left->val) {
      node->left = leftRotate(node->left);
      return rightRotate(node);
    }

    if (balance < -1 && val < node->right->val) {
      node->right = rightRotate(node->right);
      return leftRotate(node);
    }

    return node;
  }

  AVLTreeNode<T> *minValueNode(AVLTreeNode<T> *node) {
    AVLTreeNode<T> *current = node;
    while (current->left != nullptr)
      current = current->left;
    return current;
  }

  AVLTreeNode<T> *deleteNode(AVLTreeNode<T> *root, T val) {
    if (!root)
      return root;

    if (val < root->val)
      root->left = deleteNode(root->left, val);
    else if (val > root->val)
      root->right = deleteNode(root->right, val);
    else {
      if (!root->left || !root->right) {
        AVLTreeNode<T> *temp = root->left ? root->left : root->right;

        if (!temp) {
          temp = root;
          root = nullptr;
        } else
          *root = *temp;

        delete temp;
      } else {
        AVLTreeNode<T> *temp = minValueNode(root->right);
        root->val = temp->val;
        root->right = deleteNode(root->right, temp->val);
      }
    }

    if (!root)
      return root;

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
      return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
      root->left = leftRotate(root->left);
      return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
      return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
      root->right = rightRotate(root->right);
      return leftRotate(root);
    }

    return root;
  }

  void preOrder(AVLTreeNode<T> *root) {
    if (root != nullptr) {
      cout << root->val << " ";
      preOrder(root->left);
      preOrder(root->right);
    }
  }

public:
  AVLTree() : root(nullptr) {}

  void insert(T val) { root = insert(root, val); }

  void deleteNode(T val) { root = deleteNode(root, val); }

  void print() {
    preOrder(root);
    cout << endl;
  }

  AVLTreeNode<T> *find(T val) {
    AVLTreeNode<T> *current = root;
    while (current != nullptr) {
      if (current->val == val)
        return current;
      else if (val < current->val)
        current = current->left;
      else
        current = current->right;
    }
    return nullptr;
  }
};

int main() {
  AVLTree<int> tree;
  tree.insert(10);
  tree.insert(20);
  tree.insert(30);
  tree.insert(40);
  tree.insert(50);
  tree.insert(25);
  tree.print();
  tree.deleteNode(40);
  cout << "Deleted 40" << endl;
  tree.print();
  return 0;
}