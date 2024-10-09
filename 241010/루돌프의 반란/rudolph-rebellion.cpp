#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

//1루돌프의 움직임
//가장 가까운 산타에게 1칸 돌진
//가장 가까운 산타가 2명일시 r좌표가 큰 그리고 같다면 c좌표가 큰 산타에게 돌진
//루돌프는 8방향 움직임가능

//2산타의 움직임
//기절, 탈락한 산타는 움직일 수 업슴
//산타는 루돌프에게 거리가 가장 가까워지는 방향으로 1칸 움직임
//4방향 움직임
//산타는 다른 산타와 중복 불가
//움직일 수 있는 칸이없다면 정지
//가ㅏ장 가까워지는 방향이 여러개라면 상우하좌 순으로 움직임

//3 충돌
//산타와 루돌프가 같은칸이라면 충돌
//루돌프가 움직여서 충돌인경우 산타 C점수 획득
//산타는 루돌프가 이동해온 반대방향으로 d칸 밀려남
//밀려난 위치가 게임판 밖이면 탈락
//밀려난 칸에 다른 산타가 있따면 상호작용

//4 상호작용
//충돌후 작지하는 산타의 방향으로 1칸 밀려남 그옆에 다른 산타가 있다면 연쇄반응

//5. 산타는 루돌프와 충돌 후 기절 현재 턴 + 1까지 기절 2가 된다면 정상
//밀려나갈 수는 있음
//루돌프는 기절한 산타를 돌진 대상으로 선캑할 수 이슴

//6. 게임 종료
//루돌프 산타 순서대로 움직임
//산타 모두 탈락하면 즉시 게임 종료
//매턴 이후 탈락하지 않은 산타들에게1 점 추가 부여
//각산타가 얻은 최종 점수를 구하자

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
    return y<1 || x < 1 || y>N || x>N;
}

void santaSantaChain(int y, int x, int dir) {
    int dy[8] = { -1,-1,0,1,1,1,0,-1 };
    int dx[8] = { 0,1,1,1,0,-1,-1,-1 };

    if (map[y][x] == 0) return;
    //cout << map[y][x] << "\n";
    int curSantaNum = map[y][x];
    int ny = y + dy[dir];
    int nx = x + dx[dir];
    map[y][x] = 0;
    //cout << "chain " << curSantaNum << " dir : " << dir << "\n";
    if (oob(ny, nx)) {
        //cout << "oob\n";
        santa[curSantaNum].isOut = true;
    }
    else if (map[ny][nx] >= 1) {

        santaSantaChain(ny, nx, dir);
        map[ny][nx] = curSantaNum;
    }else map[ny][nx] = curSantaNum;
}

void santaChain(int y, int x, int dir, int turn) {
    int dy[8] = { -1,-1,0,1,1,1,0,-1 };
    int dx[8] = { 0,1,1,1,0,-1,-1,-1 };

    int curSantaNum = map[y][x];
    santa[curSantaNum].stunTime = turn;
    santa[curSantaNum].point += C;

    map[y][x] = 0;
    if (oob(y + dy[dir] * C, x + dx[dir] * C)) santa[curSantaNum].isOut = true;
    else if (map[y + dy[dir] * C][x + dx[dir] * C] >= 1) {
        santaSantaChain(y + dy[dir] * C, x + dx[dir] * C, dir);
        map[y + dy[dir] * C][x + dx[dir] * C] = curSantaNum;
    }
    else map[y + dy[dir] * C][x + dx[dir] * C] = curSantaNum;

}

void sMove(int turn) {
    pair<int, int> s[31];
    int dy[4] = { -1,0,1,0 };
    int dx[4] = { 0,1,0,-1 };

    int ry, rx;
    //find r's point
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

    //산타 1번부터 움직임
    for (int i = 1; i <= P; i++) {
        //기절한 상태면 패스
        if (santa[i].stunTime + 1 >= turn)continue;

        int curY = 0;
            int curX = 0;

        for (int a = 1; a<= N; a++) {
            for (int b = 1; b <= N; b++) {
                if (map[a][b] == i) {
                    curY = a;
                    curX = b;
                }
            }
        }

        if (curY == 0) continue;

        //cout << "stuntime number : " << i << " " << santa[i].stunTime << " " << turn << "\n";

        int gap = pow(curY - ry,2) + pow(curX - rx,2);

        int moveY = 0;
        int moveX = 0;
        int dir = -1;

        for (int d = 0; d < 4; d++) {
            int ny = curY + dy[d];
            int nx = curX + dx[d];
            if (oob(ny, nx))continue;
            if (map[ny][nx] >= 1) continue;
            int nGap = pow(ry - ny,2) + pow(rx - nx,2);
            //cout << nGap << " " << gap << "\n";
            if (gap > nGap) {
                gap = nGap;
                moveY = ny;
                moveX = nx;
                dir = d;
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
        //santa move
        if (moveY != 0) {
            map[curY][curX] = 0;
            //도착한 곳이 루돌프라면
            if (map[moveY][moveX] == -1) {
                //점수증가
                santa[i].point += D;
                //연쇄반응 0 2 4 6
                santa[i].stunTime = turn;
                dir = (dir + 2) % 4;
                //cout <<moveX << " " << moveY << " "<< dir << "\n";
                //날아가는 곳이 장외라면
                if (oob(moveY + dy[dir] * D, moveX + dx[dir] * D)) {
                    santa[i].isOut = true;
                }//벗어난다면
                else {
                    int chainDir = dir * 2;
                    santaSantaChain(moveY + dy[dir] * D, moveX + dx[dir] * D, chainDir);
                    map[moveY + dy[dir] * D][moveX + dx[dir] * D] = i;
                }
            }
            else {
                map[moveY][moveX] = i;
            }

        }
        else {
            //cout <<i << " "<< "santa doesnt move\n";
        }
        //printMap(); cout << "\n";
    }

    for (int i = 1; i <= P; i++) {
        if (!santa[i].isOut)santa[i].point++;
    }
}



void rMove(int turn) {
    int dy[8] = { -1,-1,0,1,1,1,0,-1 };
    int dx[8] = { 0,1,1,1,0,-1,-1,-1 };
    int ry, rx;
    //find r's point
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (map[i][j] == -1) {
                ry = i; rx = j;
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
                //선택된 산타거리보다 작다면 선택
                if (selectSantaDis > dis) {
                    selectSantaDis = dis;
                    selectSantaNum = map[i][j];
                    selectSantaY = i;
                    selectSantaX = j;
                }
                else if (selectSantaDis == dis) {
                    if (selectSantaY < i) {//거리가같고 선택된 산타의 R 보다 크다면
                        selectSantaDis = dis;
                        selectSantaNum = map[i][j];
                        selectSantaY = i;
                        selectSantaX = j;
                    }
                    else if (selectSantaY == i) {
                        if (selectSantaX < j) {
                            selectSantaDis = dis;
                            selectSantaNum = map[i][j];
                            selectSantaY = i;
                            selectSantaX = j;
                        }
                    }
                }
            }
        }
    }

    int dir = -1;
    int dirDis = 1e9;
    //루돌프가 선택된 산타까지 최단거리로 움직일 수 있는 위치 찾기
    for (int d = 0; d < 8; d++) {
        int ny = ry + dy[d];
        int nx = rx + dx[d];
        int nGap = pow(selectSantaY - ny,2) + pow(selectSantaX - nx,2);
        if (oob(ny, nx))continue;
        if (dirDis > nGap) {
            dir = d;
            dirDis = nGap;
        }

    }

    //루돌프 이동
    int rry = ry + dy[dir];
    int rrx = rx + dx[dir];
    if (oob(rry, rrx)) return;
    //움직이려는 위치에 산타가있다면 연쇄반응
    if (map[rry][rrx] >= 1) {
        map[ry][rx] = 0;
        santaChain(rry, rrx, dir, turn);
        map[rry][rrx] = -1;
    }
    else {
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
    //printMap(); cout << "\n";
    for (int turn = 1; turn <= M; turn++) {
        if (thereIsNoSanta()) {
            break;
        }

        rMove(turn);
         //cout << "rMove\n";
         //printMap();
        sMove(turn);
        //cout << "sMove\n";
        //printMap();

        //for (int i = 1; i <= P; i++) {
        //    cout << santa[i].point << " ";
        //}
        //cout << '\n';
    }

    for (int i = 1; i <= P; i++) {
        cout << santa[i].point << " ";
    }
    return 0;
}