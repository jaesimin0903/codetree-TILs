#include <iostream>
#include <queue>
#include <vector>

#define DEAD (0)

#define RIGHT (0)
#define DOWN (1)
#define LEFT (2)
#define UP (4)

using namespace std;

struct Tower {
    int y, x, attack, attackTime;
    int idx;
    bool isAction;
    bool isDead;
};

struct AttackComp {
    bool operator()(const Tower &a, const Tower &b) {
        if (a.attack != b.attack) return a.attack > b.attack;
        if (a.attackTime != b.attackTime) return a.attackTime < b.attackTime;
        if (a.y + a.x != b.y + b.x) return a.y + a.x < b.y + b.x;
        if (a.y != b.y) return a.y < b.y;
    }
};

struct DefenceComp {
    bool operator()(const Tower& a, const Tower& b) {
        if (a.attack != b.attack) return a.attack < b.attack;
        if (a.attackTime != b.attackTime) return a.attackTime > b.attackTime;
        if (a.y + a.x != b.y + b.x) return a.y + a.x > b.y + b.x;
        if (a.y != b.y) return a.y > b.y;
    }
};

int N, M, K;
int day = 0;

int dy[4] = { 0,1,0,-1 };
int dx[4] = { 1,0,-1,0 };

pair<int,int> map[11][11];
vector<Tower> towers;
priority_queue<Tower, vector<Tower>, AttackComp> attackPQ;
priority_queue<Tower, vector<Tower>, DefenceComp> defencePQ;

void debug() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
           cout <<  map[i][j].second << " ";
        }
        cout << "\n";
    }
}
pair<int,int> oob(int y, int x) {
    if (y < 0) y = M - 1;
    else if (y >= M) y = 0;
    else if (x < 0) x = N - 1;
    else if (x >= N) x = 0;
    return { y,x };
}

Tower selectAttackTower() {
    //부서지지 않은 포탑 중 가장 약한 포탑 N+M 공격력 증가
    while (!attackPQ.empty()) {
        attackPQ.pop();
    }

    //공격력이 가장 낮은 포탑
    //가장 최근에 공격한 포탑
    //행과 열의 합이 가장 큰 포탑
    //열 > 행 포탑
    for (auto t : towers) {
        if (!t.isDead) {
            attackPQ.push(t);
        }
    }
    //cout << attackPQ.top().attack << "\n";
    return attackPQ.top();
}

Tower selectDefenceTower() {
    //공격력이 가장 높은 포탑
    //공격한지 가장 오래된 포탑
    //행과열의 합이 가장 작은 포탑
    //열 < 행 포탑
    while (!defencePQ.empty()) {
        defencePQ.pop();
    }

    for (auto t : towers) {
        if (!t.isDead) {
            defencePQ.push(t);
        }
    }

    return defencePQ.top();
}

void makeMap(Tower& attackTower, Tower& defenceTower) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            map[i][j] = { -1,0 };
        }
    }

    for (auto t : towers) {
        map[t.y][t.x].first = t.idx;
        map[t.y][t.x].second = t.attack;
    }

    map[attackTower.y][attackTower.x].first = attackTower.idx;
    map[attackTower.y][attackTower.x].second = attackTower.attack + N + M;
    map[defenceTower.y][defenceTower.x].first = defenceTower.idx;
    map[defenceTower.y][defenceTower.x].second = defenceTower.attack;

    towers[attackTower.idx].isAction = true;
    towers[attackTower.idx].attackTime = day;
    towers[defenceTower.idx].isAction = true;
}

bool razor(Tower& attackTower, Tower& defenceTower) {
    //우 하 좌 상
    int aY = attackTower.y;
    int aX = attackTower.x;

    int dY = defenceTower.y;
    int dX = defenceTower.x;

    pair<int, int> before[11][11];
    bool visited[11][11] = { false };
    queue<pair<int, int>> q;

    visited[aY][aX] = true;
    q.push({ aY,aX });
    while (!q.empty()) {
        int y = q.front().first;
        int x = q.front().second;

        q.pop();
        
        if (y == dY && x == dX) {
            //cout << "razor\n";
            pair<int, int> trace = { dY, dX };
            map[dY][dX].second -= (attackTower.attack + N + M);
            while (!(before[trace.first][trace.second].first == aY && before[trace.first][trace.second].second == aX)) {
                trace = before[trace.first][trace.second];
                
                map[trace.first][trace.second].second -= (attackTower.attack + N +M) / 2;
                towers[map[trace.first][trace.second].first].isAction = true;
            }
            return true;
        }

        for (int d = 0; d < 4; d++) {
            int ny = y + dy[d];
            int nx = x + dx[d];
            ny = oob(ny, nx).first;
            nx = oob(ny, nx).second;
            //cout << "BFS DIR " << ny << " " << nx << "\n";
            if (visited[ny][nx]) continue;
            
            if (map[ny][nx].second == DEAD) continue;
            before[ny][nx] = { y,x };
            q.push({ ny,nx });
            visited[ny][nx] = true;
        }
    }

    return false;
}

void cannon(Tower& attackTower, Tower& defenceTower) {
    int cY[8] = { -1,-1,0,1,1,1,0,-1 };
    int cX[8] = { 0,1,1,1,0,-1,-1,-1 };

    map[defenceTower.y][defenceTower.x].second -= (attackTower.attack + N+M);
    for (int d = 0; d < 8; d++) {
        int ny = defenceTower.y + cY[d];
        int nx = defenceTower.x + cX[d];
        ny = oob(ny, nx).first;
        nx = oob(ny, nx).second;
        if (map[ny][nx].second <= 0) continue;
        if (ny == attackTower.y && nx == attackTower.x) continue;
        map[ny][nx].second -= (attackTower.attack+N+M) / 2;
        towers[map[ny][nx].first].isAction = true;
    }
}

void attack() {
    //가장 강한 포탑 공격
    Tower attackTower = selectAttackTower();

    Tower defenceTower = selectDefenceTower();

    makeMap(attackTower, defenceTower);
    
    if (!razor(attackTower, defenceTower)) {
        cannon(attackTower, defenceTower);
    }
}

void repair() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++){
            if (map[i][j].first == -1)continue;
            if (!towers[map[i][j].first].isAction && map[i][j].second > 0) {
 //               cout << "REPAIR IDX " << map[i][j].first << "\n";
                towers[map[i][j].first].attack += 1;
                map[i][j].second++;
            }
        }
    }
}

void towerSetting1() {

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            int towerIdx = map[i][j].first;
            if (towerIdx == -1) continue;
            if (map[i][j].second <= 0 && !towers[towerIdx].isDead) {
                towers[towerIdx].attack = 0;
                towers[towerIdx].isDead = true;
            }
            else {
                towers[towerIdx].attack = map[i][j].second;
                //towers[towerIdx].isAction = false;
            }
        }
    }
}

void towerSetting2() {
    
    for (int i = 0; i <N; i++) {
        for (int j = 0; j < M; j++) {
            int towerIdx = map[i][j].first;
            if (towerIdx == -1) continue;
            if (map[i][j].second <= 0 && !towers[towerIdx].isDead) {
                towers[towerIdx].attack = 0;
                towers[towerIdx].isDead = true;
            }
            else {
                towers[towerIdx].attack = map[i][j].second;
                towers[towerIdx].isAction = false;
            }
        }
    }
}

void clear() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            map[i][j] = { -1,0 };
        }
    }
}

void init() {
    cin >> N >> M >> K;

    int tIdx = 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            map[i][j].first = -1;
            cin >> map[i][j].second;
            if (map[i][j].second != 0) {
                map[i][j].first = tIdx;
                Tower t;
                t.y = i;
                t.x = j;
                t.attack = map[i][j].second;
                t.attackTime = 0;
                t.isAction = false;
                t.isDead = false;
                t.idx = tIdx++;
                towers.push_back(t);
            }
        }
    }
}

int findKingTower() {
    int cnt = 0;
    int maxAttack = 0;
    for (auto t : towers) {
        if (!t.isDead) {
            cnt++;
            maxAttack = max(maxAttack, t.attack);
        }
    }

    return maxAttack;
}

bool onlyOne() {
    int cnt = 0;
    for (auto t : towers) {
        if (!t.isDead) {
            cnt++;
        }
    }

    if (cnt <= 1) return true;
    return false;
}

void setMap() {
    clear();
    for (auto t : towers) {
        map[t.y][t.x] = { t.idx, t.attack };
    }
    /*debug();
    cout << "\n";*/
}

void simulation() {
    while (day++ < K && !onlyOne()) {
        //cout << "\n------------------GOOD MORING DAY "<< day <<" ---------------------\n";
        //debug();
        attack();
        towerSetting1();
        setMap();
        int cnt = 0;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                if (map[i][j].second > 0)cnt++;
            }
        }
        if (cnt <= 1) {
            break;
        }

        repair();
        towerSetting2();
        setMap();
        //cout << "\n------------------AFTER ATTACK---------------------\n";
        //debug();


    }
    cout << findKingTower();
}
int main() {
    // 여기에 코드를 작성해주세요.
    init();

    simulation();
    return 0;
}