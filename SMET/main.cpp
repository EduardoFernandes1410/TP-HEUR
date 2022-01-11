#include <bits/stdc++.h>
using namespace std;

const int INF = 0x3f3f3f3f;

int J_size, W_size;
vector<tuple<int, int, int>> J; //[s_j, f_j, j] - tasks data
vector<vector<int>> P; // set of workers who can perform task
vector<vector<int>> T; // set of tasks which can be performed by worker

bool comp2(tuple<int, int, int> a, tuple<int, int, int> b) {
  auto [s1, f1, j1] = a;
  auto [s2, f2, j2] = b;
  if(f1 == f2) return s1 < s2;
  return f1 < f2;
}

vector<vector<int>> find_maximal_cliques() {
  vector<vector<int>> C;
  int p = 0;

  auto J_s = J;
  auto J_f = J;
  sort(J_s.begin(), J_s.end());
  sort(J_f.begin(), J_f.end(), comp2);

  int s = get<0>(J_s[0]);
  do {
    int f_index = upper_bound(J_f.begin(), J_f.end(), make_tuple(INF, s, 0), comp2) - J_f.begin();
    int f = get<1>(J_f[f_index]);

    C.push_back(vector<int>());
    for(int j = f_index; j < J_f.size(); j++) {
      auto [s1, f1, j1] = J_f[j];
      if(s1 < f) C[C.size()-1].push_back(j1);
    }

    auto it = lower_bound(J_s.begin(), J_s.end(), make_tuple(f, -INF, 0));
    if(it == J_s.end()) break;
    s = get<0>(*it);
  } while(true);

  return C;
}

int main() {
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

  auto C = find_maximal_cliques();
  cout << endl << "C:" << endl;
  for(auto i : C) {
    for(auto j : i) cout << j << " ";
    cout << endl;
  }

  return 0;
}