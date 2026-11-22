#include <string>
#include <vector>
#include <climits>

#define MAX_N 100
#define MAX_QUE_SIZE (MAX_N * MAX_N)

using namespace std;

struct Pos {
	int y, x;
};

int n, m; // 격자 크기
vector<string> map; // 격자 정보

// visit[y][x] : (y, x)에 멈춘 시간 (미방문 INT_MAX)
int visit[MAX_N][MAX_N];

Pos que[MAX_QUE_SIZE];
int front, rear;

int dy[] = { -1, 0, 1, 0 };
int dx[] = { 0, 1, 0, -1 };


/* 로봇 위치 반환 */
Pos get_robot_pos() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (map[i][j] == 'R') {
				return { i, j };
			}
		}
	}

	return { 0, 0 };
}


/* 이동 가능한지 확인 */
bool can_move(int y, int x) {
	// 맵 범위 확인
	if (y < 0 || y >= n || x < 0 || x >= m) {
		return false;
	}

	// 장애물인지 확인
	if (map[y][x] == 'D') {
		return false;
	}

	return true;
}


/* 현재 위치에서 dir 방향 끝까지 이동한 위치 반환 */
Pos get_end_pos(Pos cur, int dir) {
	int y = cur.y;
	int x = cur.x;

	// 다음 칸이 범위 밖이거나 장애물이면 멈춤
	while (can_move(y + dy[dir], x + dx[dir])) {
		y += dy[dir];
		x += dx[dir];
	}

	return { y, x };
}


/* 로봇 위치부터 BFS 탐색 */
int bfs(Pos start) {
	// 초기화
	front = rear = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			visit[i][j] = INT_MAX;
		}
	}

	// 방문 표시
	visit[start.y][start.x] = 0;
	que[rear++] = start;

	// BFS 탐색
	while (front < rear) {
		Pos cur = que[front++];
		int time = visit[cur.y][cur.x];

		// 현재 위치 기준 4방향 탐색
		for (int i = 0; i < 4; i++) {
			Pos end = get_end_pos(cur, i);

			// 이미 현재 시간 + 1 이하로 방문한 좌표
			if (visit[end.y][end.x] <= time + 1) {
				continue;
			}

			visit[end.y][end.x] = time + 1;

			// G에 도착
			if (map[end.y][end.x] == 'G') {
				return visit[end.y][end.x];
			}

			que[rear++] = end;
		}
	}

	// G에 도달 불가
	return -1;
}


int solution(vector<string> board) {
	map = board;
	n = map.size();
	m = map[0].size();

	return bfs(get_robot_pos());
}