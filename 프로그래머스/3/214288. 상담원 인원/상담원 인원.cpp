#include <string>
#include <vector>
#include <queue>
#include <climits>

#define MAX_TYPE 5

using namespace std;

/* 상담 요청 정보 구조체
int start : 상담 요청 시각
int dur : 상담 시간
*/
struct PQ_INFO {
	int start;
	int dur;
};

/* 상담 요청 우선순위 비교 구조체 */
struct compare {
	bool operator()(const PQ_INFO& a, const PQ_INFO& b) {
		if (a.start != b.start) {
			return a.start > b.start;
		}
		return a.dur > b.dur;
	}
};

int type_cnt; // 상담 유형 수
int mentor_cnt; // 멘토 수

// mentor[i] : i번 유형에 배치된 멘토 수
int mentor[MAX_TYPE];

// ready_pq[i] : i번 유형의 전체 상담 요청
priority_queue<PQ_INFO, vector<PQ_INFO>, compare> ready_pq[MAX_TYPE];

// 최소 대기 시간
int answer = INT_MAX;


/* 상담 요청을 유형별 ready_pq에 저장 */
void set_ready_pq(vector<vector<int>>& reqs) {
	for (int i = 0; i < reqs.size(); i++) {
		int type = reqs[i][2] - 1;
		ready_pq[type].push({ reqs[i][0], reqs[i][1] });
	}
}


/* type번 유형의 총 대기 시간 반환 */
int get_wait_time(int type) {
	int wait = 0;

	// 요청 복사 (조합마다 다시 써야 함)
	priority_queue<PQ_INFO, vector<PQ_INFO>, compare> req = ready_pq[type];

	// 멘토 수만큼 0 넣기
	priority_queue<int, vector<int>, greater<int>> pq;
	for(int i = 0; i < mentor[type]; i++) {
		pq.push(0);
	}

	// ready_pq가 빌 때까지 가장 빨리 끝나는 멘토에게 배정
	while (!req.empty()) {
		PQ_INFO cur = req.top();
		req.pop();

		int end_time = pq.top();
		pq.pop();

		// 멘토가 상담 중이면 대기
		if (end_time > cur.start) {
			wait += end_time - cur.start;
			pq.push(end_time + cur.dur);
		}
		else {
			pq.push(cur.start + cur.dur);
		}
	}

	return wait;
}


/* 현재 멘토 배치로 총 대기 시간 반환 */
int simulate() {
	int total = 0;

	for (int i = 0; i < type_cnt; i++) {
		total += get_wait_time(i);
	}

	return total;
}


/* 조합 */
void combination(int start, int remain) {
	// 남은 멘토 모두 배치 완료
	if (!remain) {
		answer = min(answer, simulate());
		return;
	}

	// 조합 생성
	for (int i = start; i < type_cnt; i++) {
		mentor[i]++;
		combination(i, remain - 1);
		mentor[i]--;
	}
}


int solution(int k, int n, vector<vector<int>> reqs) {
	type_cnt = k;
	mentor_cnt = n;
	answer = INT_MAX;

	set_ready_pq(reqs);

	// 유형별 멘토 1명씩 배치
	for (int i = 0; i < type_cnt; i++) {
		mentor[i] = 1;
		mentor_cnt--;
	}

	combination(0, mentor_cnt);

	return answer;
}