#include <string>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

int n; // 체크포인트 수
int half; // n / 2
vector<int> h; // 오름차순 정렬된 높이


/* 정렬 기준 half + 1개 연속 구간의 높이 차 중 가장 작은 값 2개 계산 */
void get_min_span(int& min1, int& min2) {
	min1 = INT_MAX;
	min2 = INT_MAX;

	for (int i = 0; i + half < n; i++) {
		int span = h[i + half] - h[i];

		if (span < min1) {
			min2 = min1;
			min1 = span;
		}
		else if (span < min2) {
			min2 = span;
		}
	}
}


int solution(vector<int> heights) {
	h = heights;
	n = h.size();
	half = n / 2;

	sort(h.begin(), h.end());

	int min1, min2;
	get_min_span(min1, min2);

	// 짝수: 가장 작은 값, 홀수: 두 번째로 작은 값
	if (n % 2 == 0) {
		return min1;
	}

	return min2;
}