#include <bits/stdc++.h>
#include <ilcplex/ilocplex.h>
#include "constructive.h"
#include "../NAMESPACE/smet_namespace.h"
#include "../SMPTSP_SUB/smptsp_sub.h"
using namespace std;

Constructive::Constructive(int b_) : b(b_) {
  J = smet::J;
  P = smet::P;
  T = smet::T;
  C = smet::C;
  C_w = smet::C_w;
}


vector<int> Constructive::get_final_y_sol() {
  return final_y_sol;
}


vector<vector<int>> Constructive::get_final_x_sol() {
  return final_x_sol;
}


void Constructive::build_solution(vector<int> &R_j) {
  vector<int> y_sol(smet::W_size);
  vector<vector<int>> x_sol(smet::J_size);

  for(int j = 0; j < smet::J_size; j++) {
    x_sol[j].resize(P[j].size(), 0);
    if(R_j[j] == -1) continue;

    int pos = lower_bound(P[j].begin(), P[j].end(), R_j[j]) - P[j].begin();
    x_sol[j][pos] = 1;
    y_sol[R_j[j]] = 1;
  }

  final_y_sol = y_sol;
  final_x_sol = x_sol;
}


void Constructive::construct() {
  vector<int> W(smet::W_size);
  iota(W.begin(), W.end(), 0);
  random_shuffle(W.begin(), W.end());

  vector<vector<int>> R_w(smet::W_size);
  vector<int> R_j(smet::J_size, -1);

  vector<bool> task_used(smet::J_size, false), worker_used(smet::W_size, false), task_trash(smet::J_size, false);

  int pointer = 0, trash_counter = 0;
  while(pointer < smet::W_size and trash_counter < smet::J_size) {
    map<int, int> dict_task; //[task_og_id, task_new_id]
    map<int, int> dict_worker; //[worker_og_id, worker_new_id]

    map<int, int> dict_task_inv; //[task_new_id, task_og_id]
    map<int, int> dict_worker_inv; //[worker_new_id, worker_og_id]

    // Collect workers and tasks used in subproblem
    for(int i = pointer; i < min(pointer + b, smet::W_size); i++) {
      worker_used[W[i]] = true;
      dict_worker[W[i]] = i - pointer;
      dict_worker_inv[i - pointer] = W[i];

      for(auto j : T[W[i]]) {
        if(task_used[j] or task_trash[j]) continue;

        int tam = dict_task.size();
        dict_task[j] = tam;
        dict_task_inv[tam] = j;

        task_used[j] = true;
      }
    }

    // Build new C_w with only the workers and tasks of subproblem
    vector<vector<vector<int>>> new_C_w(dict_worker.size());
    for(int i = pointer; i < min(pointer + b, smet::W_size); i++) {
      int w = W[i];
      for(auto k : C_w[w]) {
        new_C_w[dict_worker[w]].push_back(vector<int>());

        for(auto j : k) {
          if(task_used[j]) new_C_w[dict_worker[w]].back().push_back(dict_task[j]);
        }

        if(new_C_w[dict_worker[w]].back().size() <= 1) new_C_w[dict_worker[w]].pop_back();
        else sort(new_C_w[dict_worker[w]].back().begin(), new_C_w[dict_worker[w]].back().end());
      }
    }

    // Build new P and D with only the workers and tasks of subproblem
    vector<vector<int>> new_P(dict_task.size());
    vector<int> new_D(dict_task.size());
    for(auto [task, id] : dict_task) {
      new_D[id] = get<1>(smet::J[task]) - get<0>(smet::J[task]);

      for(auto w : P[task]) {
        if(worker_used[w]) new_P[id].push_back(dict_worker[w]);
      }

      sort(new_P[id].begin(), new_P[id].end());
    }




    //////////////////////////////
    // cout << "Conjunto de Workers: " << endl;
    // for(auto [w, id] : dict_worker) cout << w << " -> " << id << endl;

    // cout << endl << "Conjunto de Jobs: " << endl;
    // for(auto [j, id] : dict_task) cout << j << " -> " << id << endl;

    // cout << endl << "P: " << endl;
    // for(auto i : new_P) {
    //   for(auto j : i) cout << j << " ";
    //   cout << endl;
    // }

    // cout << endl << "D: " << endl;
    // for(auto i : new_D) cout << i << ", ";
    // cout << endl;

    // cout << endl << "C_w:" << endl;
    // for(auto w : new_C_w) {
    //   for(auto k : w) {
    //     cout << "{";
    //     for(auto j : k) cout << j << " ";
    //     cout << "}, ";
    //   }
    //   cout << endl;
    // }
    /////////////////////////////



    // Call CPLEX
    SMPTSP_SUB smptsp_sub(dict_task.size(), dict_worker.size(), new_P, new_C_w, new_D);
    auto sol = smptsp_sub.solve();

    // for(auto i : sol) {
    //   for(auto j : i) cout << j << " ";
    //   cout << endl;
    // }

    // Build solution
    for(int j = 0; j < sol.size(); j++) {
      for(int w = 0; w < sol[j].size(); w++) {
        if(sol[j][w] == 1) {
          int real_j = dict_task_inv[j], real_w = dict_worker_inv[new_P[j][w]];
          R_w[real_w].push_back(real_j);
          R_j[real_j] = real_w;

          task_trash[real_j] = true;
          trash_counter++;
          break;
        }
      }
    }

    // Last subproblem
    // if(trash_counter == smet::J_size) {
    //   SMPTSP_SUB smptsp(dict_task.size(), dict_worker.size(), new_P, new_C_w, new_D);
    //   auto alternativa_sol = smptsp.solve();
    // }

    // Reset task_used and worker_used arrays
    for(auto [task, id] : dict_task) task_used[task] = false;
    for(auto [worker, id] : dict_worker) worker_used[worker] = false;

    pointer += b;
    // break;
  }

  // Build final solution
  build_solution(R_j);

  // cout << "Solucao construida:" << endl;
  // cout << "R_w:" << endl;
  // for(int w = 0; w < smet::W_size; w++) {
  //   cout << "W=" << w << ": ";
  //   for(auto j : R_w[w]) cout << j << ", ";
  //   cout << endl;
  // }
  // cout << endl << "R_j:" << endl;
  // for(int j = 0; j < smet::J_size; j++) {
  //   cout << "J=" << j << ": " << R_j[j] << endl;
  // }
}