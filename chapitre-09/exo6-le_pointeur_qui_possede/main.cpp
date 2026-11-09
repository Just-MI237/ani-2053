#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;
struct Op { string type, a, b; };
int main() {
    int N; cin >> N; vector<Op> ops(N); vector<int> copy_lines;
    for (int i = 0; i < N; ++i) {
        cin >> ops[i].type;
        if (ops[i].type == "COPY") { cin >> ops[i].a >> ops[i].b; copy_lines.push_back(i+1); }
        else if (ops[i].type == "MOVE") cin >> ops[i].a >> ops[i].b;
        else cin >> ops[i].a;
    }
    if (!copy_lines.empty()) { for (int l : copy_lines) cout << "REFUSE " << l << "\n"; return 0; }
    map<string, int> var_obj; vector<string> order; map<string, bool> seen;
    int next_obj = 1, crees = 0, detruits = 0, nuls = 0; vector<bool> alive(1, false);
    auto kill = [&](int obj) { if (obj > 0 && alive[obj]) { cout << "DETRUIT " << obj << "\n"; alive[obj] = false; detruits++; } };
    for (int i = 0; i < N; ++i) {
        string type = ops[i].type, a = ops[i].a, b = ops[i].b;
        if (type == "NEW") {
            int obj = ++crees; alive.push_back(true);
            if (var_obj.count(a) && var_obj[a] > 0) kill(var_obj[a]);
            var_obj[a] = obj; if (!seen[a]) { seen[a] = true; order.push_back(a); }
        } else if (type == "MOVE") {
            int obj_a = var_obj.count(a) ? var_obj[a] : 0; var_obj[a] = 0;
            if (var_obj.count(b) && var_obj[b] > 0) kill(var_obj[b]);
            var_obj[b] = obj_a;
            if (!seen[a]) { seen[a] = true; order.push_back(a); }
            if (!seen[b]) { seen[b] = true; order.push_back(b); }
        } else if (type == "RESET") {
            if (var_obj.count(a) && var_obj[a] > 0) kill(var_obj[a]);
            var_obj[a] = 0; if (!seen[a]) { seen[a] = true; order.push_back(a); }
        } else if (type == "USE") {
            if (!var_obj.count(a) || var_obj[a] == 0) { cout << "NUL " << i+1 << " " << a << "\n"; nuls++; }
            if (!seen[a]) { seen[a] = true; order.push_back(a); }
        }
    }
    for (auto it = order.rbegin(); it != order.rend(); ++it) if (var_obj.count(*it) && var_obj[*it] > 0) kill(var_obj[*it]);
    cout << "CREES " << crees << "\nDETRUITS " << detruits << "\nNULS " << nuls << "\n";
}
