#include <bits/stdc++.h>
#include "aco_namespace.h"
using namespace std;

int aco::J_size, aco::W_size;
vector<tuple<int, int, int>> aco::J; //[s_j, f_j, j] - tasks data
vector<vector<int>> aco::P; // set of workers who can perform task
vector<vector<int>> aco::T; // set of tasks which can be performed by worker
vector<vector<int>> aco::C; // set of maximal cliques in interval graph
vector<vector<vector<int>>> aco::C_w; // set of maximal cliques per worker
vector<int> aco::b; // cost of each worker
vector<vector<bool> > aco::conflicts; // conflicts between tasks