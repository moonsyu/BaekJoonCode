#include <string>
#include <vector>
#include <queue>
#include <cstring>
#include <cstdlib>

#define MAX_QUE_SIZE 1600
#define MAX_PANEL 16 // 0번: 엘리베이터, 1 ~ 15번: 패널
#define MAX_MASK (1 << 15)

using namespace std;

struct Pos {
	int y, x;
};

int n, m; // 격자 크기
int panel_cnt; // 패널 개수

Pos panel_pos[MAX_PANEL]; // 패널 위치
int panel_floor[MAX_PANEL]; // 패널 층

// panel_dist[0][i] : 엘리베이터 ~ i번 패널 거리, panel_dist[i][j] : 같은 층 i번 ~ j번 패널 거리
int panel_dist[MAX_PANEL][MAX_PANEL];

// floor_panels[f] : f층에 있는 패널 번호
vector<vector<int>> floor_panels;

// pre_mask[b] : b번 패널 활성화 전에 활성화해야 하는 패널의 비트마스크
int pre_mask[MAX_PANEL];


/* BFS 탐색을 위한 변수 */
int front, rear;
Pos que[MAX_QUE_SIZE];
int grid_dist[40][40]; // 시작 칸 ~ 각 칸 거리 (-1 : 미방문)

int dy[] = { -1, 0, 1, 0 };
int dx[] = { 0, 1, 0, -1 };


/* 이동 가능한지 확인 */
bool can_move(vector<string>& grid, int y, int x) {
	// 맵 범위 확인
	if (y < 0 || y >= n || x < 0 || x >= m) {
		return false;
	}

	// 방문 여부 확인
	if (grid_dist[y][x] != -1) {
		return false;
	}

	// 폐쇄 구역인지 확인
	if (grid[y][x] == '#') {
		return false;
	}

	return true;
}


/* start에서 모든 칸까지의 거리 계산 */
void bfs(vector<string>& grid, Pos start) {
	memset(grid_dist, -1, sizeof(grid_dist));
	front = rear = 0;

	grid_dist[start.y][start.x] = 0;
	que[rear++] = start;

	while (front < rear) {
		Pos cur = que[front++];

		for (int i = 0; i < 4; i++) {
			int ny = cur.y + dy[i];
			int nx = cur.x + dx[i];

			if (!can_move(grid, ny, nx)) {
				continue;
			}

			grid_dist[ny][nx] = grid_dist[cur.y][cur.x] + 1;
			que[rear++] = { ny, nx };
		}
	}
}


/* 1. 엘리베이터 ~ 각 패널 거리 계산 */
void set_elevator_dist(vector<string>& grid) {
	Pos elevator = { 0, 0 };

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (grid[i][j] == '@') {
				elevator = { i, j };
			}
		}
	}

	bfs(grid, elevator);

	for (int i = 1; i <= panel_cnt; i++) {
		panel_dist[0][i] = grid_dist[panel_pos[i].y][panel_pos[i].x];
		panel_dist[i][0] = panel_dist[0][i];
	}
}


/* 3. 패널 정보를 층 별로 저장 */
void set_floor_panels(int h, vector<vector<int>>& panels) {
	floor_panels.assign(h + 1, vector<int>());

	for (int i = 1; i <= panel_cnt; i++) {
		panel_floor[i] = panels[i - 1][0];
		panel_pos[i] = { panels[i - 1][1] - 1, panels[i - 1][2] - 1 };

		floor_panels[panel_floor[i]].push_back(i);
	}
}


/* 4. 같은 층에 있는 패널 간 거리 계산 */
void set_same_floor_dist(vector<string>& grid) {
	for (int f = 1; f < floor_panels.size(); f++) {
		// 패널이 2개 이상인 층만 확인
		if (floor_panels[f].size() < 2) {
			continue;
		}

		for (int i = 0; i < floor_panels[f].size(); i++) {
			int from = floor_panels[f][i];
			bfs(grid, panel_pos[from]);

			for (int j = 0; j < floor_panels[f].size(); j++) {
				int to = floor_panels[f][j];
				panel_dist[from][to] = grid_dist[panel_pos[to].y][panel_pos[to].x];
			}
		}
	}
}


/* 5. seqs를 돌며 선행 패널 비트 설정 */
void set_pre_mask(vector<vector<int>>& seqs) {
	for (int i = 0; i < seqs.size(); i++) {
		int a = seqs[i][0];
		int b = seqs[i][1];

		pre_mask[b] |= (1 << (a - 1));
	}
}


/* from번 ~ to번 패널 이동 시간 */
int get_time(int from, int to) {
	// 같은 층
	if (panel_floor[from] == panel_floor[to]) {
		return panel_dist[from][to];
	}

	// 다른 층은 엘리베이터를 거쳐야 함
	return panel_dist[from][0] + abs(panel_floor[from] - panel_floor[to]) + panel_dist[0][to];
}


/* 다익스트라 탐색을 위한 구조체
int time : 현재까지 걸린 시간
int pos : 현재 패널 번호
int mask : 패널 활성화 정보 (활성화 시 0, 비활성화 시 1)
int can_visit[] : 각 패널의 남은 선행 패널 비트마스크 (0이면 방문 가능)
*/
struct PQ_INFO {
	int time, pos, mask;
	int can_visit[MAX_PANEL];

	bool operator<(const PQ_INFO& other) const {
		return time > other.time; // 거리가 짧은 것 먼저
	}
};

int best_time[MAX_MASK][MAX_PANEL]; // 같은 상태를 더 느리게 다시 탐색하지 않기 위한 배열


/* 6. 1번 패널부터 다익스트라 */
int dijkstra() {
	memset(best_time, 0x3f, sizeof(best_time));
	priority_queue<PQ_INFO> pq;

	PQ_INFO start;
	start.time = 0;
	start.pos = 1;
	start.mask = (1 << panel_cnt) - 1; // 시작 시 모든 패널 비활성화
	for (int i = 0; i < MAX_PANEL; i++) {
		start.can_visit[i] = pre_mask[i];
	}

	best_time[start.mask][start.pos] = 0;
	pq.push(start);

	while (!pq.empty()) {
		PQ_INFO cur = pq.top();
		pq.pop();

		// 모든 패널 활성화 시 현재 거리 반환
		if (cur.mask == 0) {
			return cur.time;
		}

		// 이미 더 빠른 시간으로 탐색한 상태
		if (cur.time > best_time[cur.mask][cur.pos]) {
			continue;
		}

		for (int i = 1; i <= panel_cnt; i++) {
			int bit = (1 << (i - 1));

			// 이미 활성화한 패널
			if (!(cur.mask & bit)) {
				continue;
			}

			// 선행 패널이 남아 있는 패널
			if (cur.can_visit[i] != 0) {
				continue;
			}

			PQ_INFO next = cur;
			next.time = cur.time + get_time(cur.pos, i);
			next.pos = i;
			next.mask = cur.mask & ~bit;

			if (next.time >= best_time[next.mask][next.pos]) {
				continue;
			}
			best_time[next.mask][next.pos] = next.time;

			// i번 패널을 선행 목록에서 제거
			for (int j = 1; j <= panel_cnt; j++) {
				next.can_visit[j] &= ~bit;
			}

			pq.push(next);
		}
	}

	return -1;
}


int solution(int h, vector<string> grid, vector<vector<int>> panels, vector<vector<int>> seqs) {
	int answer = 0;

	n = grid.size();
	m = grid[0].size();
	panel_cnt = panels.size();

	memset(panel_dist, 0, sizeof(panel_dist));
	memset(pre_mask, 0, sizeof(pre_mask));

	set_floor_panels(h, panels);
	set_elevator_dist(grid);
	set_same_floor_dist(grid);
	set_pre_mask(seqs);

	answer = dijkstra();

	return answer;
}
