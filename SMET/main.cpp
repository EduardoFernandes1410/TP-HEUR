#include <bits/stdc++.h>
using namespace std;

int main() {
  int J_size, W_size;
  vector<tuple<int, int, int>> J; //[s_j, f_j, j] - tasks data
  vector<vector<int>> P; // set of workers who can perform task
  vector<vector<int>> T; // set of tasks which can be performed by worker

  char trash;
  string input;
  getline(cin, input);
  getline(cin, input);
  getline(cin, input);
  getline(cin, input);

  cin >> input >> input >> J_size;
  cout << "J_size: " << J_size << endl;

  J.resize(J_size);
  P.resize(J_size);
  for(int i = 0; i < J_size; i++) {
    int s, f; cin >> s >> f;
    J[i] = {s, f, i};
  }

  cin >> input >> input >> W_size;
  cout << "W_size: " << W_size << endl;

  T.resize(W_size);
  for(int i = 0; i < W_size; i++) {
    int num_tasks; cin >> num_tasks >> trash;
  
    T[i].resize(num_tasks);
  
    for(int j = 0; j < num_tasks; j++) {
      cin >> T[i][j];
      P[T[i][j]].push_back(i);
    }
  }

  cout << "Tasks:" << endl;
  for(auto [s, f, j] : J) cout << s << ", " << f << ", " << j << endl;

  cout << endl << "P:" << endl;
  for(auto i : P) {
    for(auto j : i) cout << j << " ";
    cout << endl;
  }
  cout << endl << "T:" << endl;
  for(auto i : T) {
    for(auto j : i) cout << j << " ";
    cout << endl;
  }

  return 0;
}