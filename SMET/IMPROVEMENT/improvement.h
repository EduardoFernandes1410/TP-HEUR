#ifndef IMPROVE_H
#define IMPROVE_H

using namespace std;

class Improvement {
  private:
    double f;
    vector<tuple<int, int, int>> J; //[s_j, f_j, j] - tasks data
    vector<vector<int>> P; // set of workers who can perform task
    vector<vector<int>> T; // set of tasks which can be performed by worker
    vector<vector<int>> C; // set of maximal cliques in interval graph
    vector<vector<vector<int>>> C_w; // set of maximal cliques per worker
    vector<vector<int>> initial_x;
    vector<int> initial_y;

  public:
    Improvement(double f_);
    int evaluate(vector<int> &y);
    void improve(int lower_bound, vector<vector<int>> initial_x, vector<int> initial_y); //[y_w, x_jw]
};

#endif