#include <iostream>

using namespace std;

int N;
int y, x;
char grid[101][101];

// 방문 배열
bool visited[101][101][4];

// 회전 방향 (상, 우, 하, 좌)
int dy[] = { -1, 0, 1, 0 };
int dx[] = { 0, 1, 0, -1 };

/* 입출력 최적화 */
void fast_io() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
}

int main() {
    fast_io();

    cin >> N;
    cin >> y >> x;

    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            cin >> grid[i][j];
        }
    }

    // Please write your code here.
    int dir = 1;
    int answer = 0;
    int escape = false;
    while (!escape) {
        answer++;

        // 중복 방문 시 탈출 불가
        if (visited[y][x][dir]) {
            answer = -1;
            break;
        }

        visited[y][x][dir] = true;

        // 우측 좌표
        int ry = y + dy[(dir + 1) % 4];
        int rx = x + dx[(dir + 1) % 4];

        // 우측이 비어 있다면 우측으로 이동
        if (grid[ry][rx] == '.') {
            y = ry;
            x = rx;
            dir = (dir + 1) % 4;

            continue;
        }

        // 현재 방향 좌표
        int ny = y + dy[dir];
        int nx = x + dx[dir];

        // 미로 탈출 확인
        if (ny < 1 || ny > N || nx < 1 || nx > N) {
            break;
        }

        // 현재 방향으로 이동 가능하다면 이동
        if (grid[ny][nx] == '.') {
            y = ny;
            x = nx;

            continue;
        }

        // 이동이 불가능 하다면 좌측으로 방향 전환
        while (!visited[y][x][(dir + 3) % 4]) {
            dir = (dir + 3) % 4;
            visited[y][x][dir] = true;

            ny = y + dy[dir];
            nx = x + dx[dir];

            // 해당 방향으로 탈출 가능할 경우 탈출
            if (ny < 1 || ny > N || nx < 1 || nx > N) {
                escape = true;
                break;
            }

            // 해당 방향으로 이동 가능하다면 이동
            if (grid[ny][nx] == '.') {
                y = ny;
                x = nx;
                break;
            }
        }
    }

    cout << answer;

    return 0;
}
