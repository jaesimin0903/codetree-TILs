#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;
unordered_map<int,int> um;
int main() {
    // 여기에 코드를 작성해주세요.
    int N;
    cin >> N;
    while(N--){
        string op;
        int n1, n2;
        cin >> op ;

        if(op == "add"){
            cin >> n1 >> n2;
            if(um.find(n1) != um.end()){
                um[n1] = n2;
            }
            else um.insert({n1,n2});
        }else if(op== "find"){
            cin >> n1;
            if(um.find(n1) != um.end()){
                cout << (um.find(n1))->second<<"\n";
            }else{
                cout << "None\n";
            }
        }else if(op == "remove"){
            cin >> n1;
            um.erase(n1);
        }
    }
    return 0;
}