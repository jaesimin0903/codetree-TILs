#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;

unordered_map<char, pair<int,int>> um;//character, index, count
int main() {
    // 여기에 코드를 작성해주세요.
    string str;
    cin >> str;
    for(int i =0;i<str.size();i++){
        if(um.find(str[i])!=um.end()){
            um[str[i]].second++;
        }else{
            um[str[i]] = {i, 1};
        }
    }

    vector<tuple<char,int,int>> v;
    for(int i =0;i<str.size();i++){
        v.push_back({str[i], um[str[i]].first, um[str[i]].second});
    }

    sort(v.begin(), v.end(), [](const auto&a, const auto &b){
        if(get<2>(a) != get<2>(b)) return get<1>(a) < get<1>(b);
        if(get<2>(a)!= get<2>(b)) return get<2>(a) < get<2>(b);
    });

    if(get<2>(v[0]) > 1){
        cout << "None";
        return 0;
    }
    cout << get<0>(v[0]);

    return 0;
}