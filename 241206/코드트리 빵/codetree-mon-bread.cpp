#include <iostream>
#include <vector>
#include <queue>
using namespace std;
struct Person {
    int y, x;
    int ty, tx;
    bool isFinish;
};

int N, M;
int map[20][20];
vector<pair<int, int>> basecamps;
vector<pair<int, int>> marts;
Person people[40];


int dy[4] = { -1,0,0,1 };
int dx[4] = { 0,-1,1,0 };

bool oob(int y, int x) {
    return y < 0 || x < 0 || y >= N || x >= N;
}

void init() {
    cin >> N >> M;

    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> map[i][j];
            if (map[i][j] == 1) {
                basecamps.push_back({ i,j });
            }
        }
    }

    marts.push_back({ -1,-1 });
    for (int i = 0; i < M; i++) {
        int y, x;
        cin >> y >> x;
        marts.push_back({ y-1,x-1 });
    }

    for (int i = 0; i < 40; i++) {
        people[i].y = -1;
        people[i].x = -1;
        people[i].ty = -1;
        people[i].tx = -1;
        people[i].isFinish = false;
    }
}

int bfs(int sy,int sx, int ey, int ex) {
    
    int distance = 0;

    bool visited[20][20] = { false };
    queue<pair<pair<int, int>, int>> q;
    q.push({ {sy,sx}, 1 });
    visited[sy][sx] = true;

    while (!q.empty()) {
        int y = q.front().first.first;
        int x = q.front().first.second;
        int cnt = q.front().second;
        q.pop();
        if (y == ey && x == ex) {
            return cnt;
        }

        for (int d = 0; d < 4; d++) {
            int ny = y + dy[d];
            int nx = x + dx[d];

            if (oob(ny, nx))continue;
            if (visited[ny][nx])continue;
            if (map[ny][nx] == 2 || map[ny][nx]== 3) continue;

            q.push({ {ny,nx},cnt + 1 });
            visited[ny][nx] = true;
        }
    }
    return -1;
}

void startBasecamp(int day) {
    //cout << "BASECAMP\n";
    pair<int, int> minBasecamp = { 1e9,1e9 };
    int shortestDis = 1e9;
    int martY = marts[day].first;
    int martX = marts[day].second;

    for (auto basecamp : basecamps) {
        if (map[basecamp.first][basecamp.second] == 3)continue;
        int dis = bfs(martY, martX, basecamp.first, basecamp.second);
        //cout <<"bfs val " << dis << " positoin " << basecamp.first << " " << basecamp.second << "\n";
        if (dis != -1 && shortestDis > dis) {
            shortestDis = dis;
            minBasecamp = basecamp;
        }
        else if (shortestDis == dis) {
            if (minBasecamp.first > basecamp.first) {
                minBasecamp = basecamp;
            }
            else {
                if (minBasecamp.second > basecamp.second){
                    minBasecamp = basecamp;
                }
            }
        }
    }

    people[day].y = minBasecamp.first;
    people[day].x = minBasecamp.second;
    people[day].ty = martY;
    people[day].tx = martX;
    map[minBasecamp.first][minBasecamp.second] = 3;
}

void movePeople() {
    //cout << "MOVE PEOPLE\n";

    for (int i = 1; i <= M; i++) {
        Person& p = people[i]; // 사람 객체를 참조로 가져옴
        if (p.y == -1 || p.x == -1 || p.isFinish) continue; // 초기 상태거나 도착 완료 시 건너뜀

        bool visited[20][20] = { false }; // 방문 여부 초기화
        pair<int, int> before[20][20]; // 이전 좌표 추적용
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                before[j][k] = { -1, -1 };
            }
        }

        queue<pair<int, int>> q;
        q.push({ p.y, p.x });
        visited[p.y][p.x] = true;

        bool found = false; // 목적지 도달 여부

        while (!q.empty() && !found) {
            int y = q.front().first;
            int x = q.front().second;
            q.pop();

            for (int d = 0; d < 4; d++) {
                int ny = y + dy[d];
                int nx = x + dx[d];
                if (oob(ny, nx)) continue;
                if (visited[ny][nx]) continue;
                if (map[ny][nx] == 3 || map[ny][nx] ==2) continue; // 다른 사람이 점유한 경우 이동 불가

                visited[ny][nx] = true;
                before[ny][nx] = { y, x }; // 이전 좌표 저장
                q.push({ ny, nx });

                // 목적지 도달 시
                if (ny == p.ty && nx == p.tx) {
                    found = true;
                    break;
                }
            }
        }

        // 목적지로 이동할 경우 경로를 추적하여 한 칸 이동
        if (found) {
            pair<int, int> trace = { p.ty, p.tx };
            while (!(before[trace.first][trace.second].first == p.y &&
                before[trace.first][trace.second].second == p.x)) {
                trace = before[trace.first][trace.second];
            }
            p.y = trace.first;
            p.x = trace.second;
        }
    }
}


void scanMart() {
    for (Person &p : people) {
        if (p.ty != -1 && p.tx != -1 && p.y == p.ty && p.x == p.tx) {
            map[p.y][p.x] = 2;
            p.y = -1;
            p.x = -1;
            p.isFinish = true;
        }
    }
}

bool check() {
    bool isAllFinish = true;
    for (int i = 1; i <= M; i++) {
        //cout << "person " << i << " " <<people[i].y << " " << people[i].x << " | "<< people[i].ty << " " << people[i].tx << " "<< people[i].isFinish << " ";
        if (!people[i].isFinish) isAllFinish = false;
    }
    //cout << "\n";
    //for (int i = 0; i < N; i++) {
    //    for (int j = 0; j < N; j++) {
    //        cout << map[i][j] << " ";
    //    }
    //    cout << "\n";
    //}
    //cout << "\n";
    return isAllFinish;
}

void simiulation() {
    int day = 0;

    while (++day) {
        //cout << day << "\n";
        movePeople();

        scanMart();

        if(day <= M)startBasecamp(day);

        if (check()) {
            break;
        }
    }
    cout << day;
}

int main() {
    // 여기에 코드를 작성해주세요.
    init();

    simiulation();
    return 0;
}