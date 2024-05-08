#include <iostream>
#include <vector>

using namespace std;

int main() {
    int n, k;
    cin >> n >> k;
    vector<int> nums(n);
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }

    int count = 0;

    // 세 개의 서로 다른 위치를 모두 조합하는 반복문
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            for (int m = j + 1; m < n; ++m) {
                if (nums[i] + nums[j] + nums[m] == k) {
                    count++;
                }
            }
        }
    }

    cout << count << endl;
    return 0;
}