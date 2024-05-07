#include <iostream>
#include <unordered_map>

using namespace std;
typedef long long ll;
unordered_map<ll,ll> um;
ll n,  m;
int main() {
    // 여기에 코드를 작성해주세요.
    cin >> n >> m;

    while(n--){
        int n1;
        cin >> n1;
        um[n1]++;
    }

    while(m--){
        int n1;
        cin >> n1;
        cout << um[n1] << " ";
    }
    return 0;
}