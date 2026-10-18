#include <iostream>
#include <algorithm>

using namespace std;


int money; // 보유 금액
int cloth_cnt; // 옷 개수
long long answer; // 선택한 옷의 비트마스트
int max_quality; // 선택한 옷의 최대 품질

/* 옷의 가격 및 품질 */
struct cloth_data {
	int price;
	int quality;
};

// 옷 정보 배열
cloth_data clothes[25];


/* 입출력 최적화 */
void fast_io() {
	cin.tie(nullptr);
	ios::sync_with_stdio(false);
	cout.tie(nullptr);
}


/* 입력 */
void input() {
	cin >> money >> cloth_cnt;
	for (int i = 0; i < cloth_cnt; i++) {
		cin >> clothes[i].price >> clothes[i].quality;
	}
}


/* 조합 */
void combination(int idx, long long mask, int quality) {
	// 최대 품질 갱신
	if (max_quality < quality) { 
		max_quality = quality; 
		answer = mask; 
	}

	for (int i = idx; i < cloth_cnt; i++) {
		if (money >= clothes[i].price) { // 구매 가능한 경우
			mask |= (1LL << i); // 옷 선택
			money -= clothes[i].price; // 보유 금액 차감
			combination(i + 1, mask, quality + clothes[i].quality); // 다음 옷 선택

			mask &= ~(1LL << i); // 옷 선택 해제
			money += clothes[i].price; // 보유 금액 복구
		}
	}
}


/* 출력 */
void output() {
	for (int i = 0; i < 25; i++) {
		if ((answer >> i) & 1) {
			cout << i << " ";
		}
	}

	cout << max_quality << "\n";
}


int main() {
	fast_io();

	int tc;
	cin >> tc;

	for (int t = 1; t <= tc; t++) {
		input();

		answer = 0;
		max_quality = 0;
		long long mask = 0;
		int  quality = 0;
		combination(0, mask, quality);

		cout << "#" << t << " ";
		output();
	}
}