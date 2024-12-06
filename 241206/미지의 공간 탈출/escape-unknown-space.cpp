#include <algorithm>
#include <iostream>
#include <queue>
#include <cassert>

using namespace std;

const int MAXN = 20;
const int MAXM = 20;
const int MAXF = 400;
const int MAXP = MAXF * 6;

const int INF = 1e9 + 10;

int SpaceMap[MAXN + 10][MAXN + 10]; // 미지의 공간의 평면도
int SpaceMapCellId[MAXN + 10][MAXN + 10]; // 평면도의 각 셀에 대응하는 그래프 정점의 번호를 저장하는 배열
int TimeWall[6][MAXM + 10][MAXM + 10]; // 시간의 벽의 단면도
int TimeWallCellId[6][MAXM + 10][MAXM + 10]; // 시간의 벽의 단면도의 각 셀에 대응하는 그래프 정점의 번호를 저장하는 배열

// 시간 이상 현상에 대한 정보를 저장할 구조체
struct AbnormalTimeEvent {
    // 시간 이상 현상이 시작점의 행번호, 열번호, 방향, 확장 주기, 시간 이상 현상의 진행여부를 차례로 저장합니다
    int xpos, ypos, direction, cycle, alive;
};

AbnormalTimeEvent events[MAXF + 10];

// 그래프를 저장할 인접리스트
vector<vector<int> > Graph;

// 타임머신에서 해당 번호의 셀까지 도달하는데 필요한 최소 턴 횟수
int dist[MAXP];

// 그래프 생성 함수
void build_graph(int N, int M) {
    int cnt = 0;

    // 각 셀에 대해 대응될 번호를 차례로 부여합니다
    // 평면도 중 시간의 벽이 아닌 부분의 셀들을 순회한 후,
    // 단면도에 속하는 셀들을 단면도의 동쪽 -> 남쪽 -> 서쪽 -> 북쪽 -> 위쪽 순으로 셀들을 순회하면서 번호를 부여합니다
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (SpaceMap[i][j] != 3) { // 시간의 벽이 있는 셀이 아닌 경우에만 번호르 부여합니다
                cnt++;
                SpaceMapCellId[i][j] = cnt;
            }
        }
    }

    // 단면도의 동쪽, 남쪽, 서쪽, 북쪽, 위쪽 순으로 순회하며 셀에 번호를 부여합니다
    for (int t = 0; t < 5; t++) {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < M; j++) {
                cnt++;
                TimeWallCellId[t][i][j] = cnt;
            }
        }
    }

    // 부여한 번호의 정점들로 구성된 그래프
    // 정점의 번호에 대응되는 셀과 인접한 셀의 번호를 가지는 정점과 간선으로 연결합니다
    // 최대 4개의 정점과 연결될 수 있습니다
    // 평면도(단면도)에서, 오른쪽으로 인접한 경우 0, 아래쪽으로 인접한 경우 1, 왼쪽으로 인접한 경우 2, 위쪽으로 인접한 경우 3의 인덱스에 저장합니다
    Graph = vector<vector<int> >(cnt + 1, vector<int>(4, -1));

    // 간선을 추가하는 작업을 위해 사용할 dx, dy 배열
    // 동, 남, 북, 서에 대응되는 순서로 채워져 있습니다
    int dx[4] = {0, 1, 0, -1};
    int dy[4] = {1, 0, -1, 0};

    // 평면도에 속하는 셀에 대응되는 번호의 정점 쌍에 대해 간선을 추가합니다
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (SpaceMap[i][j] != 3) { // 현재 셀에 장애물이 없는 경우

                int idx = SpaceMapCellId[i][j];

                // 동, 남, 북, 서 순으로 인접한 셀들을 탐색합니다
                for (int dd = 0; dd < 4; dd++) {
                    int nx = i + dx[dd];
                    int ny = j + dy[dd];

                    // 범위를 벗어나면 넘어갑니다
                    if (nx < 0 || ny < 0 || nx >= N || ny >= N) continue;
                    // 셀에 장애물이 있는 경우 넘어갑니다
                    if (SpaceMap[nx][ny] == 3) continue;

                    // 그렇지 않은 경우, 이어줍니다
                    Graph[idx][dd] = SpaceMapCellId[nx][ny];
                }
            }
        }
    }

    // 단면도의 동쪽, 남쪽, 서쪽, 북쪽에 있는 셀들이 인접할 경우
    // 대응되는 번호의 정점들을 이어줍니다
    for (int t = 0; t < 4; t++) {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < M; j++) {

                int idx = TimeWallCellId[t][i][j];

                // 위와 비슷하게 4방향 탐색
                for (int dd = 0; dd < 4; dd++) {
                    int nx = i + dx[dd];
                    int ny = j + dy[dd];

                    // 행 범위가 넘어갔을 경우 통과
                    if (nx < 0 || nx >= M) continue;

                    // 열 번호가 0보다 작아질 경우, 시계방향순으로 하나 앞에 있는 단면도의 가장 오른쪽 열의 셀과 인접합니다
                    if (ny < 0) {
                        Graph[idx][dd] = TimeWallCellId[(t + 1) % 4][nx][M - 1];
                    }
                        // 행 번호가 M-1보다 커질 경우, 반시계방향순으로 하나 앞에 있는 단면도의 가장 왼쪽 열의 셀과 인접합니다
                    else if (ny >= M) {
                        Graph[idx][dd] = TimeWallCellId[(t + 3) % 4][nx][0];
                    }
                        // 그 외의 경우 평면도의 경우처럼 이어줍니다
                    else {
                        Graph[idx][dd] = TimeWallCellId[t][nx][ny];
                    }

                }
            }

        }
    }

    // 외쪽 단면도에 속하는 셀들에 대응되는 번호의 정점 쌍에 대해 간선을 추가합니다
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {

            int idx = TimeWallCellId[4][i][j];

            for (int dd = 0; dd < 4; dd++) {
                int nx = i + dx[dd];
                int ny = j + dy[dd];
                // 범위를 벗어날 경우 넘어갑니다
                if (nx < 0 || ny < 0 || nx >= M || ny >= M) continue;
                // 그렇지 않을 경우 이어줍니다
                Graph[idx][dd] = TimeWallCellId[4][nx][ny];
            }
        }
    }

    // 위쪽 단면도와 인접한 동쪽, 남쪽, 서쪽, 북쪽 단면도의 셀들에 대해
    // 대응되는 번호의 정점을 잇는 간선을 추가합니다

    // 동쪽 단면도와 인접한 셀들의 경우
    for (int i = 0; i < M; i++) {
        int idx = TimeWallCellId[4][i][M - 1]; // 인접한 위쪽 단면도의 셀의 번호
        int idy = TimeWallCellId[0][0][M - 1 - i]; // 인접한 동쪽 단면도의 셀의 번호
        Graph[idx][0] = idy;
        Graph[idy][3] = idx;
    }
    // 남쪽 단면도와 인접한 셀들의 경우
    for (int i = 0; i < M; i++) {
        int idx = TimeWallCellId[4][M - 1][i]; // 인접한 위쪽 단면도의 셀의 번호
        int idy = TimeWallCellId[1][0][i]; // 인접한 남쪽 단면도의 셀의 번호
        Graph[idx][1] = idy;
        Graph[idy][3] = idx;
    }
    // 서쪽 단면도와 인접한 셀들의 경우
    for (int i = 0; i < M; i++) {
        int idx = TimeWallCellId[4][i][0]; // 인접한 위쪽 단면도의 셀의 번호
        int idy = TimeWallCellId[2][0][i]; // 인접한 서쪽 단면도의 셀의 번호
        Graph[idx][2] = idy;
        Graph[idy][3] = idx;
    }
    // 북쪽 단면도와 인접한 셀들의 경우
    for (int i = 0; i < M; i++) {
        int idx = TimeWallCellId[4][0][i]; // 인접한 위쪽 단면도의 셀의 번호
        int idy = TimeWallCellId[3][0][M - 1 - i]; // 인접한 북쪽 단면도의 셀의 번호
        Graph[idx][3] = idy;
        Graph[idy][3] = idx;
    }

    // 평면도에서 시간의 벽이 시작하는 셀의 행 번호, 열 번호
    int timewallStartx = -1;
    int timewallStarty = -1;

    // 평면도에서 시간의 벽이 시작하는 셀의 위치를 탐색
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (SpaceMap[i][j] == 3) {
                timewallStartx = i;
                timewallStarty = j;
                break;
            }
        }
        if (timewallStartx != -1) break;
    }

    // 평면도와 인접하는 단면도 셀들에 대응되는 번호의 정점을 잇는 간선 추가
    // 동쪽 단면도의 경우
    if (timewallStarty + M < N) {
        for (int i = 0; i < M; i++) {
            int idx = TimeWallCellId[0][M - 1][i];
            int idy = SpaceMapCellId[timewallStartx + (M - 1) - i][timewallStarty + M];
            Graph[idx][1] = idy;
            Graph[idy][2] = idx;
        }
    }
    // 남쪽 단면도의 경우
    if (timewallStartx + M < N) {
        for (int i = 0; i < M; i++) {
            int idx = TimeWallCellId[1][M - 1][i];
            int idy = SpaceMapCellId[timewallStartx + M][timewallStarty + i];
            Graph[idx][1] = idy;
            Graph[idy][3] = idx;
        }
    }
    // 서쪽 단면도의 경우
    if (timewallStarty > 0) {
        for (int i = 0; i < M; i++) {
            int idx = TimeWallCellId[2][M - 1][i];
            int idy = SpaceMapCellId[timewallStartx + i][timewallStarty - 1];
            Graph[idx][1] = idy;
            Graph[idy][0] = idx;
        }
    }
    // 북쪽 단면도의 경우
    if (timewallStartx > 0) {
        for (int i = 0; i < M; i++) {
            int idx = TimeWallCellId[3][M - 1][i];
            int idy = SpaceMapCellId[timewallStartx - 1][timewallStarty + (M - 1) - i];
            Graph[idx][1] = idy;
            Graph[idy][1] = idx;
        }
    }

    return;
}

int main() {
    int N, M, E; // 미지의 공간의 한 변의 길이, 시간의 벽 한 변의 길이, 시간 이상 현상의 개수

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N >> M >> E;

    // 공간의 평면도 입력
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> SpaceMap[i][j];
        }
    }

    // 시간의 벽의 동쪽 단면도 입력
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            cin >> TimeWall[0][i][j];
        }
    }

    // 시간의 벽의 서쪽 단면도 입력
    // 구현의 편의를 위해서 TimeWall[2][][]에 서쪽 단면도 정보 저장
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            cin >> TimeWall[2][i][j];
        }
    }

    // 시간의 벽의 남쪽 단면도 입력
    // 구현의 편의를 위해서 TimeWall[1][][]에 남쪽 단면도 정보 저장
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            cin >> TimeWall[1][i][j];
        }
    }

    // 시간의 벽의 북쪽 단면도 입력
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            cin >> TimeWall[3][i][j];
        }
    }

    // 시간의 벽의 위쪽 단면도 입력
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            cin >> TimeWall[4][i][j];
        }
    }

    // 시간 이상 현상에 대한 정보 입력
    for (int i = 1; i <= E; i++) {
        cin >> events[i].xpos >> events[i].ypos >> events[i].direction >> events[i].cycle;
        if (events[i].direction == 1) events[i].direction = 2;
        else if (events[i].direction == 2) events[i].direction = 1;

        events[i].alive = 1;
    }

    // 공간의 상황에 대응되는 그래프 생성
    build_graph(N, M);


    // 생성된 그래프의 정점의 개수는
    // N*N - M*M + 5*M*M = N*N + 4*M*M 개
    // -1로 배열의 값을 초기화한다.
    fill(dist, dist + (N * N) + 4 * (M * M) + 1, -1);

    // 장애물인 경우 도달할 수 없으므로 미리 아주 큰 값으로 세팅합니다
    // 평면도에 있는 장애물의 경우
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (SpaceMap[i][j] == 3) continue;
            if (SpaceMap[i][j] == 1) {
                int idx = SpaceMapCellId[i][j];
                dist[idx] = INF;
            }
        }
    }

    // 평면도에서 시간 이상 현상의 시작점도 도달 불가능하므로 장애물과 같이 처리합니다
    for (int i = 1; i <= E; i++) {
        int x = events[i].xpos;
        int y = events[i].ypos;
        int idx = SpaceMapCellId[x][y];
        dist[idx] = INF;
    }

    // 단면도 위에 있는 장애물의 경우 역시 똑같이 처리합니다
    for (int t = 0; t < 5; t++) {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < M; j++) {
                if (TimeWall[t][i][j] == 1) {
                    int idx = TimeWallCellId[t][i][j];
                    dist[idx] = INF;
                }
            }
        }
    }

    // BFS를 진행할 큐
    queue<int> que;

    int cell_start = -1;
    int cell_end = -1;

    // 탈출구 위치 탐색
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (SpaceMap[i][j] == 4) {
                cell_end = SpaceMapCellId[i][j];
                break;
            }
        }
    }

    // 타임머신의 시작점 탐색
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            if (TimeWall[4][i][j] == 2) {
                cell_start = TimeWallCellId[4][i][j];
                break;
            }
        }
    }

    que.push(cell_start);
    dist[cell_start] = 0;

    for (int runs = 1;; runs++) {

        // 현재 턴에 확장하는 이상현상이 있으면 영향을 받는 셀을 업데이트합니다
        for (int i = 1; i <= E; i++) {
            if (!events[i].alive) continue; // 더 이상 확산하지 않는 이상 현상이면 넘어갑니다
            if (runs % events[i].cycle) continue; // 지금 턴에 확산하지 않으면 넘어갑니다

            int nx = events[i].xpos, ny = events[i].ypos; // 이상 현상의 시작점

            // 이상현상의 방향에 따라 영향을 받는 셀의 좌표를 구합니다
            if (events[i].direction == 0) {
                ny += (runs / events[i].cycle);
            } else if (events[i].direction == 1) {
                nx += (runs / events[i].cycle);
            } else if (events[i].direction == 2) {
                ny -= (runs / events[i].cycle);
            } else {
                nx -= (runs / events[i].cycle);
            }

            if (nx < 0 || ny < 0 || nx >= N || ny >= N) {
                events[i].alive = 0;
                continue;
            }

            // 이상현상이 장애물이나 탈출구, 시간의 벽을 만난 경우, 확산하지 않습니다
            if (SpaceMap[nx][ny]) {
                events[i].alive = 0;
                continue;
            }

            // 대응되는 셀의 번호는 이제 타임머신이 도달할 수 없습니다
            int idx = SpaceMapCellId[nx][ny];
            dist[idx] = INF;
        }

        // 이번턴에 도달 가능한 셀들의 번호를 저장할 벡터
        vector<int> next_cells;

        // 이번턴에 도달 가능한 셀들을 찾습니다
        while (!que.empty()) {
            int idx = que.front();
            que.pop();

            for (int i = 0; i < 4; i++) {
                int idy = Graph[idx][i];
                if (idy == -1) continue; // 해당 방향으로 이동가능한 셀이 없는 경우 통과합니다
                if (dist[idy] != -1) continue; // 이미 최소 턴의 수를 계산한 셀의 경우 통과합니다
                dist[idy] = runs;
                next_cells.push_back(idy); // 이번에 새로 도달 가능한 셀의 번호를 추가합니다
            }
        }

        // 새로 도달 가능한 셀들이 없으면 종료합니다
        if (!(int) next_cells.size()) {
            break;
        }

        // 새로 도달 가능한 셀들에 대응하는 번호를 큐에 추가합니다
        for (int i = 0; i < (int) next_cells.size(); i++) {
            que.push(next_cells[i]);
        }

        // 탈출구에 가기 위해 필요한 최소 턴수를 구했다면, 종료합니다.
        if (dist[cell_end] != -1) {
            break;
        }
    }

    // 정답을 출력합니다.
    // 불가능하면 -1이 출력됩니다
    cout << dist[cell_end] << "\n";

    return 0;
}