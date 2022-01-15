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

  public:
    Constructive(int b_);
    // pair<vector<int>, vector<vector<int>>> construct(); //[y_w, x_jw]
    void construct(); //[y_w, x_jw]
};

#endif