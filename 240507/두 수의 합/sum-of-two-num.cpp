#include <iostream>
#include <unordered_map>
#include <vector>
#include <utility>
#include <algorithm>
typedef long long ll;
using namespace std;

unordered_map<ll,ll> um;
vector<ll> v;
ll n,k;
int main() {
    // 여기에 코드를 작성해주세요.
    cin >> n >> k;
    ll answer= 0 ;
    for(int i =0;i<n;i++){
        ll n1;
        cin >> n1;
        um[n1]++;
        v.push_back(n1);
    }

    v.erase(unique(v.begin(),v.end()),v.end());

    for(int i =0;i<v.size();i++){
        ll target = k - v[i];
        if(um.find(target) != um.end()){
            answer += um[target];
            um[target] = 0;
            um[v[i]] =0;
        }
        
    }

    cout << answer;
    return 0;
}