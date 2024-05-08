#include <iostream>
#include <unordered_map>
#include <vector>
#include <utility>
#include <algorithm>
typedef long long ll;

using namespace std;

unordered_map<ll,ll> um;

//두개의 수를 더해서 나오는 경우의 수 배열을 만들고
//

ll n,k;
ll answer = 0;
vector<ll> v;
int main() {
    // 여기에 코드를 작성해주세요.
    cin >> n >> k;
    ll N = n;
    while(n--){
        int n1;
        cin >> n1;
        um[n1]++;
        v.push_back(n1);
    }

    vector<ll> per;

    for(int i = 0;i<v.size();i++){
        per.push_back(1);
    }

    per[0] = 0;
    per[1] = 0;
    per[2] = 0;

    do{
        ll ans  =0;
        for(int i =0;i<per.size();i++){
            if(per[i] == 0) ans += v[i];
        }
        if(ans == k) answer++;
    }while(next_permutation(per.begin(), per.end()));
    cout << answer;
    return 0;
}