#ifndef SMET_NAMESPACE
#define SMET_NAMESPACE

using namespace std;

namespace smet {
  extern int J_size, W_size;
  extern vector<tuple<int, int, int>> J; //[s_j, f_j, j] - tasks data
  extern vector<vector<int>> P; // set of workers who can perform task
  extern vector<vector<int>> T; // set of tasks which can be performed by worker
  extern vector<vector<int>> C; // set of maximal cliques in interval graph
  extern vector<vector<vector<int>>> C_w; // set of maximal cliques per worker
}

#endif