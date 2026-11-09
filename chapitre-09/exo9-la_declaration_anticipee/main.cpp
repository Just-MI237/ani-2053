#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
using namespace std;
int main() {
    int U; cin >> U; map<string, bool> needs; vector<string> types;
    for(int i=0; i<U; ++i) {
        string t, u; cin >> t >> u;
        if (needs.find(t) == needs.end()) types.push_back(t);
        if (u == "VALEUR" || u == "BASE" || u == "CORPS" || u == "TAILLE") needs[t] = true;
    }
    int A; cin >> A; set<string> inc; for(int i=0; i<A; ++i) { string t; cin >> t; inc.insert(t); }
    int c_i = 0, c_a = 0, c_r = 0;
    for(string t : types) {
        if (needs[t]) { cout << t << " INCLURE\n"; c_i++; }
        else { cout << t << " ANTICIPER\n"; c_a++; if (inc.count(t)) c_r++; }
    }
    cout << "INCLURE " << c_i << "\nANTICIPER " << c_a << "\nINCLUSIONS_RETIREES " << c_r << "\n";
}
