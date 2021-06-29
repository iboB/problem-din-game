// Solution: to MNK Challenge Din
// Idea: BFS + (optimized) memo

#include <bits/stdc++.h>

using namespace std;

int main() {
    vector<int> cur(3), nxt(3);
    for (int& a : cur) cin>>a;
    // (min << 32) + max
    unordered_set<long long> memo;
    // a,b,c,from,to,prev
    vector<tuple<int, int, int, char, char, int>> que;
    que.emplace_back(cur[0], cur[1], cur[2], ' ', ' ', -1),
    memo.insert(((long long)*min_element(cur.begin(), cur.end())<<32) + *max_element(cur.begin(), cur.end()));
    int head = 0;

    while(head < que.size()) {
        cur[0] = nxt[0] = get<0>(que[head]);
        cur[1] = nxt[1] = get<1>(que[head]);
        cur[2] = nxt[2] = get<2>(que[head]);

        for (int from = 0; from < 3; from++) {
            for (int to = 0; to < 3; to++) {
                if (from == to) continue;
                if (cur[from] >= cur[to]) {
                    nxt[from] -= cur[to];
                    nxt[to] += cur[to];

                    int mn = *min_element(nxt.begin(), nxt.end());
                    int mx = *max_element(nxt.begin(), nxt.end());

                    long long key = ((long long)mn<<32) + mx;
                    if (memo.find(key) == memo.end()) {
                        memo.insert(key);
                        que.emplace_back(nxt[0], nxt[1], nxt[2], 'A' + from, 'A' + to, head);
                    }

                    if (mn == 0) {
                        stack<int> stk;
                        int at = (int)que.size() - 1;
                        while(at > 0) {
                            stk.push(at);
                            at = get<5>(que[at]);
                        }
                        while(!stk.empty()) {
                            int i = stk.top();
                            cout << get<3>(que[i]) << "->" << get<4>(que[i]) << "\n";
                            //uncomment to see intermediate result
                            //cout << get<0>(que[i]) << " " << get<1>(que[i]) << " " << get<2>(que[i]) << "\n";
                            stk.pop();
                        }
                        return 0;
                    }

                    nxt[from] += cur[to];
                    nxt[to] -= cur[to];
                }
            }
        }
        head++;
    }

    cout << "NOT POSSIBLE"<<endl;

    return 0;
}
