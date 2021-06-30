// To compile this use `g++ -O3 -std=c++20 din.cpp -o din` command line.

#include <bits/stdc++.h>

using namespace std;

struct Triple {
  union {
    struct {
      int32_t a, b, c;
    };
    int32_t values[3];
  };
};

struct Move {
  int8_t from, to;
};

Triple readInput() {
  Triple result;
  cin >> result.a >> result.b >> result.c;
  return result;
}

bool constructMove(vector<Triple>& triples, vector<Move>& moves,
                   int i, int8_t a, int8_t b) {
  Triple triple = triples[i];
  if (triple.values[a] > triple.values[b])
    swap(a, b);
  triple.values[b] -= triple.values[a];
  triple.values[a] <<= 1;
  triples.push_back(triple);
  moves.push_back({b, a});
  return triple.a == 0 || triple.b == 0 || triple.c == 0;
}

vector<Move> moveSequence(const vector<Move>& moves) {
  vector<Move> result;
  result.reserve(17);
  int i = moves.size() - 1;
  while (i > 0) {
    result.push_back(moves[i]);
    i = (i - 1) / 3;
  }
  ranges::reverse(result);
  return result;
}

vector<Move> solve(const Triple& input)
{
  constexpr int capacity = 130'000'000;
  vector<Triple> triples;
  vector<Move> moves;
  triples.reserve(capacity);
  moves.reserve(capacity);
  triples.push_back(input);
  moves.push_back({-1, -1});
  int i = 0;
  while (true) {
    if (constructMove(triples, moves, i, 1, 2))
      return moveSequence(moves);
    if (constructMove(triples, moves, i, 0, 2))
      return moveSequence(moves);
    if (constructMove(triples, moves, i, 0, 1))
      return moveSequence(moves);
    ++i;
  }
}

void printOutput(const vector<Move>& moves) {
  for (const auto& move : moves) {
    cout << char('A' + move.from) << "->" << char('A' + move.to) << "\n";
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  printOutput(solve(readInput()));
  return 0;
}
