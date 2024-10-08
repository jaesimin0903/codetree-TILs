#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <cstring>
#include <tuple>
#include <iomanip>

using namespace std;

//미로는 NxN, 각위치는 r,c, 좌상단은 1,1

//미로규칙
//1. 빈칸 : 참가자가 이동가능한 칸
//2. 벽 : 참가자 이동 불가, 1~9 내구도, 회전할 때 내구도 1씩 감소, 0될 때 빈칸
//3. 출구 : 해당칸에 참가자가 도착하면 즉시탈출

//움직이는 조건 : 매초 한칸씩 움ㅁ직인다.
//두위치의 최단거리 |x1-x2| + |y1-y2|
//모든 참가자는 동시에 움직인다. (배열 복사해서 움직여야할듯)
//상하좌우 움직일 수 있다., 벽이 없는 곳만 움직일 수 있다.
//움직인 칸은 현재 칸보다 출구까지의 최단거리가 가까워야한다. (현재칸 - 출구) > (움직이려는칸 - 출구)
//움직일 수 있는칸이 여러개, 상하로 움직이는 것을 우선
//움직일 수 없다면 움직이지 않는다.
//한칸에 두명 이상 참가자 있을 수 있다.

//모든 참가자가 이동을 끝내면 미로가 회전
//회전 조건
// 한명이상의 참가자와 출구를 포함한 가장 작은 정사각형을 잡는다.
//가장 작은 정사각형이 여러개라면 r 작은것, c 작은 것 우선
//선택된 정사각형은 시계방향으로 90도 회전, 내구도 1씩 깎인다.

//게임이 끝났을 때 모든 참가자들의 이동 거리 합, 출구좌표를 출력

struct Player {
    int y;
    int x;
    int howmany;
};

int N, M, K;
int exitY, exitX;

int maze[11][11];
int player[11][11];

int dy[4] = { -1,1,0,0 };
int dx[4] = { 0,0,-1,1 };

int totalDis = 0;

void printMaze() {
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            cout << setw(3) << maze[i][j];
        }
        cout << "\n";
    }
}

bool oob(int y, int x) {
    return y < 1 || x < 1 || y > N || x > N;
}

void rotate(tuple<int, int, int> pointWithSize) {
    //cout << "rotate\n";
    int size = get<2>(pointWithSize);
    vector<vector<int>> tmp = vector<vector<int>>(size + 1, vector<int>(size + 1, 0));

    int row = get<0>(pointWithSize);
    int col = get<1>(pointWithSize);

    for (int i = 1; i <= size; i++) {
        for (int j = 1; j <= size; j++) {
            tmp[j][size - i+1] = maze[row + i - 1][col + j - 1];
            if (tmp[j][size-i+1] > 0) tmp[j][size-i+1]--;
        }
    }

    for (int i = 1; i <= size; i++) {
        for (int j = 1; j <= size; j++) {
            maze[row + i - 1][col + j - 1] = tmp[i][j];
        }
    }
}

tuple<int, int, int> findSmallSqure() {
    tuple<int, int, int> pointWithSize;
    int eY, eX;
    
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (maze[i][j] == -11) {
                eY = i; eX = j;
            }
        }
    }

    int pointY = 0;
    int pointX = 0;

    for (int size = 2; size <= N; size++) {
        for (int i = 1; i <= N - size +1; i++) {
            for (int j = 1; j <= N - size+1; j++) {
                int peopleCount = 0;
                bool hasExit = false;
                for (int row = i; row < i + size; row++) {
                    for (int col = j; col < j + size; col++) {
                        if (maze[row][col] == -11) hasExit = true;
                        else if (-10 <= maze[row][col] && maze[row][col] <= -1) peopleCount += -1 * maze[row][col];
                    }
                }
                //cout << i << " " << j << " " << size << "---loop\n";
                if (hasExit && peopleCount >=1) {
                    pointWithSize = { i,j,size };
                    //cout << i << " " << j << " " << size << "\n";
                    return pointWithSize;
                }
            }
        }
    }
    
    return pointWithSize;
}

void move() {
    int eY, eX;
    queue<pair<int, int>> q;
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (maze[i][j] == -11) {
                eY = i; eX = j;
            }
            else if (maze[i][j] <= -1 && -10 <= maze[i][j]) {
                q.push({ i,j });
            }
        }
    }

    while (!q.empty()) {
        int curY, curX;
        curY = q.front().first; curX = q.front().second;
        q.pop();
        
        int moveY = 0, moveX = 0;
        int exitDis = abs(eY - curY) + abs(eX - curX);

        for (int d = 0; d < 4; d++) {
            int ny = curY + dy[d];
            int nx = curX + dx[d];
            if (oob(ny, nx)) continue;
            if (maze[ny][nx] > 0) continue;
            int dis = abs(ny - eY) + abs(nx - eX) + 1;
            if (exitDis < dis) continue;
            else if (exitDis == dis) {
                if (eY - curY > 0 && ny - curY > 0) {
                    moveY = ny;
                    moveX = nx;
                }
                else if (eY - curY < 0 && ny - curY < 0) {
                    moveY = ny;
                    moveX = nx;
                }
                else if (maze[ny][nx] == -11) {
                    moveY = ny;
                    moveX = nx;
                }
                else if(moveY == 0) {
                    
                    moveY = ny;
                    moveX = nx;
                }
            }
        }

        //움직일 곳을 찾았다면
        if (moveY != 0) {
            totalDis += maze[curY][curX] * -1;
            if (maze[moveY][moveX] != -11) {
                maze[moveY][moveX] += maze[curY][curX];
            }
            maze[curY][curX] =0;
        }
    }
}

bool isAllEmpty() {
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (-10 <= maze[i][j] && maze[i][j] <= -1) return false;
        }
    }

    return true;
}

int main() {
    cin >> N >> M >> K;

    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            cin >> maze[i][j];
        }
    }

    for (int i = 0; i < M; i++) {
        int y, x;
        cin >> y >> x;
        maze[y][x] -= 1;
    }

    cin >> exitY >> exitX;
    maze[exitY][exitX] = -11;

    while (K--) {
        if (isAllEmpty()) {
            break;
        }

        move();
    /*    cout << "----\n";
        printMaze();
        cout << "\n";*/
        tuple<int, int, int> pointWithSize = findSmallSqure();

        rotate(pointWithSize);
        //printMaze();
        //cout << "----\n";
    }

    cout << totalDis << "\n";
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (maze[i][j] == -11) cout << i << " " << j;
        }
    }

    return 0;
}