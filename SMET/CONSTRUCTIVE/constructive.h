#ifndef CONSTRUCT_H
#define CONSTRUCT_H

using namespace std;

class Constructive {
  private:
    int b;
    vector<tuple<int, int, int>> J; //[s_j, f_j, j] - tasks data
    vector<vector<int>> P; // set of workers who can perform task
    vector<vector<int>> T; // set of tasks which can be performed by worker
    vector<vector<int>> C; // set of maximal cliques in interval graph
    vector<vector<vector<int>>> C_w; // set of maximal cliques per worker
    vector<int> final_y_sol;
    vector<vector<int>> final_x_sol;

  public:
    Constructive(int b_);
    void build_solution(vector<int> &R_j);
    void construct(); //[y_w, x_jw]
    vector<int> get_final_y_sol();
    vector<vector<int>> get_final_x_sol();
};

#endif