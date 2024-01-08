#include <iostream>
#include <string>
#include <vector>

using namespace std;


bool check(vector<char>& seq) {
    int n = seq.size();
    bool found = false;
    int i = 0, j = 0;
    while (i < n) {
        j = i + 1;
        while (j < n && seq[j] == seq[i]) j++;
        if (j - i >= 3) {
            seq.erase(seq.begin() + i, seq.begin() + j);
            found = true;
            n = seq.size();
        } else {
            i = j;
        }
    }
    return found;
}

int main() {
    string input;
    cin >> input;
    vector<char> seq(input.begin(), input.end());
    int n;
    cin >> n;
    vector<int> idx(n);
    vector<char> color(n);
    for (int i = 0; i < n; i++) {
        cin >> idx[i] >> color[i];
    }
    for (int i = 0; i < n; i++) {
        seq.insert(seq.begin() + idx[i], color[i]);
        while (check(seq)) {}
    }
    for (char c : seq) {
        cout << c;
    }
    cout << endl;
    return 0;
}