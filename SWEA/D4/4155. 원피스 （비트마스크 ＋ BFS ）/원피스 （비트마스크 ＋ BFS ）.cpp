#include <iostream>
#include <unordered_map>

#define MAX_QUE_SIZE 50000

using namespace std;

struct Pos {
	int y, x;

	bool is_same(Pos pos) {
		return this->y == pos.y && this->x == pos.x;
	}
};

int n, m;
Pos start, dist;

char map[50][50];
unordered_map<int, bool> visited[50][50];

void fast_io() {
	cin.tie(nullptr);
	ios::sync_with_stdio(false);
	cout.tie(nullptr);
}


/* 맵 정보 입력 */
void input() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cin >> map[i][j];

			// 시작위치 및 목적지 저장
			if (map[i][j] == '0') {
				start = { i, j };
			} else if (map[i][j] == '1') {
				dist = { i, j };
			}
		}
	}
}


/* 이동 가능한지 확인 */
bool can_move(int y, int x, int mask) {
	// 맵 범위 확인
	if (y < 0 || y >= n || x < 0 || x >= m) {
		return false;
	}

	// 방문 여부 확인
	if (visited[y][x][mask]) {
		return false;
	}

	// 벽인지 확인
	if (map[y][x] == '#') {
		return false;
	}

	// 키 획득 여부 확인
	if (map[y][x] >= 'A' && map[y][x] <= 'F') {
		if (!(mask & (1 << (map[y][x] - 'A')))) {
			return false;
		}
	}

	return true;
}


/* 키 획득 시 마스크 갱신 */
int update_key(int mask, char key) {
	if (key >= 'a' && key <= 'f') {
		mask |= (1 << (key - 'a'));
	}

	return mask;
}


/* BFS 탐색을 위한 큐 구조체
Pos pos : 현재 위치
int mask : 현재 획득한 키 정보
int time : 현재까지 이동한 시간
*/
struct QUE_INFO {
	Pos pos;
	int mask, time;
};

int front, rear;
QUE_INFO que[MAX_QUE_SIZE];

int dy[] = { -1, 0, 1, 0 };
int dx[] = { 0, 1, 0, -1 };

void reset() {
	front = rear = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			visited[i][j].clear();
		}
	}
}


/* BFS 탐색 */
int bfs() {
	reset();
	visited[start.y][start.x][0] = true;
	que[rear++] = { start, 0, 0 };

	while (front < rear) {
		QUE_INFO& cur = que[front++];

		// 목적지 도착 시 탐색 중지
		if (cur.pos.is_same(dist)) {
			return cur.time;
		}

		for (int i = 0; i < 4; i++) {
			int ny = cur.pos.y + dy[i];
			int nx = cur.pos.x + dx[i];

			// 이동 가능한지 확인
			if (!can_move(ny, nx, cur.mask)) {
				continue;
			}
			visited[ny][nx][cur.mask] = true;


			int new_mask = update_key(cur.mask, map[ny][nx]);
			que[rear++] = { { ny, nx }, new_mask, cur.time + 1 };
		}
	}

	return -1;
}


int main() {
	fast_io();

	int tc;
	cin >> tc;

	for (int t = 1; t <= tc; t++) {
		cin >> n >> m;

		input();
		cout << "#" << t << " " << bfs() << "\n";
	}
}