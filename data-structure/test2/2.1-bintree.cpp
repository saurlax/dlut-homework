#include <iostream>
#include <queue>
#include <stack>
#include <unordered_map>
#include <vector>

using namespace std;

template <typename T> struct TreeNode {
  T value;
  TreeNode *left;
  TreeNode *right;
  TreeNode(T x) : value(x), left(NULL), right(NULL) {}
};

template <typename T> class BinaryTree {
public:
  TreeNode<T> *root;

  BinaryTree() : root(NULL) {}

  void insert(T value) {
    if (!root) {
      root = new TreeNode<T>(value);
    } else {
      insert(root, value);
    }
  }

  // 广度优先遍历
  void breadthFirstTraversal() {
    if (!root)
      return;
    queue<TreeNode<T> *> q;
    q.push(root);
    while (!q.empty()) {
      TreeNode<T> *node = q.front();
      q.pop();
      cout << node->value << " ";
      if (node->left)
        q.push(node->left);
      if (node->right)
        q.push(node->right);
    }
    cout << endl;
  }

  // 前序递归遍历
  void preorderTraversalRecursive(TreeNode<T> *node) {
    if (!node)
      return;
    cout << node->value << " ";
    preorderTraversalRecursive(node->left);
    preorderTraversalRecursive(node->right);
  }

  // 中序递归遍历
  void inorderTraversalRecursive(TreeNode<T> *node) {
    if (!node)
      return;
    inorderTraversalRecursive(node->left);
    cout << node->value << " ";
    inorderTraversalRecursive(node->right);
  }

  // 后序递归遍历
  void postorderTraversalRecursive(TreeNode<T> *node) {
    if (!node)
      return;
    postorderTraversalRecursive(node->left);
    postorderTraversalRecursive(node->right);
    cout << node->value << " ";
  }

  // 前序非递归遍历
  void preorderTraversalIterative() {
    if (!root)
      return;
    stack<TreeNode<T> *> s;
    s.push(root);
    while (!s.empty()) {
      TreeNode<T> *node = s.top();
      s.pop();
      cout << node->value << " ";
      if (node->right)
        s.push(node->right);
      if (node->left)
        s.push(node->left);
    }
    cout << endl;
  }

  // 中序非递归遍历
  void inorderTraversalIterative() {
    stack<TreeNode<T> *> s;
    TreeNode<T> *node = root;
    while (node || !s.empty()) {
      while (node) {
        s.push(node);
        node = node->left;
      }
      node = s.top();
      s.pop();
      cout << node->value << " ";
      node = node->right;
    }
    cout << endl;
  }

  // 后序非递归遍历
  void postorderTraversalIterative() {
    if (!root)
      return;
    stack<TreeNode<T> *> s1, s2;
    s1.push(root);
    while (!s1.empty()) {
      TreeNode<T> *node = s1.top();
      s1.pop();
      s2.push(node);
      if (node->left)
        s1.push(node->left);
      if (node->right)
        s1.push(node->right);
    }
    while (!s2.empty()) {
      TreeNode<T> *node = s2.top();
      s2.pop();
      cout << node->value << " ";
    }
    cout << endl;
  }

  // 由先序和中序构造二叉树
  TreeNode<T> *buildTreePreIn(vector<T> &preorder, vector<T> &inorder) {
    unordered_map<T, int> inMap;
    for (int i = 0; i < inorder.size(); ++i) {
      inMap[inorder[i]] = i;
    }
    return buildTreePreIn(preorder, 0, preorder.size() - 1, inorder, 0,
                          inorder.size() - 1, inMap);
  }

  // 由后序和中序构造二叉树
  TreeNode<T> *buildTreePostIn(vector<T> &postorder, vector<T> &inorder) {
    unordered_map<T, int> inMap;
    for (int i = 0; i < inorder.size(); ++i) {
      inMap[inorder[i]] = i;
    }
    return buildTreePostIn(postorder, 0, postorder.size() - 1, inorder, 0,
                           inorder.size() - 1, inMap);
  }

private:
  void insert(TreeNode<T> *node, T value) {
    if (value < node->value) {
      if (node->left) {
        insert(node->left, value);
      } else {
        node->left = new TreeNode<T>(value);
      }
    } else {
      if (node->right) {
        insert(node->right, value);
      } else {
        node->right = new TreeNode<T>(value);
      }
    }
  }

  TreeNode<T> *buildTreePreIn(vector<T> &preorder, int preStart, int preEnd,
                              vector<T> &inorder, int inStart, int inEnd,
                              unordered_map<T, int> &inMap) {
    if (preStart > preEnd || inStart > inEnd)
      return NULL;
    TreeNode<T> *root = new TreeNode<T>(preorder[preStart]);
    int inRoot = inMap[root->value];
    int numsLeft = inRoot - inStart;
    root->left = buildTreePreIn(preorder, preStart + 1, preStart + numsLeft,
                                inorder, inStart, inRoot - 1, inMap);
    root->right = buildTreePreIn(preorder, preStart + numsLeft + 1, preEnd,
                                 inorder, inRoot + 1, inEnd, inMap);
    return root;
  }

  TreeNode<T> *buildTreePostIn(vector<T> &postorder, int postStart, int postEnd,
                               vector<T> &inorder, int inStart, int inEnd,
                               unordered_map<T, int> &inMap) {
    if (postStart > postEnd || inStart > inEnd)
      return NULL;
    TreeNode<T> *root = new TreeNode<T>(postorder[postEnd]);
    int inRoot = inMap[root->value];
    int numsLeft = inRoot - inStart;
    root->left = buildTreePostIn(postorder, postStart, postStart + numsLeft - 1,
                                 inorder, inStart, inRoot - 1, inMap);
    root->right = buildTreePostIn(postorder, postStart + numsLeft, postEnd - 1,
                                  inorder, inRoot + 1, inEnd, inMap);
    return root;
  }
};

int main() {
  BinaryTree<int> bt;
  bt.insert(3);
  bt.insert(1);
  bt.insert(4);
  bt.insert(2);

  cout << "广度优先遍历: ";
  bt.breadthFirstTraversal();

  cout << "前序遍历（递归）: ";
  bt.preorderTraversalRecursive(bt.root);
  cout << endl;

  cout << "中序遍历（递归）: ";
  bt.inorderTraversalRecursive(bt.root);
  cout << endl;

  cout << "后序遍历（递归）: ";
  bt.postorderTraversalRecursive(bt.root);
  cout << endl;

  cout << "前序遍历（非递归）: ";
  bt.preorderTraversalIterative();

  cout << "中序遍历（非递归）: ";
  bt.inorderTraversalIterative();

  cout << "后序遍历（非递归）: ";
  bt.postorderTraversalIterative();

  vector<int> preorder = {3, 1, 2, 4};
  vector<int> inorder = {1, 2, 3, 4};
  bt.root = bt.buildTreePreIn(preorder, inorder);
  cout << "由先序和中序构造的树的中序遍历: ";
  bt.inorderTraversalRecursive(bt.root);
  cout << endl;

  vector<int> postorder = {2, 1, 4, 3};
  inorder = {1, 2, 3, 4};
  bt.root = bt.buildTreePostIn(postorder, inorder);
  cout << "由后序和中序构造的树的中序遍历: ";
  bt.inorderTraversalRecursive(bt.root);
  cout << endl;

  return 0;
}