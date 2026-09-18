#include <iostream>
#include <queue>
#include <climits>
#include <algorithm>
#include <cmath>

#define MAX_SIZE 51
#define MAX_JUMP 6

using namespace std;

// 맵 사이즈
int n;

// 맵 정보 구조체
struct MAP_INFO {
    char status;
};

// 맵 정보 배열
MAP_INFO map[MAX_SIZE][MAX_SIZE];

// 좌표 정보 구조체
struct POS {
    int y, x;

    bool same(POS& other) {
        return y == other.y && x == other.x;
    }
};

// 다익스트라 정보 구조체
struct DIJK_INFO {
    POS pos;
    int jump, time;
};

// 다익스트라 정렬 구조체
struct DIJK_COMP {
    bool operator()(DIJK_INFO& a, DIJK_INFO& b) {
        if (a.time == b.time) {
            return a.jump < b.jump;
        }
        
        return a.time > b.time;
    }
};

// 방문 정보 구조체
struct VISIT_INFO {
    int jump_time[MAX_JUMP] = { INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX };
};

// 방문 정보 배열
VISIT_INFO visited[MAX_SIZE][MAX_SIZE]; 

// 이동 방향 배열
int dy[4] = { -1, 0, 1, 0 };
int dx[4] = { 0, 1, 0, -1 };


/* 입출력 최적화 */
void fast_io() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout.tie(nullptr);
}

/* 맵 정보 입력 */
void input_map() {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            cin >> map[i][j].status;
        }
    }
}

/* 유효한 좌표 여부 반환*/
bool valid_pos(POS pos) {
    return pos.y >= 1 && pos.y <= n && pos.x >= 1 && pos.x <= n;
}

/* 점프 가능 여부 반환 */
bool can_jump(POS s, POS e) {
    
    // 목적지가 미끄러운 돌이라면 false
    if (map[e.y][e.x].status == 'S') {
        return false;
    }

    // 점프 사이에 천적이 있다면 false
    for (int i = min(s.y, e.y); i <= max(s.y, e.y); i++) {
        for (int j = min(s.x, e.x); j <= max(s.x, e.x); j++) {
            if (map[i][j].status == '#') {
                return false;
            }
        }
    }

    return true;
}


/* 다익스트라 탐색 */
int dijkstra(POS start, POS end) {
    // 기본 정보 초기화
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            visited[i][j] = VISIT_INFO();
        }
    }

    priority_queue<DIJK_INFO, vector<DIJK_INFO>, DIJK_COMP> pq;

    // 시작점 초기화
    visited[start.y][start.x].jump_time[1] = 0;
    pq.push({start, 1, 0});

    // 탐색 시작
    int result = -1;
    while (!pq.empty()) {
        DIJK_INFO cur = pq.top();
        pq.pop();

        // 도착지 도착 시 시간 반환
        if (cur.pos.same(end)) {
            result = cur.time;
            break;
        }

        // 4방향 탐색
        for (int i = 0; i < 4; i++) {
            int ny = cur.pos.y + dy[i] * cur.jump;
            int nx = cur.pos.x + dx[i] * cur.jump;

            // 유효한 좌표가 아니면 continue
            if (!valid_pos({ ny, nx })) {
                continue;
            }

            // 점프 가능 여부 확인
            if (can_jump(cur.pos, { ny, nx })) {
                if (visited[ny][nx].jump_time[cur.jump] > cur.time + 1) {
                    visited[ny][nx].jump_time[cur.jump] = cur.time + 1;
                    pq.push({ {ny, nx}, cur.jump, cur.time + 1 });
                }
            }
        }

        // 점프력 증가 (최대 5)
        if (cur.jump + 1 <= 5) {
            if (visited[cur.pos.y][cur.pos.x].jump_time[cur.jump + 1] > cur.time + pow(cur.jump + 1, 2)) {
                visited[cur.pos.y][cur.pos.x].jump_time[cur.jump + 1] = cur.time + pow(cur.jump + 1, 2);
                pq.push({ cur.pos, cur.jump + 1, (int)(cur.time + pow(cur.jump + 1, 2)) });
            }
        }


        // 점프력 감소 (최소 1)
        for (int jump = cur.jump - 1; jump >= 1; jump--) {
            if (visited[cur.pos.y][cur.pos.x].jump_time[jump] > cur.time + 1) {
                visited[cur.pos.y][cur.pos.x].jump_time[jump] = cur.time + 1;
                pq.push({ cur.pos, jump, cur.time + 1 });
            }
        }
    }

    return result;
}


int main() {
    fast_io();

    cin >> n;
    input_map();

    int tc;
    cin >> tc;
    for (int t = 1; t <= tc; t++) {
        int s_y, s_x, e_y, e_x;
        cin >> s_y >> s_x >> e_y >> e_x;

        cout << dijkstra({s_y, s_x}, {e_y, e_x}) << "\n";
    }
}
