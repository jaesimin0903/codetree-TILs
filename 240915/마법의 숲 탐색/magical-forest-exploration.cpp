#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include<queue>
using namespace std;

int R, C, K;
int map[80][80];

int dy[4] = { -1,0,1,0 };
int dx[4] = { 0,1,0,-1 };

bool oob(int y, int x) {
	return y < 0 || x < 0 || y >= R + 3 || x >= C;
}

void print() {
	for (int i = 0; i < R + 3; i++) {
		for (int j = 0; j < C; j++) {
			cout << map[i][j] << "\t";
		}
		cout << "\n";
	}
}
void clearMap(int y, int x) {
	map[y][x] = 0;
	for (int d = 0; d < 4; d++) {
		int ny = y + dy[d];
		int nx = x + dx[d];
		map[ny][nx] = 0;
	}
}

bool canMove(int sy, int sx, int ey, int ex, int num) {
	//cout << "run canMove\n";
	bool canMove = true;

	int tmp[3][3];
	fill(&tmp[0][0], &tmp[0][0] + 3 * 3, 0);

	tmp[1][1] = map[sy][sx];
	for (int d = 0; d < 4; d++) {
		int ny = sy + dy[d];
		int nx = sx + dx[d];
		tmp[1 + dy[d]][1 + dx[d]] = map[ny][nx];
	}


	//초기위치 초기화
	clearMap(sy, sx);

	//print();
	//다음 움직일 곳으로 갈수 있는지?
	for (int d = 0; d < 4; d++) {
		int ny = ey + dy[d];
		int nx = ex + dx[d];
		//cout << ny << " " << nx << "\n";
		if (map[ny][nx] != 0 || oob(ny, nx)) {
			//cout << "cant move\n";
			map[sy][sx] = tmp[1][1];
			for (int dd = 0; dd < 4; dd++) {
				int nny = sy + dy[dd];
				int nnx = sx + dx[dd];
				map[nny][nnx] = tmp[1 + dy[dd]][1 + dx[dd]];
				//cout << tmp[1 + dy[dd]][1 + dx[dd]] << " ";
			}
			return false;
		}
	}
	map[sy][sx] = tmp[1][1];
	for (int dd = 0; dd < 4; dd++) {
		int nny = sy + dy[dd];
		int nnx = sx + dx[dd];
		map[nny][nnx] = tmp[1 + dy[dd]][1 + dx[dd]];
		//cout << tmp[1 + dy[dd]][1 + dx[dd]] << " ";
	}
	return true;
}

void setMap(int sy, int sx, int ey, int ex, int num, int d, int lr) {
	//cout << "setMap " << ey << ex << num<<"\n";
	int direction = -1;
	if (d != -1) {
		if (lr == 1) d = (d + 1) % 4;
		else if(lr==2) d = (d + 3) % 4;
	}
	else {
		for (int dir = 0; dir < 4; dir++) {
			int ny = sy + dy[dir];
			int nx = sx + dx[dir];
			if (map[ny][nx] < 0) direction = dir;
		}
	}
	//cout << "before clear\n";
	//print();
	clearMap(sy, sx);

	map[ey][ex] = num;
	for (int dir = 0; dir < 4; dir++) {
		int ny = ey + dy[dir];
		int nx = ex + dx[dir];
		map[ny][nx] = num;
	}


	//입구 설정
	if (d != -1) {
		int ny = ey + dy[d];
		int nx = ex + dx[d];
		map[ny][nx] = -1 * num;
	}
	else {
		//cout << ey << " " << ex << "\n";
		map[ey + dy[direction]][ex + dx[direction]] = -1 * num;
	}
	//print();

}
bool canMoveDown(int y, int x, int num,int dir) {
	int dy = y + 1;
	int dx = x;
	if (dir == -1) return false;
	if (canMove(y, x, dy, dx, num)) {
		setMap(y, x, dy, dx, num, dir, -1);
		return true;
	}
	else {
		setMap(y, x, y, x, num, dir, -1);
	}
	return false;
}

int canMoveRL(int y, int x, int num) {


	//현재 위치에서 오른쪽 왼쪽 갈수있는지
	int d = -1;
	//cout << y << " " << x << " ";
	for (int dir = 0; dir < 4; dir++) {
		int ny = y + dy[dir];
		int nx = x + dx[dir];
		//cout << map[ny][nx] << " ";
		if (oob(ny, nx)) continue;
		if (map[ny][nx] <0 )
			d = dir;
	}

	while (canMoveDown(y, x, num, d)) {
		y += 1;
	}
	//y -= 1;


	int ly = y;
	int lx = x - 1;

	if (canMove(y, x, ly, lx, num) && canMove(y, x, ly + 1, lx, num)) {
		setMap(y, x, ly + 1, lx, num, d, 2);
		//cout << "move left\n";
		return 2;
	}

	setMap(y, x, y, x, num, -1, -1);

	//move right
	int ry = y;
	int rx = x + 1;

	if (canMove(y, x, ry, rx, num) && canMove(y, x, ry + 1, rx, num)) {
		setMap(y, x, ry + 1, rx, num, d, 1);
		//cout << "move right\n";
		return 1;
	}
	setMap(y, x, y, x, num, -1, -1);
	return 0;
}


bool isFull() {
	for (int i = 0; i < C; i++) {
		if (map[2][i] >= 1) {
			return true;
		}
	}
	return false;
}

int bfs(int y, int x) {
	queue<pair<int, int>> q;
	int maxDepth = 0;
	q.push({ y,x });
	bool v[80][80];
	fill(&v[0][0], &v[0][0] + 80 * 80, false);
	v[y][x] = true;

	//print();
	while (!q.empty()) {
		int yy = q.front().first;
		int xx = q.front().second;
		q.pop();
		for (int d = 0; d < 4; d++) {
			int ny = yy + dy[d];
			int nx = xx + dx[d];
			if (oob(ny, nx) || v[ny][nx])continue;
			if (map[ny][nx] == 0)continue;
			if (map[yy][xx] > 0) {
				if (map[ny][nx] > 0) {
					if (map[ny][nx] != map[yy][xx])continue;
				}
				else if (map[ny][nx] < 0) {
					if (map[ny][nx] != -1 * map[yy][xx])continue;
				}
				//cout << yy << " " << xx << " " << map[ny][nx] << "-\n";
				q.push({ ny,nx });
				v[ny][nx] = true;
				maxDepth = max(maxDepth, ny);
			}
			else if (map[yy][xx] < 0) {
				//cout << yy << " " << xx << " " << map[yy][xx] << " " << map[ny][nx] << "-\n";
				q.push({ ny,nx });
				v[ny][nx] = true;
				maxDepth = max(maxDepth, ny);
			}
		}
	}
	//cout << "값 : " << maxDepth - 2 <<"\n";
	return maxDepth - 2;
}


int main() {
	// 여기에 코드를 작성해주세요.

	cin >> R >> C >> K;
	int ans = 0;
	for (int i = 0; i < K; i++) {
		int sRow, dir;
		cin >> sRow >> dir;
		//인덱스 0 으로 맞추기위한 빼기
		//0 북 1 동 2 남 3 서
		sRow -= 1;

		int curY = 1;
		int curX = sRow;
		while (canMoveDown(curY, curX, i + 1,dir)) {
			//print();
			//cout << "\n";
			curY += 1;
		}


		while (true) {
			int res = canMoveRL(curY, curX, i + 1);
			if (res == 1) {
				curY += 1; curX += 1;
			}
			else if (res == 2) {
				curY += 1; curX -= 1;
			}
			else {
				break;
			}

		}

		if (isFull()) {

			//cout << "clear\n";
			fill(&map[0][0], &map[0][0] + 80 * 80, 0);
		}
		else {
			//bfs 점수 계산
			ans += bfs(curY, curX);
		}
	}
	cout << ans;
	return 0;
}