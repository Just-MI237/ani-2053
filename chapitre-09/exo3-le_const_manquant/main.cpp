#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;
struct M { string name; bool is_const; bool mod; vector<string> calls; bool pourrait = false; };
int main() {
    int N; cin >> N; vector<M> ms(N); map<string, int> idx;
    for (int i = 0; i < N; ++i) {
        cin >> ms[i].name; string c; cin >> c; ms[i].is_const = (c == "const");
        cin >> ms[i].mod; int k; cin >> k;
        for (int j = 0; j < k; ++j) { string call; cin >> call; ms[i].calls.push_back(call); }
        idx[ms[i].name] = i;
    }
    bool changed = true;
    while(changed) {
        changed = false;
        for (int i = 0; i < N; ++i) {
            if (ms[i].is_const || ms[i].mod || ms[i].pourrait) continue;
            bool ok = true;
            for (string c : ms[i].calls) if (!ms[idx[c]].is_const && !ms[idx[c]].pourrait) { ok = false; break; }
            if (ok) { ms[i].pourrait = true; changed = true; }
        }
    }
    vector<string> pour, rac;
    for (int i = 0; i < N; ++i) if (ms[i].pourrait) {
        pour.push_back(ms[i].name); bool root = true;
        for (string c : ms[i].calls) if (!ms[idx[c]].is_const) { root = false; break; }
        if (root) rac.push_back(ms[i].name);
    }
    int passes = 0; vector<bool> cur_const(N);
    for (int i = 0; i < N; ++i) cur_const[i] = ms[i].is_const;
    int rem = pour.size();
    while (rem > 0) {
        vector<int> to_make;
        for (int i = 0; i < N; ++i) if (ms[i].pourrait && !cur_const[i]) {
            bool ok = true; for (string c : ms[i].calls) if (!cur_const[idx[c]]) { ok = false; break; }
            if (ok) to_make.push_back(i);
        }
        if (to_make.empty()) break;
        for (int i : to_make) { cur_const[i] = true; rem--; } passes++;
    }
    if (pour.empty()) cout << "POURRAIT AUCUN\n"; else { cout << "POURRAIT"; for (string s : pour) cout << " " << s; cout << "\n"; }
    if (rac.empty()) cout << "RACINES AUCUN\n"; else { cout << "RACINES"; for (string s : rac) cout << " " << s; cout << "\n"; }
    cout << "PASSES " << passes << "\n";
}
