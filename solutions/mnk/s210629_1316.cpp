#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

struct State {
  int a, b;
};

struct Move {
  char a, b;
};

long long id(int a, int b, int c) {
  return ((long long)min({a, b, c}) << 20) | max({a, b, c}); 
}

void printPath(int a, int b, int c, unordered_map<long long, int> &depth) {
  vector<Move> path;
  int d = depth[id(a, b, c)];
  int na = a, nb = b, nc = c;
  
  while (d > 1) {
    a = na, b = nb, c = nc, d--;

    if (a % 2 == 0) {
      na = a / 2, nb = b + na, nc = c;
      if (depth[id(na, nb, nc)] == d) { path.push_back({'B', 'A'}); continue; }
      na = a / 2, nb = b, nc = c + na;
      if (depth[id(na, nb, nc)] == d) { path.push_back({'C', 'A'}); continue; }
    }
    if (b % 2 == 0) {
      nb = b / 2, na = a + nb, nc = c;
      if (depth[id(na, nb, nc)] == d) { path.push_back({'A', 'B'}); continue; }
      nb = b / 2, na = a, nc = c + nb;
      if (depth[id(na, nb, nc)] == d) { path.push_back({'C', 'B'}); continue; }
    }
    if (c % 2 == 0) {
      nc = c / 2, na = a + nc, nb = b;
      if (depth[id(na, nb, nc)] == d) { path.push_back({'A', 'C'}); continue; }
      nc = c / 2, na = a, nb = b + nc;
      if (depth[id(na, nb, nc)] == d) { path.push_back({'B', 'C'}); continue; }
    }
  }

  char la = 'A', lb = 'B', lc = 'C';
  if (na > nc) swap(na, nc), swap(la, lc);
  if (nb > nc) swap(nb, nc), swap(lb, lc);
  if (na > nb) swap(na, nb), swap(la, lb);

  reverse(path.begin(), path.end());

  for (Move m : path) {
    if (m.a == la) cout << 'A';
    if (m.a == lb) cout << 'B';
    if (m.a == lc) cout << 'C';
    cout << "->";
    if (m.b == la) cout << 'A';
    if (m.b == lb) cout << 'B';
    if (m.b == lc) cout << 'C';
    cout << "\n";
  }
}

bool bfs(int a, int b, int c) {
  unordered_map<long long, int> depth;
  depth.reserve(10000000);

  int z = a + b + c;
  int d = 1;
  
  vector<State> layer;
  vector<State> nextLayer;
  layer.push_back({a, b});
  while (not layer.empty()) {
    nextLayer.clear();
    nextLayer.reserve(layer.size() * 3);
    
    for (State s : layer) {
      int a = s.a;
      int b = s.b;
      int c = z - a - b;
      
      int &t = depth[id(a, b, c)];
      if (t > 0) continue;
      t = d;
      if (a == 0 or b == 0 or c == 0) {
        printPath(a, b, c, depth);
        return true;
      }

      if (a < b) nextLayer.push_back({a*2, b-a});
      else nextLayer.push_back({a-b, b*2});
      if (a < c) nextLayer.push_back({a*2, b});
      else nextLayer.push_back({a-c, b});
      if (b < c) nextLayer.push_back({a, b*2});
      else nextLayer.push_back({a, b-c});
    }

    d++;
    layer.swap(nextLayer);
  }
  return false;
}

int main() {
  int a, b, c;
  cin >> a >> b >> c;
  if (not bfs(a, b, c)) { // never happens
    cout << "NOT POSSIBLE\n";
  }
  return 0;
}