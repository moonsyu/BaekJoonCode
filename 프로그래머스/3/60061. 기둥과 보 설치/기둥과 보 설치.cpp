#include <string>
#include <vector>
#include <cstring>

#define MAX_N 101 // 교차점 좌표 0 ~ 100

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

#define PILLAR 0
#define BEAM 1

#define REMOVE 0
#define BUILD 1

using namespace std;

int n; // 벽면 크기

// frame[y][x][d] : (x, y) 교차점의 d방향 연결 여부 (상: 기둥 시작, 하: 기둥 끝 또는 바닥, 좌: 보 끝, 우: 보 시작)
bool frame[MAX_N][MAX_N][4];


/* 바닥은 모두 하 방향 true */
void set_floor() {
	for (int x = 0; x <= n; x++) {
		frame[0][x][DOWN] = true;
	}
}


/* (y, x)에 기둥 설치 가능한지 확인 */
bool can_build_pillar(int y, int x) {
	// 바닥 또는 아래 기둥 위
	if (frame[y][x][DOWN]) {
		return true;
	}

	// 보의 한쪽 끝 위
	if (frame[y][x][LEFT] || frame[y][x][RIGHT]) {
		return true;
	}

	return false;
}


/* (y, x)에 보 설치 가능한지 확인 */
bool can_build_beam(int y, int x) {
	// 왼쪽 끝 아래에 기둥
	if (frame[y][x][DOWN]) {
		return true;
	}

	// 오른쪽 끝 아래에 기둥
	if (frame[y][x + 1][DOWN]) {
		return true;
	}

	// 양쪽 끝이 모두 다른 보와 연결
	if (frame[y][x][LEFT] && frame[y][x + 1][RIGHT]) {
		return true;
	}

	return false;
}


/* (y, x) 기둥 설치/삭제 표시 */
void set_pillar(int y, int x, bool state) {
	frame[y][x][UP] = state;
	frame[y + 1][x][DOWN] = state;
}


/* (y, x) 보 설치/삭제 표시 */
void set_beam(int y, int x, bool state) {
	frame[y][x][RIGHT] = state;
	frame[y][x + 1][LEFT] = state;
}


/* (y, x) 기둥 삭제 가능한지 확인 (위쪽 끝 (y + 1, x)의 하는 삭제될 기둥이므로 제외) */
bool can_remove_pillar(int y, int x) {
	int top = y + 1;

	// 위 기둥 : 좌 또는 우에 보가 있어야 함
	if (frame[top][x][UP]) {
		if (!frame[top][x][LEFT] && !frame[top][x][RIGHT]) {
			return false;
		}
	}

	// 위쪽 끝의 왼쪽 보 : 보의 왼쪽 끝 하에 기둥 또는 양쪽 보 연결
	if (frame[top][x][LEFT]) {
		if (!frame[top][x - 1][DOWN] && !(frame[top][x - 1][LEFT] && frame[top][x][RIGHT])) {
			return false;
		}
	}

	// 위쪽 끝의 오른쪽 보 : 보의 오른쪽 끝 하에 기둥 또는 양쪽 보 연결
	if (frame[top][x][RIGHT]) {
		if (!frame[top][x + 1][DOWN] && !(frame[top][x][LEFT] && frame[top][x + 1][RIGHT])) {
			return false;
		}
	}

	return true;
}


/* (y, x) 보 삭제 가능한지 확인 (현재 위치의 우, 오른쪽 위치의 좌는 삭제될 보이므로 제외) */
bool can_remove_beam(int y, int x) {
	// 왼쪽 보 : 양 끝 하에 기둥이 있어야 함
	if (frame[y][x][LEFT]) {
		if (!frame[y][x - 1][DOWN] && !frame[y][x][DOWN]) {
			return false;
		}
	}

	// 오른쪽 보 : 양 끝 하에 기둥이 있어야 함
	if (frame[y][x + 1][RIGHT]) {
		if (!frame[y][x + 1][DOWN] && !frame[y][x + 2][DOWN]) {
			return false;
		}
	}

	// 왼쪽 끝 위 기둥 : 하에 기둥 또는 좌에 보
	if (frame[y][x][UP]) {
		if (!frame[y][x][DOWN] && !frame[y][x][LEFT]) {
			return false;
		}
	}

	// 오른쪽 끝 위 기둥 : 하에 기둥 또는 우에 보
	if (frame[y][x + 1][UP]) {
		if (!frame[y][x + 1][DOWN] && !frame[y][x + 1][RIGHT]) {
			return false;
		}
	}

	return true;
}


/* 기둥 설치 (불가능하면 pass) */
void build_pillar(int y, int x) {
	if (!can_build_pillar(y, x)) {
		return;
	}

	set_pillar(y, x, true);
}


/* 보 설치 (불가능하면 pass) */
void build_beam(int y, int x) {
	if (!can_build_beam(y, x)) {
		return;
	}

	set_beam(y, x, true);
}


/* 기둥 삭제 (불가능하면 pass) */
void remove_pillar(int y, int x) {
	if (!can_remove_pillar(y, x)) {
		return;
	}

	set_pillar(y, x, false);
}


/* 보 삭제 (불가능하면 pass) */
void remove_beam(int y, int x) {
	if (!can_remove_beam(y, x)) {
		return;
	}

	set_beam(y, x, false);
}


/* 최종 구조물 탐색 (x → y → 기둥 → 보 순서라 정렬 불필요) */
vector<vector<int>> get_result() {
	vector<vector<int>> result;

	for (int x = 0; x <= n; x++) {
		for (int y = 0; y <= n; y++) {
			if (frame[y][x][UP]) {
				result.push_back({ x, y, PILLAR });
			}

			if (frame[y][x][RIGHT]) {
				result.push_back({ x, y, BEAM });
			}
		}
	}

	return result;
}


vector<vector<int>> solution(int n_size, vector<vector<int>> build_frame) {
	n = n_size;
	memset(frame, false, sizeof(frame));

	set_floor();

	for (vector<int>& cmd : build_frame) {
		int x = cmd[0];
		int y = cmd[1];
		int type = cmd[2];
		int op = cmd[3];

		if (type == PILLAR) {
			if (op == BUILD) {
				build_pillar(y, x);
			}
			else {
				remove_pillar(y, x);
			}
		}
		else {
			if (op == BUILD) {
				build_beam(y, x);
			}
			else {
				remove_beam(y, x);
			}
		}
	}

	return get_result();
}