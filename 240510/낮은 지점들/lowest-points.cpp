#include <iostream>
#include <unordered_map>
#include <vector>

typedef long long ll;

using namespace std;

ll n;
unordered_map<ll,ll> um;

int main() {
    // 여기에 코드를 작성해주세요.

     cin >> n;
     while(n--){
        ll x, y;
        cin >> x >> y;

        if(um.find(x) != um.end()){
            um[x] = min(um[x] , y);
        }else{
            um[x] = y;
        }
     }
    ll answer = 0;
     for(auto & val : um){
        answer += val.second;
     }

     cout << answer;
    return 0;
}