#ifndef SUB_H
#define SUB_H

using namespace std;

class SMPTSP_SUB {
  private:
    int J_size, W_size;
    vector<vector<int>> P;
    vector<vector<vector<int>>> C_w;
    vector<int> D;

  public:
    SMPTSP_SUB(int J_size_, int W_size_, vector<vector<int>> P_, vector<vector<vector<int>>> C_w_, vector<int> D_);
    void define_problem(IloModel model, const IloArray<IloBoolVarArray> x, const IloBoolVarArray y);
    void print_solution(IloModel &model, IloCplex &cplex, const IloArray<IloBoolVarArray> &var_x, const IloBoolVarArray &var_y);
    int solve();
};

#endif