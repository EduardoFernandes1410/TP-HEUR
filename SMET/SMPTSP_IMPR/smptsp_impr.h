#ifndef IMPR_H
#define IMPR_H

using namespace std;

class SMPTSP_IMPR {
  private:
    int k, J_size, W_size;
    vector<vector<int>> P, initial_x;
    vector<vector<vector<int>>> C_w;

  public:
    SMPTSP_IMPR(int k_, int J_size_, int W_size_, vector<vector<int>> P_, vector<vector<vector<int>>> C_w_, vector<vector<int>> initial_x_);
    void define_problem(IloModel model, const IloArray<IloBoolVarArray> x, const IloBoolVarArray y);
    void print_solution(IloModel &model, IloCplex &cplex, const IloArray<IloBoolVarArray> &var_x, const IloBoolVarArray &var_y);
    pair<vector<int>, vector<vector<int>>> solve();
};

#endif