/**
 * 定义采用邻接矩阵存储的图结构，封装DFS、BFS算法、Prim算法、Kruskal算法
 */

#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>
using namespace std;

class Graph {
  int V;
  vector<vector<int>> adjMatrix;

  void dfsUtil(int v, vector<bool> &visited) {
    visited[v] = true;
    cout << v << " ";

    for (int i = 0; i < V; ++i) {
      if (adjMatrix[v][i] != 0 && !visited[i]) {
        dfsUtil(i, visited);
      }
    }
  }

public:
  Graph(int V) {
    this->V = V;
    adjMatrix.resize(V, vector<int>(V, 0));
  }

  void addEdge(int u, int v, int w) {
    adjMatrix[u][v] = w;
    adjMatrix[v][u] = w;
  }

  void dfs(int v) {
    vector<bool> visited(V, false);
    dfsUtil(v, visited);
    cout << endl;
  }

  void bfs(int s) {
    vector<bool> visited(V, false);
    queue<int> q;
    visited[s] = true;
    q.push(s);

    while (!q.empty()) {
      s = q.front();
      cout << s << " ";
      q.pop();

      for (int i = 0; i < V; ++i) {
        if (adjMatrix[s][i] != 0 && !visited[i]) {
          visited[i] = true;
          q.push(i);
        }
      }
    }
    cout << endl;
  }

  Graph primMST() {
    vector<int> key(V, INT_MAX);
    vector<int> parent(V, -1);
    vector<bool> inMST(V, false);

    key[0] = 0;

    for (int count = 0; count < V - 1; ++count) {
      int u = -1;

      for (int i = 0; i < V; ++i) {
        if (!inMST[i] && (u == -1 || key[i] < key[u])) {
          u = i;
        }
      }

      inMST[u] = true;

      for (int v = 0; v < V; ++v) {
        if (adjMatrix[u][v] && !inMST[v] && adjMatrix[u][v] < key[v]) {
          key[v] = adjMatrix[u][v];
          parent[v] = u;
        }
      }
    }

    Graph mst(V);
    for (int i = 1; i < V; ++i) {
      mst.addEdge(parent[i], i, adjMatrix[i][parent[i]]);
    }
    return mst;
  }

  int find(vector<int> &parent, int i) {
    if (parent[i] == -1)
      return i;
    return find(parent, parent[i]);
  }

  void unionSets(vector<int> &parent, int x, int y) {
    int xset = find(parent, x);
    int yset = find(parent, y);
    if (xset != yset) {
      parent[xset] = yset;
    }
  }

  Graph kruskalMST() {
    vector<pair<int, pair<int, int>>> edges;

    for (int i = 0; i < V; ++i) {
      for (int j = i + 1; j < V; ++j) {
        if (adjMatrix[i][j] != 0) {
          edges.push_back({adjMatrix[i][j], {i, j}});
        }
      }
    }

    sort(edges.begin(), edges.end());

    vector<int> parent(V, -1);
    Graph mst(V);

    for (auto edge : edges) {
      int u = edge.second.first;
      int v = edge.second.second;
      int set_u = find(parent, u);
      int set_v = find(parent, v);

      if (set_u != set_v) {
        mst.addEdge(u, v, edge.first);
        unionSets(parent, set_u, set_v);
      }
    }

    return mst;
  }

  void print() {
    cout << "Edge\tWeight\n";
    for (int i = 0; i < V; ++i) {
      for (int j = i + 1; j < V; ++j) {
        if (adjMatrix[i][j] != 0) {
          cout << i << "->" << j << "\t" << adjMatrix[i][j] << "\n";
        }
      }
    }
  }
};

int main() {
  Graph g(6);
  g.addEdge(0, 1, 4);
  g.addEdge(0, 2, 4);
  g.addEdge(1, 2, 2);
  g.addEdge(1, 3, 5);
  g.addEdge(2, 3, 5);
  g.addEdge(2, 4, 9);
  g.addEdge(3, 4, 4);
  g.addEdge(3, 5, 2);
  g.addEdge(4, 5, 6);

  cout << "DFS starting from vertex 0:\n";
  g.dfs(0);

  cout << "BFS starting from vertex 0:\n";
  g.bfs(0);

  cout << "Prim's MST:\n";
  g.primMST().print();

  cout << "Kruskal's MST:\n";
  g.kruskalMST().print();

  return 0;
}