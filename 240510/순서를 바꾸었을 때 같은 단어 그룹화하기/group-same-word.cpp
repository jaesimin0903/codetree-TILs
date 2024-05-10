#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_map>

using namespace std;

unordered_map<string, int> um;
int main() {
    // 여기에 코드를 작성해주세요.

    int n;
    int answer= 0;
    cin >> n;
    while(n--){
        string str;
        cin >> str;

        sort(str.begin(), str.end());

        um[str]++;
        answer = max(answer, um[str]);
    }

    cout << answer;
    return 0;
}