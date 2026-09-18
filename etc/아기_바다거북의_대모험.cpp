#include <iostream>
#include <unordered_map>

#define MAX_GRID 20
#define MAX_TURTLE 12
#define MAX_MOUNTAIN 12
#define MAGMA 10

using namespace std;

// 좌표 구조체
struct POS {
    int y, x;
};

// 격자 정보 구조체
struct GRID_INFO {
    int is_turtle;
    int is_mountain;
    bool wall;
};

// 거북이 정보 구조체
struct TURTLE_INFO {
    POS pos;
    int dir = -1;
    bool alive = true;
    int time = -1;
};

// 화산 정보 구조체
struct MOUNTAIN_INFO {
    POS pos;
    int now, max;
};

// 큐 정보 구조체
struct QUE_INFO {
    POS pos;
};

int n, m, k;
GRID_INFO grid[MAX_GRID][MAX_GRID];

int visited[MAX_GRID][MAX_GRID];
int visit_turn = 0;

int turtle_idx = 1;
TURTLE_INFO turtle[MAX_TURTLE];

int mountain_idx = 1;
MOUNTAIN_INFO mountain[MAX_MOUNTAIN];

int front, rear;
QUE_INFO que[MAX_GRID * MAX_GRID * MAX_GRID];

// 거북이와 화산 위치에 누적된 열기 값
unordered_map<int, int> magma_log;
unordered_map<int, bool> boom_mountain;

// 방향 배열 (상, 좌, 하, 우)
int dy[] = { -1, 0, 1, 0 };
int dx[] = { 0, -1, 0, 1 };

/* 입출력 최적화 */
void fast_io() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
}

/* 큐 초기화 */
void reset() {
    front = rear = 0;
}

/* 이동 가능 여부 반환 */
bool can_move(int y, int x) {
    if (y < 0 || y >= n || x < 0 || x >= n) return false;
    if (grid[y][x].wall) return false;

    return true;
}

/* 거북이 한 마리 이동 */
void turtle_move(int turtle_id, int turn) {
    TURTLE_INFO& cur_turtle = turtle[turtle_id];

    if (!cur_turtle.alive || cur_turtle.time != -1) {
        return;
    }

    reset();
    visit_turn++;
    cur_turtle.dir = -1;

    que[rear++] = { {n - 1, n - 1} };
    visited[n - 1][n - 1] = visit_turn;

    while (front < rear) {
        int size = rear;

        // 같은 거리의 모든 칸을 확인
        while (front < size) {
            POS cur = que[front++].pos;

            for (int dir = 0; dir < 4; dir++) {
                int ny = cur.y + dy[dir];
                int nx = cur.x + dx[dir];

                if (!can_move(ny, nx) || visited[ny][nx] == visit_turn) {
                    continue;
                }

                if (grid[ny][nx].is_turtle) {
                    // dir은 안식처 -> 거북이 방향
                    // 거북이는 반대 방향으로 이동
                    if (grid[ny][nx].is_turtle == turtle_id) {
                        cur_turtle.dir = max(cur_turtle.dir, (dir + 2) % 4);
                    }
                    continue;
                }

                visited[ny][nx] = visit_turn;
                que[rear++] = { {ny, nx} };
            }
        }

        // 최초로 거북이를 찾은 거리에서 종료
        if (cur_turtle.dir != -1) {
            break;
        }
    }

    // 안식처까지 갈 수 없는 경우 이동하지 않음
    if (cur_turtle.dir == -1) {
        return;
    }

    grid[cur_turtle.pos.y][cur_turtle.pos.x].is_turtle = 0;

    cur_turtle.pos.y += dy[cur_turtle.dir];
    cur_turtle.pos.x += dx[cur_turtle.dir];
    cur_turtle.dir = -1;

    // 안식처 도착 시 지도에서 제거
    if (cur_turtle.pos.y == n - 1 && cur_turtle.pos.x == n - 1) {
        cur_turtle.time = turn;
        return;
    }

    grid[cur_turtle.pos.y][cur_turtle.pos.x].is_turtle = turtle_id;
}

/* 화산 분출 */
void magma(int mountain_id);

/* 마그마가 도달한 거북이, 화산의 열기 기록 */
void magma_record(int y, int x, int magma_power) {
    if (grid[y][x].is_turtle) {
        int turtle_id = grid[y][x].is_turtle;
        magma_log[turtle_id] += magma_power;
    }

    if (grid[y][x].is_mountain) {
        int mountain_id = grid[y][x].is_mountain;

        magma_log[mountain_id * MAX_TURTLE] += magma_power;

        int now_power = magma_log[mountain_id * MAX_TURTLE]
            + mountain[mountain_id].now;

        if (!boom_mountain[mountain_id] &&
            now_power >= mountain[mountain_id].max) {
            magma(mountain_id);
        }
    }
}

void magma(int mountain_id) {
    MOUNTAIN_INFO& cur = mountain[mountain_id];

    if (boom_mountain[mountain_id]) {
        return;
    }
    boom_mountain[mountain_id] = true;

    // 화산이 있는 칸에도 최대 열기가 발생
    magma_record(cur.pos.y, cur.pos.x, cur.max);

    for (int dir = 0; dir < 4; dir++) {
        int magma_power = cur.max;

        for (int len = 1; len < n; len++) {
            magma_power /= 2;

            if (magma_power == 0) {
                break;
            }

            int ny = cur.pos.y + dy[dir] * len;
            int nx = cur.pos.x + dx[dir] * len;

            if (!can_move(ny, nx)) {
                break;
            }

            magma_record(ny, nx, magma_power);
        }
    }
}

/* 화산 분출 확인 */
void magma_check() {
    magma_log.clear();
    boom_mountain.clear();

    // 모든 화산의 압력을 먼저 증가
    for (int i = 1; i < mountain_idx; i++) {
        mountain[i].now += MAGMA;
    }

    // 분출 및 연쇄 반응
    for (int i = 1; i < mountain_idx; i++) {
        if (!boom_mountain[i] &&
            mountain[i].now >= mountain[i].max) {
            magma(i);
        }
    }

    // 모든 분출이 끝난 뒤 화석화
    for (int i = 1; i < turtle_idx; i++) {
        TURTLE_INFO& cur = turtle[i];

        if (cur.alive && magma_log[i] >= 20) {
            cur.alive = false;
        }
    }

    // 이번 턴에 분출한 화산 압력 초기화
    for (int i = 1; i < mountain_idx; i++) {
        if (boom_mountain[i]) {
            mountain[i].now = 0;
        }
    }
}

/* 계산 함수 */
void calc() {
    for (int turn = 1; turn <= 100; turn++) {
        for (int i = 1; i < turtle_idx; i++) {
            turtle_move(i, turn);
        }

        magma_check();
    }
}

int main() {
    fast_io();

    cin >> n >> m >> k;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int temp;
            cin >> temp;

            grid[i][j].wall = temp ? true : false;
        }
    }

    int t_y, t_x;
    for (int i = 1; i <= m; i++) {
        cin >> t_y >> t_x;

        turtle[turtle_idx++].pos = { t_y, t_x };
        grid[t_y][t_x].is_turtle = i;
    }

    int m_y, m_x, max_p;
    for (int i = 1; i <= k; i++) {
        cin >> m_y >> m_x >> max_p;

        mountain[mountain_idx++] = { {m_y, m_x}, 0, max_p };
        grid[m_y][m_x].is_mountain = i;
    }

    calc();

    for (int i = 1; i <= m; i++) {
        cout << turtle[i].time << "\n";
    }
}
