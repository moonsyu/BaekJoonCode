#include <iostream>
using namespace std;

const long long MOD = 1000000007;
long long dp[1001][3][3];

int main() {
    int n;
    cin >> n;

    // 0일 동안 아무 평가도 받지 않은 경우는 1가지
    dp[0][0][0] = 1;

	// day: 현재까지 살아남은 날 수
    // day를 기준으로 day + 1의 값을 채워 나가는 방식
    for (int day = 0; day < n; day++) {
        for (int t = 0; t <= 2; t++) {
            for (int b = 0; b <= 2; b++) {
				// day날에 T를 t번, B를 b번 받았을 때 경우의 수
                long long ways = dp[day][t][b];

                // day + 1에 G를 받을 경우
                dp[day + 1][t][0] =
                    (dp[day + 1][t][0] + ways) % MOD;

                // day + 1에 T를 받을 경우
                if (t < 2) {
                    dp[day + 1][t + 1][0] =
                        (dp[day + 1][t + 1][0] + ways) % MOD;
                }

                // day + 1에 B를 받을 경우
                if (b < 2) {
                    dp[day + 1][t][b + 1] =
                        (dp[day + 1][t][b + 1] + ways) % MOD;
                }
            }
        }
    }

    // n일까지 살아남은 모든 상태의 경우의 수를 합산
    long long answer = 0;
    for (int t = 0; t <= 2; t++) {
        for (int b = 0; b <= 2; b++) {
            answer = (answer + dp[n][t][b]) % MOD;
        }
    }

    cout << answer << '\n';
}
