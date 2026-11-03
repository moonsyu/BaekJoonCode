#include <string>
#include <vector>
#include <queue>
#include <cstring>
#include <cstdlib>

#define MAX_QUE_SIZE 1600
#define MAX_PANEL 16 // 0번: 엘리베이터, 1 ~ 15번: 패널
#define MAX_MASK (1 << 16)

using namespace std;

struct Pos {
	int y, x;
};

struct PANEL_INFO {
	int y, x;
	int floor;
};

int n, m; // 격자 크기
int panel_cnt; // 패널 개수
vector<string> map; // 격자 정보

PANEL_INFO panel_info[MAX_PANEL]; // 패널 정보

// 패널 간 거리 (0번: 엘리베이터, 1 ~ 15번: 패널)
int panel_dist[MAX_PANEL][MAX_PANEL];

// floor_panels[h] : h층에 있는 패널 번호
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
bool can_move(int y, int x) {
	// 맵 범위 확인
	if (y < 0 || y >= n || x < 0 || x >= m) {
		return false;
	}

	// 방문 여부 확인
	if (grid_dist[y][x] != -1) {
		return false;
	}

	// 폐쇄 구역인지 확인
	if (map[y][x] == '#') {
		return false;
	}

	return true;
}


/* start에서 모든 칸까지의 거리 계산 */
void bfs(Pos start) {
	// 초기화
	front = rear = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			grid_dist[i][j] = -1;
		}
	}

	// 방문 표시
	grid_dist[start.y][start.x] = 0;
	que[rear++] = start;

	// BFS 탐색
	while (front < rear) {
		Pos& cur = que[front++];

		for (int i = 0; i < 4; i++) {
			int ny = cur.y + dy[i];
			int nx = cur.x + dx[i];

			if (!can_move(ny, nx)) {
				continue;
			}

			grid_dist[ny][nx] = grid_dist[cur.y][cur.x] + 1;
			que[rear++] = { ny, nx };
		}
	}
}


/* 엘리베이터 위치 반환 */
Pos get_elevator_pos() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (map[i][j] == '@') {
				return { i, j };
			}
		}
	}

	return { 0, 0 }; // 엘리베이터는 항상 존재하므로 도달하지 않음
}


/* 엘리베이터 ~ 각 패널 거리 계산 */
void set_elevator_dist() {
	bfs(get_elevator_pos());

	// 엘리베이터 ~ i번 패널까지의 거리 저장
	for (int i = 1; i <= panel_cnt; i++) {
		panel_dist[0][i] = grid_dist[panel_info[i].y][panel_info[i].x];
		panel_dist[i][0] = panel_dist[0][i];
	}
}


/* 패널 정보를 층 별로 저장 */
void set_floor_panels(int h, vector<vector<int>>& panels) {
	// vector 초기화
	floor_panels.assign(h + 1, vector<int>());

	// 패널 정보 전처리
	for (int i = 1; i <= panel_cnt; i++) {
		// 패널 정보: y, x, floor
		panel_info[i] = { panels[i - 1][1] - 1, panels[i - 1][2] - 1, panels[i - 1][0] };

		// 층 별 패널 번호 저장
		floor_panels[panel_info[i].floor].push_back(i);
	}
}


/* 같은 층에 있는 패널 간 거리 계산 */
void set_same_floor_dist() {
	// h: 층 
	for (int h = 1; h < floor_panels.size(); h++) {
		// 패널이 2개 이상인 층만 확인
		if (floor_panels[h].size() < 2) {
			continue;
		}

		// 같은 층에 있는 패널 간 거리 계산
		for (int i = 0; i < floor_panels[h].size(); i++) {
			int from = floor_panels[h][i];
			bfs({ panel_info[from].y, panel_info[from].x });

			// from -> to 거리 저장
			for (int j = 0; j < floor_panels[h].size(); j++) {
				int to = floor_panels[h][j];
				panel_dist[from][to] = grid_dist[panel_info[to].y][panel_info[to].x];
			}
		}
	}
}


/* seqs를 돌며 선행 패널 비트 설정 */
void set_pre_mask(vector<vector<int>>& seqs) {
	for (int i = 0; i < seqs.size(); i++) {
		int a = seqs[i][0];
		int b = seqs[i][1];

		pre_mask[b] |= (1 << a);
	}
}


/* from번 ~ to번 패널 이동 시간 */
int get_time(int from, int to) {
	// 같은 층
	if (panel_info[from].floor == panel_info[to].floor) {
		return panel_dist[from][to];
	}

	// 다른 층은 엘리베이터를 거쳐야 함
	int floor_move_time = abs(panel_info[from].floor - panel_info[to].floor);
	return panel_dist[from][0] + floor_move_time + panel_dist[0][to];
}


/* 다익스트라 탐색을 위한 구조체
int time : 현재까지 걸린 시간
int pos : 현재 패널 번호
int mask : 패널 활성화 정보 (활성화 시 0, 비활성화 시 1)
*/
struct PQ_INFO {
	int time, pos, mask;

	bool operator<(const PQ_INFO& other) const {
		return time > other.time; // 거리가 짧은 것 먼저
	}
};

int best_time[MAX_MASK][MAX_PANEL]; // 같은 상태를 더 느리게 다시 탐색하지 않기 위한 배열


/* 탐색 시작 */
int dijkstra() {
	memset(best_time, 0x3f, sizeof(best_time));
	priority_queue<PQ_INFO> pq;

	// 1번 패널 위치에서 출발, 모든 패널 비활성화 상태
	PQ_INFO start = { 0, 1, (1 << (panel_cnt + 1)) - 2 };

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
			int bit = (1 << i);

			// 이미 활성화한 패널
			if (!(cur.mask & bit)) {
				continue;
			}

			// 선행 패널 중 아직 비활성화된 패널이 있음
			if (pre_mask[i] & cur.mask) {
				continue;
			}

			// 다음 상태 계산
			PQ_INFO next = { cur.time + get_time(cur.pos, i), i, cur.mask & ~bit };
			if (next.time >= best_time[next.mask][next.pos]) {
				continue;
			}

			best_time[next.mask][next.pos] = next.time;
			pq.push(next);
		}
	}

	return -1;
}


int solution(int h, vector<string> grid, vector<vector<int>> panels, vector<vector<int>> seqs) {
	n = grid.size();
	m = grid[0].size();
	panel_cnt = panels.size();
	map = grid;

	memset(panel_dist, 0, sizeof(panel_dist));
	memset(pre_mask, 0, sizeof(pre_mask));

	set_floor_panels(h, panels);
	set_elevator_dist();
	set_same_floor_dist();
	set_pre_mask(seqs);

	return dijkstra();
}