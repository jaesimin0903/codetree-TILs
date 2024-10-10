#include <iostream>
#include <iomanip>
#include <vector>
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
	return y < 1 || x < 1 || y> N || x>N;
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

void attack(int knightNum) {
	int shieldW = knights[knightNum].w;
	int shieldH = knights[knightNum].h;
	int kY = knights[knightNum].y;
	int kX = knights[knightNum].x;
	
	int trapCount = 0;
	for (int i = kY; i < kY + shieldW; i++) {
		for (int j = kX; j < kX + shieldH; j++) {
			if (trap[i][j] == 1) trapCount++;
		}
	}

	for (int i = kY; i < kY + shieldW; i++) {
		for (int j = kX; j < kX + shieldH; j++) {
			if (board[i][j] >= 1 && board[i][j] != knightNum) {
				knights[board[i][j]].k -= trapCount;
				ans += trapCount;
				if (knights[board[i][j]].k <= 0) board[i][j] = 0;
			}
		}
	}
}

void moveChain(int y, int x, int dir) {
	vector<pair<pair<int,int>,int>> boardKnightArray;
	int yy = y;
	int xx = x;
	//현재 위치 저장
	boardKnightArray.push_back({ {y,x},board[y][x] });
	while ((1 <= y + dy[dir] && y + dy[dir] <= L) && (1 <= x + dx[dir] && x + dx[dir] <= L)) {
		y += dy[dir];
		x += dx[dir];
		boardKnightArray.push_back({ {y,x},board[y][x] });
	}

	pair<pair<int,int>,int> tmp1 = boardKnightArray[0];
	boardKnightArray[0] = { {0,0},0 };
	pair<pair<int, int>, int> tmp2;
	for (int i = 1; i < boardKnightArray.size()-1; i++) {
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
	//현재 y,x 는 움직이고나서의 위치
	//y,x에 기사가 있다.
	vector<int> boardKnightArray;

	

	//벡터가 꽉차있다면 밀면 나가니까 밀수없음
	bool isFull = true;
	for (auto v : boardKnightArray) {
		if (v == 0) return false;
	}
	return true;
}

bool move(int knightNum, int dir) {
	int curMoveY = knights[knightNum].y;
	int curMoveX = knights[knightNum].x;
	int curKnightW = knights[knightNum].w;
	int curKnightH = knights[knightNum].h;

	int willMoveY = knights[knightNum].y + dy[dir];
	int willMoveX = knights[knightNum].x + dx[dir];

	bool isBoundary = false;
	for (int i = willMoveY; i < willMoveY + curKnightW; i++) {
		if (oob(willMoveY + curKnightH - 1, willMoveX + curKnightW - 1)) isBoundary = true;
		for (int j = willMoveX; j < willMoveX + curKnightH; j++) {
			if (board[i][j] != knightNum && board[i][j] >= 1 && trap[i][j] != 2) isBoundary = true;
		}
	}

	//움직이려는 곳이 다른 기사가 있다면?
	if (isBoundary) {
		//연쇄 움직임이 가능한지?
		if (isMoveChain(curMoveY, curMoveX,dir)) {
			moveChain(curMoveY, curMoveX,dir);
			//기사의 상태를 업데이트 해야하므로 리셋
			resetBoard();
		}
		else {
			return false;
		}
	}
	else {
		//움직이려는 곳이 비어있다면 보드, 정보 변경
		knights[knightNum].y = willMoveY;
		knights[knightNum].x = willMoveX;
	}

	//업데이트된 정보를 그리기
	paintBoard();
}

void print() {
	for (int i = 1; i <= L;i++ ) {
		for (int j = 1; j <= L; j++) {
			cout << setw(4) << board[i][j];
		}
		cout << "\n";
	}
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

		board[r][c] = i;
	}

	while (Q--) {
		int knightNum, dir;
		cin >> knightNum >> dir;

		//움직여라
		bool isAttack = move(knightNum, dir);
		print();
		attack(knightNum);
	}

	cout << ans;
	return 0;
}