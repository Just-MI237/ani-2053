#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;
int main() {
    vector<string> lines; string l;
    while(getline(cin, l)) if(!l.empty()) lines.push_back(l);
    vector<vector<string>> blocks; blocks.push_back({});
    map<string, vector<string>> deps; map<string, bool> freed;
    for (string cmd : lines) {
        if (cmd == "{") blocks.push_back({});
        else if (cmd == "}") {
            for (auto it = blocks.back().rbegin(); it != blocks.back().rend(); ++it) {
                string obj = *it; if (freed[obj]) continue;
                for (string dep : deps[obj]) if (freed[dep]) { cout << "PLANTAGE " << obj << " " << dep << "\n"; return 0; }
                cout << "DETRUIT " << obj << "\n"; freed[obj] = true;
            }
            blocks.pop_back();
        } else if (cmd.substr(0, 5) == "DECL ") blocks.back().push_back(cmd.substr(5));
        else if (cmd.substr(0, 7) == "DEPEND ") { int p = cmd.find(' ', 7); deps[cmd.substr(7, p-7)].push_back(cmd.substr(p+1)); }
        else if (cmd.substr(0, 6) == "FERME ") {
            string obj = cmd.substr(6); if (freed[obj]) continue;
            for (string dep : deps[obj]) if (freed[dep]) { cout << "PLANTAGE " << obj << " " << dep << "\n"; return 0; }
            cout << "FERME " << obj << "\n"; freed[obj] = true;
        }
    }
    while (!blocks.empty()) {
        for (auto it = blocks.back().rbegin(); it != blocks.back().rend(); ++it) {
            string obj = *it; if (freed[obj]) continue;
            for (string dep : deps[obj]) if (freed[dep]) { cout << "PLANTAGE " << obj << " " << dep << "\n"; return 0; }
            cout << "DETRUIT " << obj << "\n"; freed[obj] = true;
        }
        blocks.pop_back();
    }
    cout << "PROPRE\n";
}
