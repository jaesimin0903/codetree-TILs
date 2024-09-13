#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include<queue>
using namespace std;

int R, C, K;
int map[80][80];

int dy[4] = { 0,1,0,-1 };
int dx[4] = { 1,0,-1,0 };

bool oob(int y, int x) {
	return y < 0 || x < 0 || y >= R + 3 || x >= C;
}

void print() {
	for (int i = 0; i < R+3; i++) {
		for (int j = 0; j < C; j++) {
			cout << map[i][j];
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

bool canMove(int sy, int sx, int y, int x,int num) {
	//cout << "canMove\n";
	bool canMove = true;

	int tmp[3][3];

	for (int d = 0; d < 4; d++) {
		int ny = y + dy[d];
		int nx = x + dx[d];
		tmp[1 + dy[d]][1 + dx[d]] = map[ny][nx];
	}
	//초기위치 초기화
	clearMap(sy, sx);

	//print();
	//밑으로 갔을때 갈수 있는지?
	for (int d = 0; d < 4; d++) {
		int ny = y + dy[d];
		int nx = x + dx[d];
		//cout << ny << " " << nx << "\n";
		if (map[ny][nx] != 0 || oob(ny,nx)) {
			cout << "cant move\n";
			for (int dir = 0; dir < 4; dir++) {
				int nny = y + dy[dir];
				int nnx = x + dx[dir];
				map[nny][nnx] = tmp[1 + dy[dir]][1 + dx[dir]];
			}
			return false;
		}
	}
	return canMove;
}

void setMap(int sy, int sx, int ey, int ex, int num, int d) {
	//cout << "setMap " << ey << ex << num<<"\n";
	int direction = -1;
	if(d!=-1)d = (d + 1) % 4;
	else {
		for (int dir = 0; dir < 4; dir++) {
			int ny = sy + dy[dir];
			int nx = sx + dx[dir];
			if (map[ny][nx] < 0) direction = dir;
		}
	}

	clearMap(sy, sx);

	map[ey][ex] = num;
	for(int dir = 0;dir<4;dir++){
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
		map[ey + dy[direction]][ex + dx[direction]] = -1 * num;
	}
	//print();

}

int canMoveRL(int y, int x, int num) {
	int d;
	for (int dir = 0; dir < 4; dir++) {
		int ny = y + dy[dir];
		int nx = x + dx[dir];
		if (oob(ny, nx)) continue;
		if (map[ny][nx] != -1 * num)continue;
		d = dir;
	}

	//move right
	int ry = y;
	int rx = x + 1;

	if (canMove(y,x,ry, rx,num) && canMove(y,x,ry + 1, rx,num)) {
		setMap(y, x, ry + 1, rx, num, (d+1)%4);
		return 1;
	}

	int ly = y;
	int lx = x - 1;

	if (canMove(y,x,ly, lx, num) && canMove(y,x,ly + 1, lx,num)) {
		setMap(y, x, ly + 1, lx, num, (d + 1) % 4);
		return 2;
	}

	setMap(y, x, y, x, num, -1);
	return 0;
}

bool canMoveDown(int y, int x, int num) {
	int dy = y + 1;
	int dx = x;

	if (canMove(y,x,dy, dx, num)) {
		setMap(y, x, dy, dx, num, -1);
		return true;
	}
	else {
		setMap(y, x, y, x, num, -1);
	}
	return false;
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
				if (map[ny][nx] < 0) {
					if (map[ny][nx] != -1 * map[yy][xx])continue;
				}
				q.push({ ny,nx });
				v[ny][nx] = true;
				maxDepth = max(maxDepth, ny);
			}
			else if(map[yy][xx] < 0){
				q.push({ ny,nx });
				v[ny][nx] = true;
				maxDepth = max(maxDepth, ny);
			}
		}
	}
	for (int i = 0; i < R + 3; i++) {
		for (int j = 0; j < C; j++) {
			cout << v[i][j] ? 1 : 0;
		}
		cout << "\n";
	}
	return maxDepth;
}


int main() {
	// 여기에 코드를 작성해주세요.

	cin >> R >> C >> K;
	int ans = 0;
	for (int i = 0; i < K; i++ ) {
		int sRow, dir;
		cin >> sRow >> dir;
		//인덱스 0 으로 맞추기위한 빼기
		//0 북 1 동 2 남 3 서
		sRow -= 1;

		int curY = 0;
		int curX = sRow;
		while (canMoveDown(curY, curX,i+1)) {
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

		print();

		if (isFull()) {

			cout << "clear\n";
			fill(&map[0][0], &map[0][0] + 80 * 80, 0);
		}
		else {
			//bfs 점수 계산
			ans+=bfs(curY, curX);
		}
	}
	cout << ans;
	return 0;
}