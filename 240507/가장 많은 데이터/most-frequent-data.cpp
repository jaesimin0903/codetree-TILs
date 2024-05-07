#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;

unordered_map<string,int> um;
int main() {
    // 여기에 코드를 작성해주세요.
    int N;
    cin >> N;
    string ans;
    int mmx = 0;
    while(N--){
        string str;
        cin >> str;
        um[str]++;
        mmx = max(mmx, um[str]);
    }

    cout << mmx;
    return 0;
}