#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <cstring>

using namespace std;

int K, M;

int board[5][5];
deque<int> add;

void printBoard() {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            cout << board[i][j] << " ";
        }
        cout << "\n";
    }
}

void makeNoneEmpty() {
    for (int j = 0; j < 5; j++) {
        for (int i = 4; i >= 0; i--) {
            if (board[i][j] == 0) {
                board[i][j] = add.front();
                int tmp = add.front();
                add.pop_front();
                add.push_back(tmp);
            }
        }
    }
}

bool oob(int y, int x) {
    return y < 0 || x < 0 || y >= 5 || x >= 5;
}

int bfs(bool isChain) {
    int answer = 0;
    int chainAnswer = 0;
    bool v[5][5];
    memset(v, false, sizeof(v));

    while (isChain) {
        //printBoard();
        bool isPassing = true;
        memset(v, false, sizeof(v));

        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (v[i][j]) continue;
                int cur = board[i][j];
                int curCnt = 1;
                queue<pair<int, int>>q;
                q.push({ i,j });
                v[i][j] = true;
                vector<pair<int, int>> eraseList;
                eraseList.push_back({ i,j });
                int dy[4] = { -1,1,0,0 };
                int dx[4] = { 0,0,-1,1 };

                while (!q.empty()) {
                    int y = q.front().first;
                    int x = q.front().second;
                    q.pop();

                    for (int d = 0; d < 4; d++) {
                        int ny = y + dy[d];
                        int nx = x + dx[d];
                        if (oob(ny, nx))continue;
                        if (board[ny][nx] != cur)continue;
                        if (v[ny][nx]) continue;
                        curCnt++;
                        v[ny][nx] = true;
                        eraseList.push_back({ ny,nx });
                        q.push({ ny,nx });
                    }
                }

                if (curCnt >= 3) {
                    for (auto v : eraseList) {
                        board[v.first][v.second] = 0;
                    }
                    chainAnswer += curCnt;
                    isPassing = false;
                }
            }
        }
        makeNoneEmpty();
        if (isPassing) {
            //cout << "chaining ans " << chainAnswer<< "\n";
            return chainAnswer;
        }
    }
    if (!isChain) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (v[i][j]) continue;
                int cur = board[i][j];
                int curCnt = 1;
                queue<pair<int, int>>q;
                q.push({ i,j });
                v[i][j] = true;
               

                int dy[4] = { -1,1,0,0 };
                int dx[4] = { 0,0,-1,1 };

                while (!q.empty()) {
                    int y = q.front().first;
                    int x = q.front().second;
                    q.pop();

                    for (int d = 0; d < 4; d++) {
                        int ny = y + dy[d];
                        int nx = x + dx[d];
                        if (oob(ny, nx))continue;
                        if (board[ny][nx] != cur)continue;
                        if (v[ny][nx]) continue;
                        curCnt++;
                        v[ny][nx] = true;
                        q.push({ ny,nx });
                    }
                }

                if (curCnt >= 3) {

                    answer += curCnt;
                }
            }
        }
        //cout << "none chaining ans " << answer << "\n";
        return answer;
    }
    
}

void rotate(pair<int, int> p, int deg) {
    int ty = p.first;
    int tx = p.second;

    
    int rotations = deg;  
    for(int i =0;i<deg+1;i++){
        // 90도 회전하는 부분
        // 3x3 구역 추출 후 회전
        int temp[3][3];

        // 3x3 구역 복사 및 회전 (ty, tx를 중심으로)
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                // 시계 방향 90도 회전
                temp[j][2 - i] = board[ty - 1 + i][tx - 1 + j];
            }
        }

        // 회전된 3x3 구역을 다시 원래 배열에 복사
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                board[ty - 1 + i][tx - 1 + j] = temp[i][j];
            }
        }
    }
}

pair<pair<int, int>, int> findRotateCell() {
    pair<pair<int, int>, int> result = { {-1,-1}, -1 };
    int maxPart = 0;
    for (int j = 3; j >= 1;j--) {
        for (int i = 3; i >= 1;i--) {
            for (int r = 0; r < 3; r++) {
                rotate(make_pair(i, j), r);
                int part = bfs(false);
                if (maxPart < part) {
                    maxPart = part;
                    result.first = { i,j };
                    result.second = r;
                }
                else if (maxPart == part) {
                    if (result.second > r) {
                        result.first = { i,j };
                        result.second = r;
                    }
                    else if (result.second == r) {
                        /*if (result.first.first > i) {
                            result.first = { i,j };
                            result.second = r;
                        }
                        else if (result.first.first == i) {
                            if (result.first.second > j) {
                                result.first = { i,j };
                                result.second = r;
                            }
                        }*/
                        if (result.first.second > j) {
                            result.first = { i,j };
                            result.second = r;
                        }
                        else if (result.first.second == j) {
                            if (result.first.first > i) {
                                result.first = { i,j };
                                result.second = r;
                            }
                        }
                    }
                }
                rotate(make_pair(i, j), 2 - r);
            }
        }
    }
    //cout << result.first.first << " " << result.first.second << " " << result.second << "\n";
    //printBoard();
    //cout << "\n";
    return result;
}

int main() {
    cin >> K >> M;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            cin >> board[i][j];
        }
    }

    for (int i = 0; i < M; i++) {
        int n1;
        cin >> n1;
        add.push_back(n1);
    }

    while (K--) {
        //돌려서 최대 중앙셀 찾기
        pair<pair<int,int>,int> pointWithDegree = findRotateCell();

        //돌리기
        if (pointWithDegree.first.first == -1) {//더이상 찾을 수 없다면
            return 0;
        }
        else rotate(pointWithDegree.first, pointWithDegree.second);

        //연쇄 반응
        int part = bfs(true);

        cout << part << " ";
    }

    return 0;
}