#include <iostream>
#include <set>
#include <map>
#include <array>
#include <algorithm>
using namespace std;

typedef array<int, 3> Stones;
typedef map<Stones, uint64_t> Nodes;

void show_solution(const Stones &init, uint64_t track)
{
    typedef pair<int, char> ValBox;
    array<ValBox, 3> boxes { ValBox(init[0], 'A'), ValBox(init[1], 'B'),
                             ValBox(init[2], 'C') };
    while (track)
    {
        int code = track & 3,
            i_src = code == 1 ? 1 : 2,
            i_dest = code == 3 ? 1 : 0;
        track >>= 2;
        cout << boxes[i_src].second << "->" << boxes[i_dest].second << endl;
        boxes[i_src].first -= boxes[i_dest].first;
        boxes[i_dest].first *= 2;
        sort(boxes.begin(), boxes.end());
    }
}

bool make_new_node(const Nodes::value_type &node, int from, int to,
                   int offset, Nodes &last_nodes, const set<Stones> &all_nodes)
{
    Stones new_node(node.first);
    new_node[from] -= new_node[to];
    new_node[to] *= 2;
    sort(new_node.begin(), new_node.end());
    return all_nodes.find(new_node) == all_nodes.end() &&
        last_nodes.insert(Nodes::value_type(new_node,
        node.second | (uint64_t(from + to) << offset))).second;
}

bool check_for_2_step_soln(const Stones &st)
{
    return st[1] == 2 * st[0] ||
           st[1] == 3 * st[0] ||
           st[2] == 3 * st[1] ||
           st[2] == 3 * st[0] ||
           st[2] == 2 * st[0] ||
           st[2] == st[0] + st[1];
}

bool check_for_3_step_soln(const Stones &st)
{
    return st[1] == 7 * st[0] ||
           st[2] == 7 * st[0] ||
           st[2] == 7 * st[1] ||
           st[1] == 6 * st[0] ||
           st[2] == 6 * st[0] ||
           st[1] == 5 * st[0] ||
           st[1] == 4 * st[0] ||
           3 * st[1] == 5 * st[0] ||
           3 * st[2] == 5 * st[0] ||
           3 * st[2] == 5 * st[1] ||
           2 * st[1] == 3 * st[0] ||
           3 * st[0] + st[1] == st[2] ||
           3 * st[1] + st[0] == st[2] ||
           2 * st[0] + st[1] == st[2] ||
           3 * st[1] == st[0] + st[2] ||
           3 * st[0] == st[1] + st[2] ||
           2 * (st[2] - st[1]) == st[0] ||
           2 * (st[2] - st[0]) == st[1] ||
           3 * (st[1] - st[0]) == st[2] ||
           3 * (st[2] - st[0]) == st[1] ||
           3 * (st[2] - st[1]) == st[0] ||
           (st[2] == 5 * st[0] && st[1] > 2 * st[0]) ||
           (st[2] == 4 * st[0] && st[1] > 3 * st[0]) ||
           (2 * st[1] + st[0] == st[2] && 2 * st[0] > st[1]) ||
           (2 * (st[1] - st[0]) == st[2] && 3 * st[0] > st[1]);
}

bool solve(const Stones &init)
{
    Nodes last_nodes, prev_nodes;
    set<Stones> all_nodes;
    last_nodes.insert(Nodes::value_type(init, 0));
    all_nodes.insert(init);
    bool must_go_on, soln_in_3 = false, forwrd_chk = !check_for_2_step_soln(init);
    int offset = 0;
    do
    {
        must_go_on = false;
        prev_nodes = last_nodes;
        last_nodes.clear();
        for (Nodes::value_type prev : prev_nodes)
        {
            if (prev.first[0] == 0)
            {
                show_solution(init, prev.second);
                return true;
            }
            if (forwrd_chk && check_for_3_step_soln(prev.first))
            {
                soln_in_3 = true;
                last_nodes.clear();
            }
            must_go_on = make_new_node(prev, 1, 0, offset, last_nodes, all_nodes)
                         || must_go_on;
            must_go_on = make_new_node(prev, 2, 0, offset, last_nodes, all_nodes)
                         || must_go_on;
            must_go_on = make_new_node(prev, 2, 1, offset, last_nodes, all_nodes)
                         || must_go_on;
            if (soln_in_3)
            {
                soln_in_3 = forwrd_chk = false;
                break;
            }
        }
        offset += 2;
        for (Nodes::value_type new_node : last_nodes)
            all_nodes.insert(new_node.first);
    }
    while (must_go_on);
    return false;
}

int main()
{
    Stones s;
    cin >> s[0] >> s[1] >> s[2];
    if (!(0 < s[0] && s[0] < s[1] && s[1] < s[2] && s[2] < 100000))
        cout << "Illegal input!\n";
    else if (!solve(s))
        cout << "NOT POSSIBLE\n";
    return 0;
}

