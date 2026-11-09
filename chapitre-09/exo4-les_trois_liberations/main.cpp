#include <iostream>
#include <vector>
using namespace std;
struct T { int id; int freed; };
int main() {
    int S, V, F; cin >> S >> V >> F; int N; cin >> N;
    vector<T> all; vector<int> tab; int creees = 0, next_id = 1;
    for (int op = 1; op <= N; ++op) {
        string cmd; cin >> cmd;
        if (cmd == "AJOUTE") { all.push_back({next_id++, 0}); tab.push_back(all.size()-1); creees++; }
        else if (cmd == "SUPPRIME") {
            int i; cin >> i; int ti = tab[i];
            for (int f = 0; f < S; ++f) { if (all[ti].freed >= 1) { cout << "PLANTAGE " << op << " TEXTURE " << all[ti].id << "\n"; return 0; } all[ti].freed++; }
            tab.erase(tab.begin() + i);
        } else if (cmd == "VIDE") {
            for (int ti : tab) for (int f = 0; f < V; ++f) { if (all[ti].freed >= 1) { cout << "PLANTAGE " << op << " TEXTURE " << all[ti].id << "\n"; return 0; } all[ti].freed++; }
            tab.clear();
        }
    }
    for (int ti : tab) for (int f = 0; f < F; ++f) { if (all[ti].freed >= 1) { cout << "PLANTAGE FIN TEXTURE " << all[ti].id << "\n"; return 0; } all[ti].freed++; }
    int fuites = 0; vector<int> perdues;
    for (auto& t : all) if (t.freed == 0) { fuites++; perdues.push_back(t.id); }
    cout << "CREEES " << creees << "\nFUITES " << fuites << "\nPERDUES";
    if (perdues.empty()) cout << " AUCUN\n"; else { for (int id : perdues) cout << " " << id; cout << "\n"; }
}
