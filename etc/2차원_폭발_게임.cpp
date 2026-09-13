#include <iostream>

using namespace std;

int N, M, K;
int area[100][100];
int temp[100][100];

int pre_ans, answer;

/* 폭탄 터뜨리기 */
void boom(int x, int s, int e) {
    for (int i = s; i < e; i++) {
        area[i][x] = 0;
    }

    answer -= (e - s);
}

/* 폭탄 확인 */
void boom_check() {
    for (int x = 0; x < N; x++) {
        int s, num;
        num = s = 0;
        for (int y = 0; y < N; y++) {
            if (num != area[y][x] && area[y][x]) {
                if (y - s >= M && num) {
                    boom(x, s, y);
                }

                num = area[y][x];
                s = y;
            }
        }

        if (N - s >= M && num) {
            boom(x, s, N);
        }
    }
}


/* 폭탄 내리기 */
void drop_boom() {
    for (int x = 0; x < N; x++) {
        // 세로 축 기준 아래에서부터 올라오며 빈칸 탐색
        int idx = -1;
        for (int y = N - 1; y >= 0; y--) {
            if (area[y][x] == 0) {
                idx = y;
                break;
            }
        }

        // 아래 칸이 비어있을 경우 폭탄 내리기
        for (int y = idx - 1; y >= 0; y--) {
            if (area[y][x]) {
                area[idx--][x] = area[y][x];
                area[y][x] = 0;
            }
        }
    }
}

/*
* (y + 0, x) (y + 0, x + 1) (y + 0, x + 2) (y + 0, x + 3)
* (y + 1, x) (y + 1, x + 1) (y + 1, x + 2) (y + 1, x + 3)
* (y + 2, x) (y + 2, x + 1) (y + 2, x + 2) (y + 2, x + 3)
* (y + 3, x) (y + 3, x + 1) (y + 3, x + 2) (y + 3, x + 3)
*/

/* 영역 회전 */
void rotate_area() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            temp[j][N - 1 - i] = area[i][j];
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            area[i][j] = temp[i][j];
        }
    }
}


///* 폭탄 개수 탐색 */
//int boom_count() {
//    int result = 0;
//    for (int i = 0; i < N; i++) {
//        for (int j = 0; j < N; j++) {
//            if (area[i][j]) {
//                result++;
//            }
//        }
//    }
//
//    return result;
//}


/* 영역 출력 */
void print_area() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << area[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}


int main() {
    cin >> N >> M >> K;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> area[i][j];
        }
    }

    // 초기 폭탄 개수
    answer = N * N;
    pre_ans = -1;

    // Please write your code here.
    for (int i = 0; i <= K; i++) {
        do {
            pre_ans = answer;
            boom_check();
            drop_boom();
        } while (pre_ans != answer);

        rotate_area();
        drop_boom();
    }

    cout << answer << endl;

    return 0;
}
