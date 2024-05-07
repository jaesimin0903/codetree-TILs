#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;
unordered_map<string,string> um_s;
unordered_map<string,string>um_i;
int main() {
    // 여기에 코드를 작성해주세요.
    int N, M;
    cin >> N >> M;
    
    for(int i =1;i<=N;i++){
        string str;
        
        cin >> str;
        um_s[str] = to_string(i);
        um_i[to_string(i)] = str;
    }

    while(M--){
        string s;
        cin >> s;
        if(um_s.find(s) == um_s.end()){
            cout << um_i[s] << "\n";
        }else{
            cout << um_s[s] << "\n";
        }
    }
    return 0;
}