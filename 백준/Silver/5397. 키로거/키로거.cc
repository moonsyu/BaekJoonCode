#include <iostream>
#include <string>

#define MAX_LEN 1000000

using namespace std;

struct Node {
	char value;
	Node* prev;
	Node* next;
};

Node nodes[MAX_LEN + 1];

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;

	while (t--) {
		string input;
		cin >> input;

		int used = 0;
		Node* head = &nodes[0];
		head->next = nullptr;
		Node* cur = head; // 커서 바로 왼쪽 노드. 맨 앞이면 head

		for (char key : input) {
			if (key == '<') {
				if (cur != head) {
					cur = cur->prev;
				}
			}
			else if (key == '>') {
				if (cur->next != nullptr) {
					cur = cur->next;
				}
			}
			else if (key == '-') {
				if (cur == head) {
					continue;
				}

				Node* prev = cur->prev;
				prev->next = cur->next;
				if (cur->next != nullptr) {
					cur->next->prev = prev;
				}
				cur = prev;
			}
			else {
				Node* node = &nodes[++used];
				node->value = key;
				node->prev = cur;
				node->next = cur->next;
				if (node->next != nullptr) {
					node->next->prev = node;
				}
				cur->next = node;
				cur = node;
			}
		}

		for (Node* node = head->next; node != nullptr; node = node->next) {
			cout << node->value;
		}
		cout << '\n';
	}

	return 0;
}