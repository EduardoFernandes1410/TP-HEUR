#include <bits/stdc++.h>
#include "NAMESPACE/smet_namespace.h"
#include "CONSTRUCTIVE/constructive.h"
#include "IMPROVEMENT/improvement.h"
using namespace std;

const int INF = 0x3f3f3f3f;

void read_instance() {
  char trash;
  string input;
  getline(cin, input);
  getline(cin, input);
  getline(cin, input);
  getline(cin, input);

  cin >> input >> input >> smet::J_size;

  smet::J.resize(smet::J_size);
  smet::P.resize(smet::J_size);
  for(int i = 0; i < smet::J_size; i++) {
    int s, f; cin >> s >> f;
    smet::J[i] = {s, f, i};
  }

  cin >> input >> input >> smet::W_size;

  smet::T.resize(smet::W_size);
  for(int i = 0; i < smet::W_size; i++) {
    int num_tasks; cin >> num_tasks >> trash;
  
    smet::T[i].resize(num_tasks);
  
    for(int j = 0; j < num_tasks; j++) {
      cin >> smet::T[i][j];
      smet::P[smet::T[i][j]].push_back(i);
    }

    sort(smet::T[i].begin(), smet::T[i].end());
  }
}

void print_instance() {
  cout << "J_size: " << smet::J_size << endl;
  cout << "W_size: " << smet::W_size << endl;

  cout << "Tasks:" << endl;
  for(auto [s, f, j] : smet::J) cout << s << ", " << f << ", " << j << endl;

  cout << endl << "P:" << endl;
  for(auto i : smet::P) {
    for(auto j : i) cout << j << " ";
    cout << endl;
  }
  cout << endl << "T:" << endl;
  for(auto i : smet::T) {
    for(auto j : i) cout << j << " ";
    cout << endl;
  }

  cout << endl << "C:" << endl;
  for(auto i : smet::C) {
    for(auto j : i) cout << j << " ";
    cout << endl;
  }

  cout << endl << "C_w:" << endl;
  for(auto w : smet::C_w) {
    for(auto k : w) {
      cout << "{";
      for(auto j : k) cout << j << " ";
      cout << "}, ";
    }
    cout << endl;
  }
}

bool comp2(tuple<int, int, int> a, tuple<int, int, int> b) {
  auto [s1, f1, j1] = a;
  auto [s2, f2, j2] = b;
  if(f1 == f2) return s1 < s2;
  return f1 < f2;
}

void find_maximal_cliques() {
  int p = 0;
  smet::C.clear();

  auto J_s = smet::J;
  auto J_f = smet::J;
  sort(J_s.begin(), J_s.end());
  sort(J_f.begin(), J_f.end(), comp2);

  int s = get<0>(J_s[0]);
  do {
    int f_index = upper_bound(J_f.begin(), J_f.end(), make_tuple(INF, s, 0), comp2) - J_f.begin();
    int f = get<1>(J_f[f_index]);

    smet::C.push_back(vector<int>());
    for(int j = f_index; j < J_f.size(); j++) {
      auto [s1, f1, j1] = J_f[j];
      if(s1 < f) smet::C.back().push_back(j1);
    }

    auto it = lower_bound(J_s.begin(), J_s.end(), make_tuple(f, -INF, 0));
    if(it == J_s.end()) break;
    s = get<0>(*it);
  } while(true);
}

void compute_c_w() {
  smet::C_w.clear();
  smet::C_w.resize(smet::W_size);

  for(int w = 0; w < smet::W_size; w++) {
    for(int k = 0; k < smet::C.size(); k++) {
      bool flag = false;
      for(auto j : smet::C[k]) {
        if(binary_search(smet::T[w].begin(), smet::T[w].end(), j)) {
          if(!flag) {
            smet::C_w[w].push_back(vector<int>());
            flag = true;
          }
          smet::C_w[w].back().push_back(j);
        }
      }
      if(smet::C_w[w].size() and smet::C_w[w].back().size() == 1) smet::C_w[w].pop_back();
    }
  }
}

int main() {
  read_instance();
  find_maximal_cliques();
  compute_c_w();

  // print_instance();

  cout << "VOU CONSTRUIR" << endl;

  Constructive constructive(10);
  constructive.construct();

  cout << "TERMINEI DE CONSTRUIR" << endl;

  auto initial_y = constructive.get_final_y_sol();
  auto initial_x = constructive.get_final_x_sol();

  cout << "OBTIVE A SOLUCAO CONSTRUIDA" << endl;

  int lower_bound = 0;
  for(auto k : smet::C) lower_bound = max(lower_bound, (int) k.size());

  cout << "VOU MELHORAR" << endl;

  Improvement improvement(4.5);
  improvement.improve(lower_bound, initial_x, initial_y);

  cout << "TERMINEI DE MELHORAR" << endl;

  return 0;
}