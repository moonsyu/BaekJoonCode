#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct cmp {
    bool operator()(vector<int>& a, vector<int>& b) {
        if (a[1] != b[1]) {
			return a[1] < b[1];
        }

		return a[0] < b[0];
    }
};

int solution(vector<vector<int>> targets) {
    
    // 마지막 좌표 기준 정렬
    sort(targets.begin(), targets.end(), cmp());

    int answer = 0;
    int last_fw = -1;
    for(vector<int>& target : targets) {
        if (target[0] >= last_fw) {
            answer++;
            last_fw = target[1];
        }
    }

    return answer;
}