#include <iostream>
#include <algorithm>

using namespace std;

int n;
int coin[1001];
int dp[1001][4];

int main() {
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> coin[i];
    }

    // 2칸씩 이동한 dp
    dp[n][0] = coin[n];
    for (int i = n - 2; i >= 0; i -= 2) {
        dp[i][0] = dp[i + 2][0] + coin[i];
    }

    // 1칸 이동을 최대 k번 사용하는 경우
    for (int k = 1; k <= 3; k++) {
        dp[n][k] = coin[n];
        dp[n - 1][k] = coin[n] + coin[n - 1];

        for (int i = n - 2; i >= 0; i--) {
            dp[i][k] =
                max(dp[i + 2][k], dp[i + 1][k - 1]) + coin[i];
        }
    }

    cout << max({ dp[0][0], dp[0][1], dp[0][2], dp[0][3] }) << '\n';
}
