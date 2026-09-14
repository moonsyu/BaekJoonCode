#include <iostream>
#include <queue>
#include <climits>

#define MAX_SIZE 50

using namespace std;

// 맵 정보 구조체
struct MAP_INFO {
	bool is_ground;
	int limit;
};

int n, m; // 맵 크기
MAP_INFO map[MAX_SIZE][MAX_SIZE]; // 맵 정보

// 좌표 정보 구조체
struct Pos {
	int y, x;

	bool same(Pos& other) {
		return y == other.y && x == other.x;
	}
};

Pos now; // 스노우맨 위치
Pos dest; // 목표 위치

// 다익스트라 정보 구조체
struct DIJK_INFO {
	Pos pos;
	int max_limit;
	int now_limit;
};

// 다익스트라 정렬 구조체
struct DIJK_COMP {
	bool operator()(DIJK_INFO& a, DIJK_INFO& b) {
		return a.max_limit > b.max_limit;
	}
};

// 다익스트라 우선순위 큐
priority_queue<DIJK_INFO, vector<DIJK_INFO>, DIJK_COMP> pq;

// 방향 배열
int dy[4] = { -1, 0, 1, 0 };
int dx[4] = { 0, 1, 0, -1 };


/* 입출력 최적화 */
void fast_io() {
	cin.tie(nullptr);
	ios::sync_with_stdio(false);
	cout.tie(nullptr);
}


/* 초기화 */
void init() {
	for (int i = 0; i < MAX_SIZE; i++) {
		for (int j = 0; j < MAX_SIZE; j++) {
			map[i][j].is_ground = false;
			map[i][j].limit = INT_MAX;
		}
	}

}

/* 맵 입력 */
void input_map() {
	int input;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {

			cin >> input;
			if (input == 1) {
				map[i][j].is_ground = true;
			}
			else if (input == 2) {
				map[i][j].is_ground = true;
				now = { i, j };
			}
			else if (input == 3) {
				map[i][j].is_ground = true;
				dest = { i, j };
			}
		}
	}
}


/* 유효한 좌표 여부 반환 */
bool valid_check(int y, int x) {
	if (y < 0 || y >= n || x < 0 || x >= m) {
		return false;
	}

	return true;
}


/* 다익스트라 */
int dijkstra() {

	pq.push({ now, 0 });
	map[now.y][now.x].limit = 0;

	while (!pq.empty()) {
		DIJK_INFO cur = pq.top();
		pq.pop();

		int now_limit = map[cur.pos.y][cur.pos.x].is_ground ? 0 : cur.now_limit;
		int max_limit = cur.max_limit;

		// 목적지 도착 시 limit 반환
		if (cur.pos.same(dest)) {
			return max_limit;
		}

		// 4방향 탐색
		for (int i = 0; i < 4; i++) {
			int ny = cur.pos.y + dy[i];
			int nx = cur.pos.x + dx[i];

			// 유효한 좌표인지 확인
			if (valid_check(ny, nx)) {
				// 좌우 이동 시
				if (i % 2) {
					// 현재 땅 위에 있는지 확인
					if (map[cur.pos.y][cur.pos.x].is_ground && map[ny][nx].is_ground) {
						if (map[ny][nx].limit > max_limit) {
							map[ny][nx].limit = max_limit;
							pq.push({ { ny, nx }, map[ny][nx].limit, now_limit });
						}
					}
				} else { // 상하 이동 시
					if (map[ny][nx].limit > max(max_limit, now_limit + 1)) {
						map[ny][nx].limit = max(max_limit, now_limit + 1);
						pq.push({ { ny, nx }, map[ny][nx].limit, now_limit + 1});
					}
				}
			}
		}
	}
}


int main() {
	fast_io(); // 입출력 최적화

	init(); // 초기화

	cin >> n >> m;
	input_map(); // 맵 입력

	cout << dijkstra() << "\n"; // 다익스트라 실행 및 결과 출력
}
