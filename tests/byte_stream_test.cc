#include <iostream>
#include <string>
#include <deque>

using namespace std;

int main() {

    deque<char> d;
    // d.push_back('c');
    // d.push_back('a');
    // d.push_back('t');
    string temp;

    string cat = "cat";
    for (int i = 0; i < 3; i++) {
        d.push_back(cat[i]);
    }

    for (size_t i = 0; i < 3; i++) {
        temp += d[i];
    }

    if (temp == cat) {
        cout << "Equal!\n";
    } else if (temp != cat) {
        cout << "Not equal\n";
    }
    return 0;    
    
}