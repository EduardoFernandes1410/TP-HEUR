#include <bits/stdc++.h>
#include "smet_namespace.h"
using namespace std;

int smet::J_size, smet::W_size;
vector<tuple<int, int, int>> smet::J; //[s_j, f_j, j] - tasks data
vector<vector<int>> smet::P; // set of workers who can perform task
vector<vector<int>> smet::T; // set of tasks which can be performed by worker
vector<vector<int>> smet::C; // set of maximal cliques in interval graph
vector<vector<vector<int>>> smet::C_w; // set of maximal cliques per worker