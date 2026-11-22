#include <string>
#include <vector>

#define MAX_SCORE 11 // 10점 ~ 0점
#define ZERO_IDX 10 // 0점 인덱스

using namespace std;

int arrow_cnt; // 화살 수
vector<int> apeach; // apeach[i] : (10 - i)점에 어피치가 맞힌 화살 수

// lion[i] : 현재 탐색 중인 라이언 배치, (10 - i)점에 맞힌 화살 수
vector<int> lion;

int best_diff; // 최대 점수 차
vector<int> answer; // 최대 점수 차를 만드는 라이언 배치


/* 현재 라이언 배치의 점수 차 반환 (라이언 - 어피치) */
int get_diff() {
	int lion_score = 0;
	int apeach_score = 0;

	// 0점은 점수 없음
	for (int i = 0; i < ZERO_IDX; i++) {
		int score = 10 - i;

		// 둘 다 못 맞힘
		if (lion[i] == 0 && apeach[i] == 0) {
			continue;
		}

		// 더 많이 맞힌 쪽이 가져감 (같으면 어피치)
		if (lion[i] > apeach[i]) {
			lion_score += score;
		}
		else {
			apeach_score += score;
		}
	}

	return lion_score - apeach_score;
}


/* 현재 배치가 answer보다 낮은 점수를 더 많이 맞혔는지 확인 */
bool is_better() {
	// 0점부터 비교
	for (int i = ZERO_IDX; i >= 0; i--) {
		if (lion[i] != answer[i]) {
			return lion[i] > answer[i];
		}
	}

	return false;
}


/* 현재 배치로 정답 갱신 */
void update() {
	int diff = get_diff();

	// 이기지 못함
	if (diff <= 0) {
		return;
	}

	// 점수 차가 더 크거나, 같으면 낮은 점수를 더 많이 맞힌 배치
	if (diff > best_diff || (diff == best_diff && is_better())) {
		best_diff = diff;
		answer = lion;
	}
}


/* idx번 과녁((10 - idx)점)부터 남은 화살 배치 */
void dfs(int idx, int remain) {
	// 0점 과녁: 남은 화살 모두 배치
	if (idx == ZERO_IDX) {
		lion[ZERO_IDX] = remain;
		update();
		lion[ZERO_IDX] = 0;
		return;
	}

	// 해당 점수 가져가기: 어피치보다 1발 더
	int need = apeach[idx] + 1;
	if (need <= remain) {
		lion[idx] = need;
		dfs(idx + 1, remain - need);
		lion[idx] = 0;
	}

	// 해당 점수 포기: 화살 0발
	dfs(idx + 1, remain);
}


vector<int> solution(int n, vector<int> info) {
	arrow_cnt = n;
	apeach = info;
	lion.assign(MAX_SCORE, 0);
	best_diff = 0;
	answer = { -1 };

	dfs(0, arrow_cnt);

	return answer;
}