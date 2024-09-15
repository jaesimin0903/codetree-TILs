#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include<queue>
using namespace std;

int R, C, K;
int map[80][80];

struct Golem {
	int y, x;
	int num;
	int dir;
};

int dy[4] = { -1,0,1,0 };
int dx[4] = { 0,1,0,-1 };

bool oob(int y, int x) {
	return y < 0 || x < 0 || y >= R + 3 || x >= C;
}

void print() {
	for (int i = 0; i < R + 3; i++) {
		for (int j = 0; j < C; j++) {
			cout << map[i][j] << "  ";
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

bool canMove(Golem &golem, int ey, int ex) {
	//cout << "run canMove\n";
	bool canMove = true;

	//초기위치 초기화
	clearMap(golem.y, golem.x);

	//print();
	//다음 움직일 곳으로 갈수 있는지?
	for (int d = 0; d < 4; d++) {
		int ny = ey + dy[d];
		int nx = ex + dx[d];
		//cout << ny << " " << nx << "\n";
		if (map[ny][nx] != 0 || oob(ny, nx)) {
			int sy = golem.y; int sx = golem.x;
			map[sy][sx] = golem.num;
			for (int dd = 0; dd < 4; dd++) {
				int nny = sy + dy[dd];
				int nnx = sx + dx[dd];
				map[nny][nnx] = golem.num;
			}
			map[sy + dy[golem.dir]][sx + dx[golem.dir]] = golem.num * -1;
			return false;
		}
	}

	// 움직일 수 있따면
	int sy = golem.y; int sx = golem.x;
	map[sy][sx] = golem.num;
	for (int dd = 0; dd < 4; dd++) {
		int nny = sy + dy[dd];
		int nnx = sx + dx[dd];
		map[nny][nnx] = golem.num;
	}
	map[sy + dy[golem.dir]][sx + dx[golem.dir]] = golem.num * -1;
	return true;
}

void setMap(Golem & golem,int lr) {
	//cout << "setMap " << ey << ex << num<<"\n";
	clearMap(golem.y, golem.x);
	
	if (lr == 1) {//right
		golem.dir = (golem.dir + 1) % 4;
		golem.x += 1;
		golem.y += 1;
	}
	else if (lr == 2) { golem.dir = (golem.dir + 3) % 4; 
	golem.x -= 1;
	golem.y += 1;
	}
	else if(lr == 0) {
		golem.y += 1;
	}
	else if (lr == -1) {

	}


	int ey = golem.y;
	int ex = golem.x;

	//움직이고자 하는 위치에 맵 저장
	map[ey][ex] = golem.num;
	for (int dir = 0; dir < 4; dir++) {
		int ny = ey + dy[dir];
		int nx = ex + dx[dir];
		map[ny][nx] = golem.num;
	}
	map[ey + dy[golem.dir]][ex + dx[golem.dir]] = golem.num * -1;

}
bool canMoveDown(Golem &golem) {
	int dy = golem.y + 1;
	int dx = golem.x;

	if (canMove(golem,  dy, dx)) {
		setMap(golem, 0);
		return true;
	}

	return false;
}

bool canMoveRL(Golem &golem) {


	//현재 위치에서 오른쪽 왼쪽 갈수있는지
	
	int y = golem.y;
	int x = golem.x;


	int ly = y;
	int lx = x - 1;
	while (canMoveDown(golem)) {}

	if (canMove(golem, ly, lx) && canMove(golem, ly + 1, lx)) {
		setMap(golem, 2);
		while (canMoveDown(golem)) {}
		//cout << "move left\n";
		return true;
	}

	setMap(golem,-1);

	//move right
	int ry = y;
	int rx = x + 1;

	if (canMove(golem, ry, rx) && canMove(golem, ry + 1, rx)) {
		setMap(golem, 1);
		while (canMoveDown(golem)) {}
		//cout << "move right\n";
		return true;
	}
	setMap(golem,-1);
	return false;
}


bool isFull() {
	for (int i = 0; i < C; i++) {
		if (map[2][i] != 0) {
			return true;
		}
	}
	return false;
}

int bfs(Golem &golem) {
	queue<pair<int, int>> q;
	int maxDepth = 0;
	q.push({ golem.y, golem.x });
	bool v[80][80];
	fill(&v[0][0], &v[0][0] + 80 * 80, false);
	v[golem.y][golem.x] = true;

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

		Golem golem;
		golem.num = i + 1;
		golem.y = curY;
		golem.x = curX;
		golem.dir = dir;

		while (canMoveDown(golem)) {
			
		}


		while(canMoveRL(golem)){ }

		/*print();
		cout << "\n";*/


		if (isFull()) {

			//cout << "clear\n";
			fill(&map[0][0], &map[0][0] + 80 * 80, 0);
		}
		else {
			//bfs 점수 계산
			ans += bfs(golem);
		}
	}
	cout << ans;
	return 0;
}