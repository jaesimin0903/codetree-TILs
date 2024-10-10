#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <tuple>
using namespace std;

//LxL chess board
//1,1 start
//blank, 함정, 벽
//각 기사의 체력은 k
//기사의 위치는 r,c , 방패는 r-> r+w , c -> c+h

//1기사이동
//상하좌우
//움직이려는 곳에 기사가 있다면 연쇄반응
//마지막 기사가 벽에 부딪히면 기사는 움직일 수 없음

//2대결대미지
//다른 기사를 밀치게되면 밀련,ㄴ 기사는 피해를 입음
//공격한 기사가 이동한 곳에서 wxh 내부의 함정만큼 속한 기사들이 피해
//체력이 다하면 사라짐
//기사는 밀린 이후에 피해
//밀쳤더라도 함정이 없다면 피해없음

struct Knight {
	int y, x;
	int h, w;
	int k;
	int lost;
};

int L, N, Q;
int board[50][50];
int trap[50][50];
Knight knights[40];
int ans = 0;
//0 빈칸 1 함정 2 벽

int dy[4] = { -1,0,1,0 };
int dx[4] = { 0,1,0,-1 };

bool oob(int y, int x) {
	return y < 1 || x < 1 || y> L || x>L;
}

void resetBoard() {
	for (int i = 1; i <= L; i++) {
		for (int j = 1; j <= L; j++) {
			board[i][j] = 0;
		}
	}
}

void paintBoard() {
	for (int i = 1; i <= N; i++) {
		if (knights[i].k <= 0) continue;
		int y = knights[i].y;
		int x = knights[i].x;

		board[y][x] = i;
	}
}

//void attack(int knightNum) {
//	int cur = knightNum;
//	
//	for (int i = 1; i <= N; i++) {
//		if (cur == i) continue;
//		if (knights[i].k <= 0) continue;
//		int y = knights[i].y;
//		int x = knights[i].x;
//		int h = knights[i].h;
//		int w = knights[i].w;
//
//		int trapCount = 0;
//		for (int row = y; row < y + h; row++) {
//			for (int col = x; col < x + w; col++) {
//				if (trap[row][col] == 1) {
//					trapCount++;
//				}
//			}
//		}
//
//		knights[i].k -= trapCount;
//		cout << i<< " : "<<knights[i].k << " , ";
//		ans += trapCount;
//	}
//	cout << "\n";
//}

bool isPointInTheShield(int pY, int pX, int pH, int pW, int nY, int nX, int nH, int nW) {
	pair<int, int> points[4];
	pair<int, int> npoints[4];

	// 직사각형의 4개의 꼭짓점 정의
	points[0] = { pY, pX };
	points[1] = { pY, pX + pW - 1 };
	points[2] = { pY + pH - 1, pX };
	points[3] = { pY + pH - 1, pX + pW - 1 };

	npoints[0] = { nY, nX };
	npoints[1] = { nY, nX + nW - 1 };
	npoints[2] = { nY + nH - 1, nX };
	npoints[3] = { nY + nH - 1, nX + nW - 1 };

	// npoints의 각 꼭짓점이 points로 정의된 직사각형 안에 있는지 확인
	for (int i = 0; i < 4; i++) {
		int nx = npoints[i].second; // npoints의 x좌표
		int ny = npoints[i].first;  // npoints의 y좌표

		/*cout << points[0].first << " " << ny << " " << points[2].first << "\n";
		cout << points[0].second << " " << nx << " " << points[1].second << "\n";*/
		// 직사각형 안에 있는지 확인
		if (points[0].second <= nx && nx <= points[1].second && points[0].first <= ny && ny <= points[2].first) {
			return true;
		}
	}

	for (int i = 0; i < 4; i++) {
		int nx = points[i].second; // npoints의 x좌표
		int ny = points[i].first;  // npoints의 y좌표

		/*cout << points[0].first << " " << ny << " " << points[2].first << "\n";
		cout << points[0].second << " " << nx << " " << points[1].second << "\n";*/
		// 직사각형 안에 있는지 확인
		if (npoints[0].second <= nx && nx <= npoints[1].second && npoints[0].first <= ny && ny <= npoints[2].first) {
			return true;
		}
	}
	return false;
}


void moveChain(int y, int x, int dir) {
	vector<pair<pair<int, int>, int>> boardKnightArray;
	int yy = y;
	int xx = x;
	//현재 위치 저장
	boardKnightArray.push_back({ {y,x},board[y][x] });
	while ((1 <= y + dy[dir] && y + dy[dir] <= L) && (1 <= x + dx[dir] && x + dx[dir] <= L)) {
		y += dy[dir];
		x += dx[dir];
		boardKnightArray.push_back({ {y,x},board[y][x] });
	}

	pair<pair<int, int>, int> tmp1 = boardKnightArray[0];
	boardKnightArray[0] = { {0,0},0 };
	pair<pair<int, int>, int> tmp2;
	for (int i = 1; i < boardKnightArray.size() - 1; i++) {
		tmp2 = boardKnightArray[i];
		boardKnightArray[i] = { {tmp1.first.first + dy[dir], tmp1.first.second + dx[dir]},tmp1.second };
		if (tmp2.second == 0) break;
		tmp1 = tmp2;
	}

	//정보갱신
	for (auto v : boardKnightArray) {
		knights[v.second].y = v.first.first;
		knights[v.second].x = v.first.second;
	}
}

bool isMoveChain(int y, int x, int dir) {
	// 부딪히는 기사를 넣을 큐
	queue<tuple<int,int,int>> targetKnight;
	//연쇄작용 성공시 담을 벡터
	vector<tuple<int, int, int>> moveKnight;
	int curKnightNum = board[y][x];
	targetKnight.push({ curKnightNum, knights[curKnightNum].y + dy[dir], knights[curKnightNum].x + dx[dir] });
	moveKnight.push_back({ curKnightNum, knights[curKnightNum].y + dy[dir], knights[curKnightNum].x + dx[dir] });
	while (!targetKnight.empty()) {
		int cur, curY, curX;
		tie(cur, curY, curX ) = targetKnight.front();
		//cout << cur << " " << curY << " " << curX << "\n";
		targetKnight.pop();

		int curW = knights[cur].w;
		int curH = knights[cur].h;
		
	//움직일 위치에 방패에 걸리는 애가 있는지?
	
		for (int i = curY; i < curY + curH; i++) {
			for (int j = curX; j < curX + curW; j++) {
				//cout << "trap " << trap[i][j] << "\n";
				if (oob(i, j)) {
					//cout << "oob\n";
					return false;
				}
				if (trap[i][j] == 2) {
					//cout << "rock\n";
					return false;
				}
			}
		}
				//전체 기사들을 순회하면서 
		for (int n = 1; n <= N; n++) {
			if (knights[n].k <= 0) continue;
			if (cur != n) {
				int nY = knights[n].y;
				int nX = knights[n].x;
				int nH = knights[n].h;
				int nW = knights[n].w;
				//움직인 기사의 방패가 i 기사와 겹친다면
				if (isPointInTheShield(curY, curX, curH, curW, nY, nX, nH, nW)) {
					//cout << cur << " push " << n << "\n";
					targetKnight.push({ n,nY + dy[dir], nX + dx[dir] });
					moveKnight.push_back({ n,nY + dy[dir], nX + dx[dir] });
				}
			}
		}
	}
	resetBoard();
	//성공이라면 정보를 직접 이동
	for (auto val : moveKnight) {
		int num = get<0>(val);
		int y = get<1>(val);
		int x = get<2>(val);

		knights[num].y = y;
		knights[num].x = x;
	}

	for (auto val : moveKnight) {
		int num = get<0>(val);
		if (num == curKnightNum) continue;
		int y = knights[num].y;
		int x = knights[num].x;
		int h = knights[num].h;
		int w = knights[num].w;


		int trapCount = 0;
		for (int row = y; row < y + h; row++) {
			for (int col = x; col < x + w; col++) {
				if (trap[row][col] == 1) {
					trapCount++;
				}
			}
		}

		knights[num].k -= trapCount;
		knights[num].lost += trapCount;
		//cout << i << " : " << knights[i].k << " , ";
		
	}

	paintBoard();
	return true;
}

bool move(int knightNum, int dir) {
	//cout << knightNum << " " << dir << "\n";
	int curMoveY = knights[knightNum].y;
	int curMoveX = knights[knightNum].x;
	int curKnightW = knights[knightNum].w;
	int curKnightH = knights[knightNum].h;

	int willMoveY = knights[knightNum].y + dy[dir];
	int willMoveX = knights[knightNum].x + dx[dir];

	bool isBoundary = false;
	for (int i = willMoveY; i < willMoveY + curKnightH; i++) {
		for (int j = willMoveX; j < willMoveX + curKnightW; j++) {
			if (oob(i,j)) return false;
			if (trap[i][j] == 2)return false;
			//if (board[i][j] != knightNum && board[i][j] >= 1 && trap[i][j] != 2) isBoundary = true;
		}
	}

	for (int n = 1; n <= N; n++) {
		if (knights[n].k <= 0) continue;
		if (knightNum != n) {
			int nY = knights[n].y;
			int nX = knights[n].x;
			int nH = knights[n].h;
			int nW = knights[n].w;
			//움직인 기사의 방패가 i 기사와 겹친다면
			if (isPointInTheShield(willMoveY, willMoveX, curKnightH, curKnightW, nY, nX, nH, nW)) {
				//cout << "boundary\n";
				isBoundary = true;
			}
			else {
				//cout << "no\n";
			}
		}
	}

	//움직이려는 곳이 다른 기사가 있다면?
	if (isBoundary) {
		//연쇄 움직임이 가능한지?
		if (isMoveChain(curMoveY, curMoveX, dir)) {
			//cout << "chain action\n";
			return true;
		}
		else {
			//cout << "chain fail\n";
			return false;
		}
	}
	else {
		//움직이려는 곳이 비어있다면 보드, 정보 변경
		//cout << "no chain\n";
		resetBoard();
		knights[knightNum].y = willMoveY;
		knights[knightNum].x = willMoveX;
		paintBoard();
		return false;
	}
	
}

void print() {
	for (int i = 1; i <= L; i++) {
		for (int j = 1; j <= L; j++) {
			cout << setw(4) << board[i][j];
		}
		cout << "\n";
	}
	for (int i = 1; i <= N; i++) {
		cout << i << " : " << knights[i].lost << " , ";
	}
	cout << "\n";
	cout << "\n";

}

int main() {
	cin >> L >> N >> Q;
	for (int i = 1; i <= L; i++) {
		for (int j = 1; j <= L; j++) {
			cin >> trap[i][j];
			board[i][j] = 0;
		}
	}

	for (int i = 1; i <= N; i++) {
		int r, c, h, w, k;
		cin >> r >> c >> h >> w >> k;
		knights[i].y = r;
		knights[i].x = c;
		knights[i].h = h;
		knights[i].w = w;
		knights[i].k = k;
		knights[i].lost = 0;
		board[r][c] = i;
	}


	//print();
	while (Q--) {
		int knightNum, dir;
		cin >> knightNum >> dir;

		//움직여라
		bool isAttack = move(knightNum, dir);

		//print();
		
	}
	for (int i = 1; i <= N; i++) {
		if (knights[i].k <= 0)continue;
		ans += knights[i].lost;
	}
	cout << ans;
	return 0;
}