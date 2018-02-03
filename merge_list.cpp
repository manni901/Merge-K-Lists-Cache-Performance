#include <algorithm>
#include <chrono>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <queue>

using namespace std;

#define NUM_ITER 1
#define P2INT pair<int, int>

int NUM_LISTS = 100;
int LIST_SIZE = 100;

// Returns a sorted list of randomly generated integers in [0, INT_MAX).
vector<int> GetRandomSortedList(int size) {
  vector<int> list;
  while (size--) {
    int val = rand() % INT_MAX;
    list.push_back(val);
  }
  sort(list.begin(), list.end());
  return list;
}

// Merges NUM_LISTS(k) number of sorted lists each having LIST_SIZE elements.
// Total elements are n = (k . LIST_SIZE) with k comparisons for each number.
// Time complexity O(n . k).
vector<int> NaiveMergeKLists(const vector<vector<int>> &lists) {
  int merge_size = NUM_LISTS * LIST_SIZE;
  // Keeps track of the current value to be processed from list[i].
  vector<int> current_index(NUM_LISTS, 0);
  vector<int> merged_list(merge_size, 0);
  for (int &val : merged_list) {
    int min = INT_MAX;
    int min_index = -1;
    // Loop to find the next smallest element accross all lists, along with
    // the index of the list to which that element belongs.
    for (int i = 0; i < NUM_LISTS; i++) {
      if (current_index[i] < LIST_SIZE && lists[i][current_index[i]] < min) {
        min = lists[i][current_index[i]];
        min_index = i;
      }
    }
    val = min;
    current_index[min_index]++;
  }
  return merged_list;
}

// Merges NUM_LISTS(k) number of sorted lists each having LIST_SIZE elements.
// Total elements are n = (k . LIST_SIZE) with lg(k) comparisons for each 
// number by using a heap of size k. Time complexity O(n . lg(k)).
vector<int> HeapMergeKLists(const vector<vector<int>> &lists) {
  int merge_size = NUM_LISTS * LIST_SIZE;
  vector<int> merged_list(merge_size, 0);

  auto cmp = [&lists](const P2INT &a, const P2INT &b) {
    return lists[a.first][a.second] > lists[b.first][b.second];
  };

  // Priority queue stores pair<int, int> where
  // pair.first : index of the list (0 .. NUM_LISTS-1)
  // pair.second : index of current element in that list (0 .. LIST_SIZE-1).
  priority_queue<P2INT, vector<P2INT>, decltype(cmp)> Q(cmp);
  for (int i = 0; i < NUM_LISTS; i++) {
    Q.push(make_pair(i, 0));
  }
  for (int &val : merged_list) { // Condition could also have been !Q.empty
    auto pair = Q.top();
    val = lists[pair.first][pair.second];
    int next_index = pair.second + 1;
    Q.pop();
    if (next_index < LIST_SIZE) {
      Q.push(make_pair(pair.first, next_index));
    }
  }
  return merged_list;
}

int main(int argc, char **argv) {
  srand(time(nullptr));
  vector<vector<int>> lists;
  if (argc == 3) {
    NUM_LISTS = stoi(argv[1]);
    LIST_SIZE = stoi(argv[2]);
  }
  int num_lists = NUM_LISTS;

  // Generate all sorted lists of random integers.
  while (num_lists--) {
    lists.push_back(GetRandomSortedList(LIST_SIZE));
  }

  vector<int> merged_list;
  vector<int> merged_list_two;

  long long int naive_duration = 0;
  long long int heap_duration = 0;

  for (int i = 0; i < NUM_ITER; ++i) {
    auto begin = chrono::high_resolution_clock::now();
    merged_list = NaiveMergeKLists(lists);
    auto end = chrono::high_resolution_clock::now();
    auto duration =
        chrono::duration_cast<chrono::nanoseconds>(end - begin).count();
    naive_duration += duration;
  }
  cout << "Avg Naive Merge Duration : " << naive_duration / NUM_ITER << "\n";

  for (int i = 0; i < NUM_ITER; ++i) {
    auto begin = chrono::high_resolution_clock::now();
    merged_list_two = HeapMergeKLists(lists);
    auto end = chrono::high_resolution_clock::now();
    auto duration =
        chrono::duration_cast<chrono::nanoseconds>(end - begin).count();
    heap_duration += duration;
  }
  cout << "Avg Heap Merge Duration : " << heap_duration / NUM_ITER << "\n";

  // Check if sorting is successful and print numbers from the merged
  // list in order.
  int merge_size = NUM_LISTS * LIST_SIZE;
  for (int i = 1; i < merge_size; i++) {
    //cout << merged_list[i] << "....." << merged_list_two[i] << "\n";
    if (merged_list[i] != merged_list_two[i]) {
      cout << "ERROR: SORTED LISTS DON'T MATCH!!\n";
      return 1;
    }
    if (merged_list[i] < merged_list[i - 1]) {
      cout << "ERROR: INCORRECT SORT ORDER!!\n";
      return 1;
    }
  }

  return 0;
}
