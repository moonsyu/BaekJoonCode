#include <iostream>
#include <algorithm>
#include <climits>
#include <vector>
#include <queue>

#define MAX_AREA 30

using namespace std;

int area[MAX_AREA][MAX_AREA];
int dist[MAX_AREA][MAX_AREA];

struct MV_INFO {
	int y, x;
	int cost;
};

struct DIJK_COMP {
	bool operator() (MV_INFO& m1, MV_INFO& m2) {
		return m1.cost > m2.cost;
	}
};

// 사방향 탐색
int dy[] = { -1, 0, 1, 0 };
int dx[] = { 0, 1, 0, -1 };


/* 입력 최적화 */
void fast_io() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
}

int main() {
	fast_io();

	int tc;
	cin >> tc;

	for (int t = 1; t <= tc; t++) {
		int n;
		cin >> n;

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				cin >> area[i][j];
				dist[i][j] = INT_MAX;
			}
		}


		priority_queue<MV_INFO, vector<MV_INFO>, DIJK_COMP> pq;
		pq.push({0, 0, 0});
		dist[0][0] = 0;

		while (!pq.empty()) {
			MV_INFO cur = pq.top();
			pq.pop();

			int y = cur.y;
			int x = cur.x;
			int cost = cur.cost;

			if (y == n - 1 && x == n - 1) {
				cout << "#" << t << " " << cost << "\n";
				break;
			}

			for (int i = 0; i < 4; i++) {
				int ny = y + dy[i];
				int nx = x + dx[i];

				// 범위 체크
				if (ny < 0 || ny >= n || nx < 0 || nx >= n) {
					continue;
				}

				// 이동 비용 계산
				int new_cost;
				if (area[y][x] > area[ny][nx]) {
					new_cost = 0;
				} else if (area[y][x] < area[ny][nx]) {
					new_cost = (area[ny][nx] - area[y][x]) * 2;
				} else {
					new_cost = 1;
				}

				if (dist[ny][nx] > cost + new_cost) {
					dist[ny][nx] = cost + new_cost;
					pq.push({ ny, nx, dist[ny][nx] });
				}
			}
		}
	}
}
