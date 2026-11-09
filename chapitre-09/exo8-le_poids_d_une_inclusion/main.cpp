#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
using namespace std;
int main() {
    int N; cin >> N; map<string, vector<string>> adj; vector<string> files;
    for(int i=0; i<N; ++i) { string f; int k; cin >> f >> k; files.push_back(f); for(int j=0; j<k; ++j) { string inc; cin >> inc; adj[f].push_back(inc); } }
    int Q; cin >> Q; vector<string> queries(Q); for(int i=0; i<Q; ++i) cin >> queries[i];
    auto is_cpp = [](string s) { return s.size() >= 4 && s.substr(s.size()-4) == ".cpp"; };
    auto is_h = [](string s) { return s.size() >= 2 && s.substr(s.size()-2) == ".h"; };
    auto count_cpp = [&](string q) {
        set<string> rec; vector<string> st = {q};
        while(!st.empty()) { string c = st.back(); st.pop_back(); if (is_cpp(c)) rec.insert(c);
            for(auto& p : adj) if (find(p.second.begin(), p.second.end(), c) != p.second.end()) st.push_back(p.first); }
        return (int)rec.size();
    };
    for(string q : queries) cout << q << " " << count_cpp(q) << "\n";
    string best = ""; int max_c = -1;
    for(string f : files) if (is_h(f)) { int c = count_cpp(f); if (c > max_c) { max_c = c; best = f; } }
    if (best == "") cout << "PLUS_LOURD AUCUN\n"; else cout << "PLUS_LOURD " << best << " " << max_c << "\n";
}
