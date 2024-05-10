#include <iostream>
#include <unordered_map>
#include <vector>

typedef long long ll;

using namespace std;

unordered_map<ll, ll> um1;
unordered_map<ll,ll>um2;


int n;
int main() {
    // 여기에 코드를 작성해주세요.
    cin >> n;
    vector<ll>v;
    vector<ll>v2;
    ll answer =0;
    for(int i =0;i<n;i++){
        int n1;
        cin >> n1;
        v.push_back(n1);
    }

    for(int i =0;i<n;i++){
        int n1;
        cin >> n1;
        v2.push_back(n1);
    }

    for(int i =0;i<v.size();i++){
        for(int j = 0;j<v.size();j++){
            um1[v[i] + v2[j]]++;
        }
    }

    v.clear();
    v2.clear();

    for(int i =0;i<n;i++){
        int n1;
        cin >> n1;
        v.push_back(n1);
    }

    for(int i =0;i<n;i++){
        int n1;
        cin >> n1;
        v2.push_back(n1);
    }

    for(int i =0;i<v.size();i++){
        for(int j = 0;j<v.size();j++){
            um2[v[i] + v2[j]]++;
        }
    }

    for(auto &val : um1){
        ll target = -val.first;
        //cout << target <<"\n";
        if(um2.find(target) != um2.end()){
            answer+=um2[target];
            val.second--;
        }
    }

    cout << answer;
    
    return 0;
}