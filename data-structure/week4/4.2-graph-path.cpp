/**
 * 定义采用邻接矩阵存储的图结构，Dijkstra算法、Floyd算法
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>
using namespace std;

class Graph {
  int V;
  vector<vector<int>> adjMatrix;

public:
  Graph(int V) {
    this->V = V;
    adjMatrix.resize(V, vector<int>(V, 0));
  }

  void addEdge(int u, int v, int w) {
    adjMatrix[u][v] = w;
    adjMatrix[v][u] = w;
  }

  int find(vector<int> &parent, int i) {
    if (parent[i] == -1)
      return i;
    return find(parent, parent[i]);
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

  vector<int> dijkstra(int src) {
    vector<int> dist(V, INT_MAX);
    vector<bool> sptSet(V, false);

    dist[src] = 0;

    for (int count = 0; count < V - 1; ++count) {
      int u = -1;

      for (int i = 0; i < V; ++i) {
        if (!sptSet[i] && (u == -1 || dist[i] < dist[u])) {
          u = i;
        }
      }

      sptSet[u] = true;

      for (int v = 0; v < V; ++v) {
        if (!sptSet[v] && adjMatrix[u][v] && dist[u] != INT_MAX &&
            dist[u] + adjMatrix[u][v] < dist[v]) {
          dist[v] = dist[u] + adjMatrix[u][v];
        }
      }
    }

    return dist;
  }

  vector<vector<int>> floydWarshall() {
    vector<vector<int>> dist = adjMatrix;

    for (int i = 0; i < V; ++i) {
      for (int j = 0; j < V; ++j) {
        if (i != j && dist[i][j] == 0) {
          dist[i][j] = INT_MAX;
        }
      }
    }

    for (int k = 0; k < V; ++k) {
      for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
          if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX &&
              dist[i][k] + dist[k][j] < dist[i][j]) {
            dist[i][j] = dist[i][k] + dist[k][j];
          }
        }
      }
    }

    return dist;
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

  cout << "Dijkstra's shortest path from vertex 0:\n";
  vector<int> dist = g.dijkstra(0);
  for (int i = 0; i < dist.size(); ++i) {
    cout << "0 to " << i << " distance: " << dist[i] << endl;
  }

  cout << "Floyd-Warshall all-pairs shortest path:\n";
  vector<vector<int>> fwDist = g.floydWarshall();
  for (int i = 0; i < fwDist.size(); ++i) {
    for (int j = 0; j < fwDist[i].size(); ++j) {
      if (fwDist[i][j] == INT_MAX) {
        cout << "INF ";
      } else {
        cout << fwDist[i][j] << " ";
      }
    }
    cout << endl;
  }

  return 0;
}