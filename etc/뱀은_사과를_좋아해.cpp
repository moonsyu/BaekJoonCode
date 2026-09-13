#include <iostream>

#define MAX_SIZE 100
#define MAX_S 10000

using namespace std;

int N, M, K; // 격자 크기, 사과 개수, 방향 전환 횟수

// 격자 정보 구조체
struct GRID_INFO {
    bool is_apple;
    bool is_snake;
	bool is_tail;
    int dir;
};

int y, x; // 좌표
int answer; // 게임 진행 시간
GRID_INFO grid[MAX_SIZE][MAX_SIZE]; // 격자 정보

// 상하좌우 이동 좌표
int dy[] = { -1, 0, 1, 0 };
int dx[] = { 0, 1, 0, -1 };


/* 초기화 */
void init() {
    answer = 0;
    y = x = 0;

    // 뱀의 시작 위치
    grid[y][x] = {false, true, true, 0};
}


/* 빠른 입출력 */
void fast_io() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
}


/* 방향 변환 char -> int */
int dir_transform(char c) {
    switch (c) {
        case 'U':
            return 0;
        case 'R':
            return 1;
        case 'D':
            return 2;
        case 'L':
            return 3;
    }
}


/* 꼬리 삭제 */
void remove_tail(int y, int x, int dir) {

	// 꼬리 위치 탐색
    while (!grid[y][x].is_tail) {
        dir = (grid[y][x].dir + 2) % 4;
        y += dy[dir];
        x += dx[dir];
    }

    // 꼬리 삭제
    grid[y][x].is_snake = false;
    grid[y][x].is_tail = false;

	// 꼬리 위치 업데이트
    grid[y - dy[dir]][x - dx[dir]].is_tail = true;
}


int main() {
    fast_io();

    cin >> N >> M >> K;

    // 사과 위치 입력
    for (int i = 0; i < M; i++) {
        cin >> y >> x;
        grid[y - 1][x - 1].is_apple = true;
    }

    // 기본 정보 초기화
    init();

    // 방향 전환 입력
    bool game_over = false;
    for (int i = 0; i < K && !game_over; i++) {
        char d;
        int p;
        cin >> d >> p;

        // 방향 정보 변환 char -> int
        int dir = dir_transform(d);
        for (int j = 0; j < p; j++) {
            answer++;

            // 이동 좌표
            int ny = y + dy[dir];
            int nx = x + dx[dir];

            // 격자 범위를 벗어났을 경우 종료
            if (ny < 0 || ny >= N || nx < 0 || nx >= N) {
                game_over = true;
                break;
            }

            // 경로에 사과가 없을 경우
            if (!grid[ny][nx].is_apple) {
                // 꼬리 삭제
                remove_tail(y, x, (dir + 2) % 4);

                // 경로에 몸이 있을 경우 종료
                if (grid[ny][nx].is_snake) {
                    game_over = true;
                    break;
                }
            }

            // 머리 위치 업데이트
            grid[ny][nx] = { false, true, grid[ny][nx].is_tail, dir};
            y = ny;
            x = nx;
        }
    }

    cout << answer << endl;

    return 0;
}
