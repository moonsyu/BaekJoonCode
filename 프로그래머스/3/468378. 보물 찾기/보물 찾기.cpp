#include <string>
#include <vector>

#define MAX_W 202 // 1 ~ 200열 + 빈 구간 확인용 여유 칸
#define INF 0x3f3f3f3f

using namespace std;

extern int excavate(int);

int w; // 열 개수
int col_depth[MAX_W]; // col_depth[i] : i열의 깊이 (1번부터)

// dp[l][r] : 보물이 l ~ r열에 있을 때, 최악의 경우에도 찾는 최소 비용
int dp[MAX_W][MAX_W];

// best_col[l][r] : dp[l][r]을 만드는 처음 팔 열
int best_col[MAX_W][MAX_W];


/* l ~ r 구간 비용 반환 (빈 구간은 0) */
int get_dp(int l, int r) {
	if (l > r) {
		return 0;
	}

	return dp[l][r];
}


/* 구간 길이 순으로 dp 계산 */
void set_dp() {
	for (int len = 1; len <= w; len++) {
		for (int l = 1; l + len - 1 <= w; l++) {
			int r = l + len - 1;
			dp[l][r] = INF;

			// k열을 처음 파는 경우
			for (int k = l; k <= r; k++) {
				// 상대는 비용이 더 드는 쪽으로 답함
				int worst = max(get_dp(l, k - 1), get_dp(k + 1, r));
				int cost = col_depth[k] + worst;

				// 최악의 비용이 가장 작은 열 선택
				if (cost < dp[l][r]) {
					dp[l][r] = cost;
					best_col[l][r] = k;
				}
			}
		}
	}
}


/* best_col을 따라 굴착하며 보물 찾기 */
int find_treasure() {
	int l = 1;
	int r = w;

	while (l <= r) {
		int col = best_col[l][r];
		int result = excavate(col);

		// 보물 발견
		if (result == 0) {
			return col;
		}

		// 보물이 왼쪽에 있음
		if (result == -1) {
			r = col - 1;
		}
		// 보물이 오른쪽에 있음
		else {
			l = col + 1;
		}
	}

	return -1;
}


int solution(vector<int> depth, int money) {
	w = depth.size();

	for (int i = 1; i <= w; i++) {
		col_depth[i] = depth[i - 1];
	}

	set_dp();

	return find_treasure();
}