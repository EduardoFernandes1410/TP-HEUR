#ifndef SMPTSP_H
#define SMPTSP_H

#include <ilcplex/ilocplex.h>
// #include "NAMESPACE/smet_namespace.h"
ILOSTLBEGIN

typedef IloArray<IloIntArray> IntMatrix;

// Data
int J_size, W_size;
vector<vector<int>> P;
vector<vector<vector<int>>> C_w;

void define_problem(IloModel model, const IloArray<IloBoolVarArray> x, const IloBoolVarArray y) {
  IloEnv env = model.getEnv();

  // Objective function
  IloExpr total_cost(env);
  for(int w = 0; w < W_size; w++)
      total_cost += y[w];
  model.add(IloMinimize(env, total_cost));

  // Constraint 2
  IloArray<IloExpr> c2(env, J_size);
  for(int j = 0; j < J_size; j++) c2[j] = IloExpr(env);

  for(int j = 0; j < J_size; j++) {
    c2[j] = x[j][0]*0;
    for(int w = 0; w < P[j].size(); w++) c2[j] += x[j][w];
    model.add(c2[j] == 1);
  }

  // Constraint 3
  IloArray<IloExprArray> c3(env, W_size);
  for(int w = 0; w < W_size; w++) c3[w] = IloExprArray(env, C_w[w].size());

  for(int w = 0; w < W_size; w++) {
    for(int k = 0; k < C_w[w].size(); k++) {
      c3[w][k] = x[w][0]*0;
      for(auto j : C_w[w][k]){
        int pos = lower_bound(P[j].begin(), P[j].end(), w) - P[j].begin();
        c3[w][k] += x[j][pos];
      }
      model.add(c3[w][k] <= y[w]);
    }
  }
} // END define_problem


void print_solution(IloModel &model, IloCplex &cplex, const IloArray<IloBoolVarArray> &var_x, const IloBoolVarArray &var_y, bool full=false) {
  IloEnv env = model.getEnv();

  IloNumArray vals_x(env), vals_y(env);
  env.out() << "Solution status = " << fixed << setprecision(3) << cplex.getStatus() << endl;
  env.out() << "Solution value  = " << cplex.getObjValue() << endl;
  if(!full) return;

  cplex.getValues(vals_y, var_y);
  env.out() << "Y:" << vals_y;

  env.out() << endl << "X:" << endl;
  for(int j = 0; j < J_size; j++) {
    cplex.getValues(vals_x, var_x[j]);
    env.out() << "j = " << j << ": " << vals_x << endl;
  }
}


int smptsp(int J_size_, int W_size_, vector<vector<int>> P_, vector<vector<vector<int>>> C_w_) {
  cout << "main" << endl;
  IloEnv env;
  try {
    // Create model
    IloModel model(env);
    cout << "model" << endl;

    // Read data
    J_size = J_size_;
    W_size = W_size_;
    P = P_;
    C_w = C_w_;
    cout << "data " << J_size << " " << W_size << endl;

    // Create variables
    IloArray<IloBoolVarArray> var_x(env, J_size);
    IloBoolVarArray var_y(env, W_size);
    for(int j = 0; j < J_size; j++) var_x[j] = IloBoolVarArray(env, P[j].size());
    cout << "vars" << endl;

    // Declare cplex object
    IloCplex cplex(model);

    // Define model
    env.out() << "gonna read" << endl;
    define_problem(model, var_x, var_y);

    // Export model
    cplex.exportModel("smptsp.lp");

    // Define time and memory restrictions
    // 100 seconds of execution time
    cplex.setParam(IloCplex::Param::TimeLimit, 100);
    // 4GB of tree memory
    cplex.setParam(IloCplex::Param::MIP::Limits::TreeMemory, 4096);

    // Optimize the problem and obtain solution.
    env.out() << "gonna start" << endl;
    if ( !cplex.solve() ) {
      env.error() << "Failed to optimize LP" << endl;
      throw(-1);
    }

    print_solution(model, cplex, var_x, var_y);    
  }
  catch (IloException& e) {
    cerr << "Concert exception caught: " << e << endl;
  }
  catch (...) {
    cerr << "Unknown exception caught" << endl;
  }

  env.end();

  return 0;
} // END main

#endif