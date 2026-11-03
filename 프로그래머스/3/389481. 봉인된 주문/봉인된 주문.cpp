#include <string>
#include <vector>
#include <algorithm>

using namespace std;


/* 주문 순서 비교 (길이 짧은 순 → 사전 순) */
bool cmp(const string& a, const string& b) {
	if (a.size() != b.size()) {
		return a.size() < b.size();
	}

	return a < b;
}


/* 주문 → 숫자 (a=1 ~ z=26 인 26진법) */
long long to_num(string& s) {
	long long num = 0;

	// 각 자릿수 순회
	for (int i = 0; i < s.size(); i++) {
		num *= 26;
		num += (s[i] - 'a') + 1;
	}

	return num;
}


/* 숫자 → 주문 (아래 자리부터) */
string to_str(long long num) {
	string s = "";

	while (num > 0) {
		num--; // 1 ~ 26 → 0 ~ 25
		s += (char)('a' + num % 26);
		num /= 26;
	}

	// 아래 자리부터 붙였으므로 뒤집기
	reverse(s.begin(), s.end());
	return s;
}


string solution(long long n, vector<string> bans) {
	// bans 정렬
	sort(bans.begin(), bans.end(), cmp);

	// bans 값을 숫자로 변환 후 n 이하이면 n 밀기
	for (int i = 0; i < bans.size(); i++) {
		// 정렬되어 있으므로 이후 주문은 모두 n보다 뒤
		if (to_num(bans[i]) > n) {
			break;
		}

		n++;
	}

	// 3. n 값을 문자로 변환
	return to_str(n);
}