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


// pair<vector<int>, vector<vector<int>>> Constructive::construct() {
void Constructive::construct() {
  cout << "W: " << smet::W_size << endl;
  vector<int> W(smet::W_size);
  iota(W.begin(), W.end(), 0);
  random_shuffle(W.begin(), W.end());

  vector<bool> task_used(smet::J_size, false), worker_used(smet::W_size, false), task_trash(smet::J_size, false);

  int pointer = 0, trash_counter = 0;
  while(pointer < smet::W_size and trash_counter < smet::J_size) {
    map<int, int> dict_task; //[task_og_id, task_new_id]
    map<int, int> dict_worker; //[worker_og_id, worker_new_id]

    // Collect workers and tasks used in subproblem
    for(int i = pointer; i < min(pointer + b, smet::W_size); i++) {
      worker_used[W[i]] = true;
      dict_worker[W[i]] = i - pointer;

      for(auto j : T[W[i]]) {
        if(task_trash[j]) continue;

        int tam = dict_task.size();
        dict_task[j] = tam;

        task_used[j] = true;
        task_trash[j] = true;
        trash_counter++;
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
    cout << "Conjunto de Workers: " << endl;
    for(auto [w, id] : dict_worker) cout << w << " -> " << id << endl;

    cout << endl << "Conjunto de Jobs: " << endl;
    for(auto [j, id] : dict_task) cout << j << " -> " << id << endl;

    cout << endl << "P: " << endl;
    for(auto i : new_P) {
      for(auto j : i) cout << j << " ";
      cout << endl;
    }

    cout << endl << "D: " << endl;
    for(auto i : new_D) cout << i << ", ";
    cout << endl;

    cout << endl << "C_w:" << endl;
    for(auto w : new_C_w) {
      for(auto k : w) {
        cout << "{";
        for(auto j : k) cout << j << " ";
        cout << "}, ";
      }
      cout << endl;
    }
    /////////////////////////////



    // Call CPLEX
    SMPTSP_SUB smptsp(dict_task.size(), dict_worker.size(), new_P, new_C_w, new_D);
    smptsp.solve();


    // Reset task_used and worker_used arrays
    for(auto [task, id] : dict_task) task_used[task] = false;
    for(auto [worker, id] : dict_worker) worker_used[worker] = false;

    pointer += b;
    // break;
  }
}