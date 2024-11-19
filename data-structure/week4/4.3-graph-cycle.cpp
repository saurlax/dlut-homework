/**
 * 假设以邻接矩阵作为图的存储结构，编写算法判别在给定的有向图中是否存在一个简单有向回路，
 * 若存在，则以顶点序列的方式输出该回路（找到一条即可）。（注：图中不存在顶点到自己的弧）
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

class Graph {
  int V;
  vector<vector<int>> adjMatrix;

  bool dfsUtil(int v, vector<bool> &visited, vector<bool> &recStack,
               vector<int> &path) {
    visited[v] = true;
    recStack[v] = true;
    path.push_back(v);

    for (int i = 0; i < V; ++i) {
      if (adjMatrix[v][i] != 0) {
        if (!visited[i] && dfsUtil(i, visited, recStack, path)) {
          return true;
        } else if (recStack[i]) {
          path.push_back(i);
          return true;
        }
      }
    }

    recStack[v] = false;
    path.pop_back();
    return false;
  }

public:
  Graph(int V) {
    this->V = V;
    adjMatrix.resize(V, vector<int>(V, 0));
  }

  void addEdge(int u, int v) { adjMatrix[u][v] = 1; }

  bool findCycle() {
    vector<bool> visited(V, false);
    vector<bool> recStack(V, false);
    vector<int> path;

    for (int i = 0; i < V; ++i) {
      if (!visited[i] && dfsUtil(i, visited, recStack, path)) {
        auto it = find(path.begin(), path.end(), path.back());
        cout << "Cycle found: ";
        for (; it != path.end(); ++it) {
          cout << *it << " ";
        }
        cout << endl;
        return true;
      }
    }

    cout << "No cycle found" << endl;
    return false;
  }
};

int main() {
  Graph g(4);
  g.addEdge(0, 1);
  g.addEdge(1, 2);
  g.addEdge(2, 0);
  g.addEdge(2, 3);

  g.findCycle();

  return 0;
}