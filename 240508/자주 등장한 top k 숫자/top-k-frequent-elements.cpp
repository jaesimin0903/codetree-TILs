#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
int n, k;

using namespace std;
int arr[100010];
unordered_map<int,int> um;
vector<pair<int,int>> v;
int main() {
    // 여기에 코드를 작성해주세요.
    cin >> n >> k;

    while(n--){
        int n1;
        cin >> n1;
        arr[n1]++;
    }

    for(int i =0;i<100010;i++){
        if(arr[i] != 0)v.push_back({i,arr[i]});
    }

    sort(v.begin(), v.end(), [](const auto&a, const auto&b){
        if(a.second!=b.second) return a.second > b.second;
        return a.first > b.first;
    });

    for(int i =0;i<k;i++){
        cout << v[i].first << " ";
    }
    
    return 0;
}