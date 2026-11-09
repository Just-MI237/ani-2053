#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    string C; if (!getline(cin, C)) return 0;
    string l; string D = "ABSENT", CP = "ABSENT", CA = "ABSENT", M = "ABSENT", MA = "ABSENT";
    string p_cp = C + "(const" + C + "&", p_ca = C + "&operator=(const" + C + "&", p_m = C + "(" + C + "&&", p_ma = C + "&operator=(" + C + "&&", p_d = "~" + C + "(";
    while (getline(cin, l)) {
        size_t p = l.find("//"); if (p != string::npos) l = l.substr(0, p);
        l.erase(remove(l.begin(), l.end(), ' '), l.end()); if (l.empty()) continue;
        string st = "ECRIT";
        if (l.size() >= 7 && l.substr(l.size()-7) == "=delete;") st = "SUPPRIME";
        else if (l.size() >= 9 && l.substr(l.size()-9) == "=default;") st = "DEFAUT";
        if (l.find(p_d) != string::npos) D = st;
        else if (l.find(p_cp) == 0 && (l.size() == p_cp.size() || l[p_cp.size()] != '&')) CP = st;
        else if (l.find(p_ca) == 0 && (l.size() == p_ca.size() || l[p_ca.size()] != '&')) CA = st;
        else if (l.find(p_m) == 0) M = st;
        else if (l.find(p_ma) == 0) MA = st;
    }
    cout << "DESTRUCTEUR " << D << "\nCOPIE " << CP << "\nAFFECTATION_COPIE " << CA << "\nDEPLACEMENT " << M << "\nAFFECTATION_DEPLACEMENT " << MA << "\n";
    int dec = (D!="ABSENT") + (CP!="ABSENT") + (CA!="ABSENT") + (M!="ABSENT") + (MA!="ABSENT");
    if (dec == 0) cout << "REGLE ZERO\n"; else if (dec == 5) cout << "REGLE CINQ\n";
    else if (D!="ABSENT" && CP!="ABSENT" && CA!="ABSENT" && M=="ABSENT" && MA=="ABSENT") cout << "REGLE TROIS\n";
    else {
        cout << "REGLE INCOMPLETE\n";
        vector<pair<string,string>> all = {{"DESTRUCTEUR",D},{"COPIE",CP},{"AFFECTATION_COPIE",CA},{"DEPLACEMENT",M},{"AFFECTATION_DEPLACEMENT",MA}};
        bool h_m = (M!="ABSENT" || MA!="ABSENT");
        for (int i = 0; i < 5; ++i) if (all[i].second == "ABSENT" && (h_m || i < 3)) cout << "MANQUE " << all[i].first << "\n";
    }
    cout << "POSSEDE " << (D == "ECRIT" ? "OUI" : "NON") << "\nUNIQUE " << ((CP == "SUPPRIME" && (M == "ECRIT" || M == "DEFAUT")) ? "OUI" : "NON") << "\n";
}
