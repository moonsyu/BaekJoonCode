#include <string>
#include <vector>
#include <algorithm>

#define MAX_TEMP 51 // -10 ~ 40도를 0 ~ 50으로 이동
#define OFFSET 10
#define INF 1e9

using namespace std;

int out_temp; // 실외 온도 (OFFSET 적용)
int low, high; // 승객 탑승 시 유지해야 하는 온도 범위 (OFFSET 적용)
int cost_move, cost_keep; // a: 희망온도 != 실내온도, b: 희망온도 == 실내온도

// dp[t] : 현재 분에 실내 온도가 t일 때 최소 소비 전력
int dp[MAX_TEMP];
int next_dp[MAX_TEMP];


/* 에어컨 끄면 다음 분 온도 (실외 온도 쪽으로 1도) */
int get_off_temp(int t) {
	if (t > out_temp) {
		return t - 1;
	}
	if (t < out_temp) {
		return t + 1;
	}
	return t;
}


/* 승객이 있을 때 온도 범위 안인지 확인 */
bool is_valid(int t, int onboard) {
	if (!onboard) {
		return true;
	}

	return low <= t && t <= high;
}


/* 다음 온도 상태 갱신 */
void update(int nt, int cost) {
	if (nt < 0 || nt >= MAX_TEMP) {
		return;
	}

	next_dp[nt] = min(next_dp[nt], cost);
}


int solution(int temperature, int t1, int t2, int a, int b, vector<int> onboard) {
	out_temp = temperature + OFFSET;
	low = t1 + OFFSET;
	high = t2 + OFFSET;
	cost_move = a;
	cost_keep = b;

	// 0분: 실내 온도 = 실외 온도
	fill(dp, dp + MAX_TEMP, INF);
	dp[out_temp] = 0;

	for (int i = 0; i < onboard.size() - 1; i++) {
		fill(next_dp, next_dp + MAX_TEMP, INF);

		for (int t = 0; t < MAX_TEMP; t++) {
			// 도달 불가능한 온도
			if (dp[t] == INF) {
				continue;
			}

			// 승객이 있는데 범위 밖
			if (!is_valid(t, onboard[i])) {
				continue;
			}

			// 에어컨 끄기: 실외 온도 쪽으로 1도, 전력 0
			update(get_off_temp(t), dp[t]);

			// 에어컨 켜고 온도 변경: 1도 이동, 전력 a
			update(t - 1, dp[t] + cost_move);
			update(t + 1, dp[t] + cost_move);

			// 에어컨 켜고 온도 유지: 전력 b
			update(t, dp[t] + cost_keep);
		}

		copy(next_dp, next_dp + MAX_TEMP, dp);
	}

	// 마지막 분 확인
	int answer = INF;
	int last = onboard.size() - 1;
	for (int t = 0; t < MAX_TEMP; t++) {
		if (!is_valid(t, onboard[last])) {
			continue;
		}

		answer = min(answer, dp[t]);
	}

	return answer;
}