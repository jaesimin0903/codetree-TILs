#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
int n, k;
typedef long long ll;
using namespace std;
int arr[100010];
unordered_map<ll,ll> um;
vector<pair<ll,ll>> v;
int main() {
    // 여기에 코드를 작성해주세요.
    cin >> n >> k;

    while(n--){
        ll n1;
        cin >> n1;
        um[n1]++;
    }

    for(auto &val : um){
        v.push_back(val);
    }

    sort(v.begin(), v.end(), [](const auto&a, const auto&b){
        if(a.second!=b.second) return a.second > b.second;
        return a.first > b.first;
    });

    for(int i =0;i<k;i++){
        if(i >= v.size()) break;
        cout << v[i].first << " ";
    }
    
    return 0;
}