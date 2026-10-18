#include <string>
#include <vector>
#include <unordered_set>


using namespace std;

int user_size, ban_size;
unordered_set<int> banned_set;

int check_user;


/* 밴 매칭 여부 확인 */
bool is_match(string& user, string& ban) {
	
	// 길이 확인
	if (user.size() != ban.size()) {
		return false;
	}

	// 각 자리 문자 확인
	for (int i = 0; i < user.size(); i++) {
		if (ban[i] == '*') {
			continue;
		}

		if (user[i] != ban[i]) {
			return false;
		}
	}

	return true;
}


/* 밴 조합 확인 */
void combination(vector<string>& user_id, vector<string>& banned_id, int ban_idx) {
    if (ban_idx == ban_size) {
        banned_set.insert(check_user);
        return;
    }
	
	for (int i = 0; i < user_size; i++) {
        
        // 이미 밴한 유저 여부 확인
        if (check_user & (1 << i)) {
            continue;
        }

		// 밴 아이디일 경우
        if (is_match(user_id[i], banned_id[ban_idx])) {
			check_user |= (1 << i);
			combination(user_id, banned_id, ban_idx + 1);
			check_user &= ~(1 << i);
        }
    }
}


int solution(vector<string> user_id, vector<string> banned_id) {
    int answer = 0;
	user_size = user_id.size();
    ban_size = banned_id.size();

	check_user = 0;
	combination(user_id, banned_id, 0);
	answer = banned_set.size();

    return answer;
}