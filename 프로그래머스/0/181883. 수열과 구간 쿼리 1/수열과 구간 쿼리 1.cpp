#include <string>
#include <vector>

using namespace std;

vector<int> solution(vector<int> arr, vector<vector<int>> queries) {
    vector<int> answer;
    
    int idx = 0;
    for (auto& query : queries) {
        for (int i = query[0]; i <= query[1]; i++) {
            arr[i]++;
        }
    }
    
    
    return arr;
}