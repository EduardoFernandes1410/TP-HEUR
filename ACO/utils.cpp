#ifndef _NAMESPACE
#define _NAMESPACE

#include <bits/stdc++.h>
#include "NAMESPACE/aco_namespace.h"
using namespace std;

const int INF = 0x3f3f3f3f;

void read_instance() {
  char trash;
  string input;
  getline(cin, input);
  getline(cin, input);
  getline(cin, input);
  getline(cin, input);

  cin >> input >> input >> aco::J_size;

  aco::J.resize(aco::J_size);
  aco::P.resize(aco::J_size);
  for(int i = 0; i < aco::J_size; i++) {
    int s, f; cin >> s >> f;
    aco::J[i] = {s, f, i};
  }

  cin >> input >> input >> aco::W_size;

  aco::T.resize(aco::W_size);
  for(int i = 0; i < aco::W_size; i++) {
    int num_tasks; cin >> num_tasks >> trash;
  
    aco::T[i].resize(num_tasks);
  
    for(int j = 0; j < num_tasks; j++) {
      cin >> aco::T[i][j];
      aco::P[aco::T[i][j]].push_back(i);
    }

    sort(aco::T[i].begin(), aco::T[i].end());
    }

    aco::b = vector<int>(aco::W_size, 1); // cost 1 for all workers
    for(int i = 0; i < aco::J_size; i++){
        aco::b.push_back(10);
        aco::P[i].push_back(aco::W_size + i);
        aco::T.push_back({i});
    }


}

bool compare_times(const tuple<int, int, int> &a, const tuple<int, int, int> &b) {
    if( get<0>(a) == get<0>(b))
        return get<2>(a) < get<2>(b);
    return get<0>(a) < get<0>(b);
}

int task_length(vector<int> &tasks){
    int acum = 0;
    for(auto idx : tasks){
        acum += get<1>(aco::J[idx]) - get<0>(aco::J[idx]);
    }
    return acum;
}

void printv(vector<double> &v){
    for(auto n:v){
        cout << n << " ";
    }
    cout << endl;
}

void printv(vector<int> &v){
    for(auto n:v){
        cout << n << " ";
    }
    cout << endl;
}

int solution_cost(vector<int> &sol){
    set<int> workers;
    int cost = 0;
    for(int i = 0; i < sol.size(); i++){
        if(workers.find(sol[i]) == workers.end()){
            workers.insert(sol[i]);
            cost += aco::b[sol[i]];
        }
    }
    return cost;
}

// SOMA DOIS VETORES
vector<double> sum_up(vector<double> &a, vector<double> &b){
    vector<double> res(a.size());
    for(int i = 0; i < a.size(); i++){
        res[i] = a[i] + b[i];
    }
    return res;
}

#endif