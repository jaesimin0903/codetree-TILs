#include <iostream>
#include <cmath>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Santa {
    int stunTime;
    int point;
    bool isOut;
};

int map[51][51];
Santa santa[31];
int N, M, P, C, D;

void printMap() {
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            cout << setw(3) << map[i][j];
        }
        cout << "\n";
    }
    cout << "\n";
}

bool oob(int y, int x) {
    return y < 1 || x < 1 || y > N || x > N;
}

void santaSantaChain(int y, int x, int dir) {
    int dy[8] = { -1,-1,0,1,1,1,0,-1 };
    int dx[8] = { 0,1,1,1,0,-1,-1,-1 };

    if (map[y][x] == 0) return;
    int curSantaNum = map[y][x];
    int ny = y + dy[dir];
    int nx = x + dx[dir];

    if (oob(ny, nx)) {
        santa[curSantaNum].isOut = true;
    } else if (map[ny][nx] >= 0) {
        santaSantaChain(ny, nx, dir);
        map[ny][nx] = curSantaNum;
    }
}

void santaChain(int y, int x, int dir, int turn) {
    int dy[8] = { -1,-1,0,1,1,1,0,-1 };
    int dx[8] = { 0,1,1,1,0,-1,-1,-1 };

    int curSantaNum = map[y][x];
    santa[curSantaNum].stunTime = turn;
    santa[curSantaNum].point += C;

    map[y][x] = 0;
    if (oob(y + dy[dir] * C, x + dx[dir] * C)) {
        santa[curSantaNum].isOut = true;
    } else if (map[y + dy[dir] * C][x + dx[dir] * C] >= 1) {
        santaSantaChain(y + dy[dir] * C, x + dx[dir] * C, dir);
        map[y + dy[dir] * C][x + dx[dir] * C] = curSantaNum;
    } else {
        map[y + dy[dir] * C][x + dx[dir] * C] = curSantaNum;
    }
}

void sMove(int turn) {
    pair<int, int> s[31];
    int dy[4] = { -1,0,1,0 };
    int dx[4] = { 0,1,0,-1 };

    int ry, rx;
    // 루돌프 위치 찾기
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (map[i][j] == -1) {
                ry = i; rx = j;
            }
        }
    }

    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (map[i][j] >= 1) {
                s[map[i][j]].first = i;
                s[map[i][j]].second = j;
            }
        }
    }

    // 산타 1번부터 이동
    for (int i = 1; i <= P; i++) {
        // 기절한 상태면 패스
        if (santa[i].stunTime + 1 >= turn) continue;
        
        int curY = s[i].first;
        int curX = s[i].second;
        int gap = pow(curY - ry, 2) + pow(curX - rx, 2);

        int moveY = 0;
        int moveX = 0;
        int dir = -1;

        for (int d = 0; d < 4; d++) {
            int ny = curY + dy[d];
            int nx = curX + dx[d];
            if (oob(ny, nx)) continue;
            if (map[ny][nx] >= 1) continue;

            int nGap = pow(ry - ny, 2) + pow(rx - nx, 2);
            
            // 거리가 더 가까워지면 업데이트
            if (gap > nGap) {
                moveY = ny;
                moveX = nx;
                dir = d;
                gap = nGap;
            }
            // 상우하좌 순서에 따른 우선순위 적용
            else if (gap == nGap) {
                if (moveY == 0 || (d < dir)) {
                    moveY = ny;
                    moveX = nx;
                    dir = d;
                }
            }
        }

        // 산타 이동
        if (moveY != 0) {
            map[curY][curX] = 0;
            // 루돌프와 충돌 시
            if (map[moveY][moveX] == -1) {
                santa[i].point += D;
                santa[i].stunTime = turn;
                dir = (dir + 2) % 4;

                if (oob(moveY + dy[dir] * D, moveX + dx[dir] * D)) {
                    santa[i].isOut = true;
                } else {
                    int chainDir = dir * 2;
                    santaSantaChain(moveY + dy[dir] * D, moveX + dx[dir] * D, chainDir);
                    map[moveY + dy[dir] * D][moveX + dx[dir] * D] = i;
                }
            } else {
                map[moveY][moveX] = i;
            }
        }
    }

    // 살아있는 산타들 점수 1점 추가
    for (int i = 1; i <= P; i++) {
        if (!santa[i].isOut) santa[i].point++;
    }
}

void rMove(int turn) {
    int dy[8] = { -1,-1,0,1,1,1,0,-1 };
    int dx[8] = { 0,1,1,1,0,-1,-1,-1 };
    int ry, rx;

    // 루돌프 위치 찾기
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (map[i][j] == -1) {
                ry = i;
                rx = j;
            }
        }
    }

    int selectSantaNum = -1;
    int selectSantaDis = 1e9;
    int selectSantaY = -1;
    int selectSantaX = -1;
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (map[i][j] >= 1) {
                int dis = pow((ry - i), 2) + pow((rx - j), 2);
                // 선택된 산타 거리보다 작다면 선택
                if (selectSantaDis > dis) {
                    selectSantaDis = dis;
                    selectSantaNum = map[i][j];
                    selectSantaY = i;
                    selectSantaX = j;
                } else if (selectSantaDis == dis) {
                    if (selectSantaY < i || (selectSantaY == i && selectSantaX < j)) {
                        selectSantaDis = dis;
                        selectSantaNum = map[i][j];
                        selectSantaY = i;
                        selectSantaX = j;
                    }
                }
            }
        }
    }

    int dir = -1;
    int dirDis = 1e9;
    // 루돌프가 선택된 산타까지 최단 거리로 움직일 수 있는 위치 찾기
    for (int d = 0; d < 8; d++) {
        int ny = ry + dy[d];
        int nx = rx + dx[d];
        int nGap = pow(selectSantaY - ny, 2) + pow(selectSantaX - nx, 2);
        if (oob(ny, nx)) continue;
        if (dirDis > nGap) {
            dir = d;
            dirDis = nGap;
        }
    }

    // 루돌프 이동
    int rry = ry + dy[dir];
    int rrx = rx + dx[dir];
    if (oob(rry, rrx)) return;
    if (map[rry][rrx] >= 1) {
        map[ry][rx] = 0;
        santaChain(rry, rrx, dir, turn);
        map[rry][rrx] = -1;
    } else {
        map[ry][rx] = 0;
        map[rry][rrx] = -1;
    }
}

bool thereIsNoSanta() {
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (map[i][j] >= 1) return false;
        }
    }
    return true;
}

int main() {
    cin >> N >> M >> P >> C >> D;

    int rY, rX;
    cin >> rY >> rX;
    map[rY][rX] = -1;
    for (int i = 0; i < P; i++) {
        int sN, sY, sX;
        cin >> sN >> sY >> sX;
        Santa s;
        s.stunTime = -1;
        s.point = 0;
        s.isOut = false;
        santa[sN] = s;
        map[sY][sX] = sN;
    }

    for (int turn = 1; turn <= M; turn++) {
        if (thereIsNoSanta()) {
            break;
        }

        rMove(turn);
        sMove(turn);
    }

    for (int i = 1; i <= P; i++) {
        cout << santa[i].point << " ";
    }

    return 0;
}