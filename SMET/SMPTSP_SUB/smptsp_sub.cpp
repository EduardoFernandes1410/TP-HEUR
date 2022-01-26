#include <ilcplex/ilocplex.h>
#include <bits/stdc++.h>
#include "smptsp_sub.h"
ILOSTLBEGIN

SMPTSP_SUB::SMPTSP_SUB(int J_size_, int W_size_, vector<vector<int>> P_, vector<vector<vector<int>>> C_w_, vector<int> D_) {
  J_size = J_size_;
  W_size = W_size_;
  P = P_;
  C_w = C_w_;
  D = D_;
}

void SMPTSP_SUB::define_problem(IloModel model, const IloArray<IloBoolVarArray> x, const IloBoolVarArray y) {
  IloEnv env = model.getEnv();

  // Objective function
  IloExpr total_cost(env);
  for(int j = 0; j < J_size; j++)
    for(int w = 0; w < P[j].size(); w++) 
      total_cost += (x[j][w] * D[j]);
  model.add(IloMaximize(env, total_cost));

  // env.out() << "Objective done" << endl;

  // Constraint 2
  IloArray<IloExpr> c2(env, J_size);
  for(int j = 0; j < J_size; j++) c2[j] = IloExpr(env);

  for(int j = 0; j < J_size; j++) {
    c2[j] = x[j][0]*0;
    for(int w = 0; w < P[j].size(); w++) c2[j] += x[j][w];
    model.add(c2[j] <= 1);
  }

  // env.out() << "C2 done" << endl;

  // Constraint 3
  IloArray<IloExprArray> c3(env, W_size);
  for(int w = 0; w < W_size; w++) c3[w] = IloExprArray(env, C_w[w].size());

  for(int w = 0; w < W_size; w++) {
    for(int k = 0; k < C_w[w].size(); k++) {
      c3[w][k] = x[0][0]*0;
      for(auto j : C_w[w][k]){
        int pos = lower_bound(P[j].begin(), P[j].end(), w) - P[j].begin();
        c3[w][k] += x[j][pos];
      }
      model.add(c3[w][k] <= y[w]);
    }
  }

  // env.out() << "C3 done" << endl;
} // END define_problem


void SMPTSP_SUB::print_solution(IloModel &model, IloCplex &cplex, const IloArray<IloBoolVarArray> &var_x, const IloBoolVarArray &var_y) {
  IloEnv env = model.getEnv();

  IloNumArray vals_x(env);
  env.out() << "Solution status = " << fixed << setprecision(3) << cplex.getStatus() << endl;
  env.out() << "Solution value  = " << cplex.getObjValue() << endl;

  bool full = false;
  if(!full) return;

  env.out() << endl << "X:" << endl;
  for(int j = 0; j < J_size; j++) {
    cplex.getValues(vals_x, var_x[j]);
    env.out() << "j = " << j << ": " << vals_x << endl;
  }
}


vector<vector<int>> SMPTSP_SUB::solve() {
  // cout << "main" << endl;
  IloEnv env;
  try {
    // Create model
    IloModel model(env);
    // cout << "model" << endl;

    // Read data
    cout << "data " << J_size << " " << W_size << endl;

    // Create variables
    IloArray<IloBoolVarArray> var_x(env, J_size);
    IloBoolVarArray var_y(env, W_size);
    for(int j = 0; j < J_size; j++) var_x[j] = IloBoolVarArray(env, P[j].size());
    // cout << "vars" << endl;

    // Declare cplex object
    IloCplex cplex(model);
    // Make it not log
    cplex.setOut(env.getNullStream());

    // Define model
    // env.out() << "gonna read" << endl;
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

    // Return solution
    vector<vector<int>> x_vector(J_size);
    IloNumArray vals_x(env);

    for(int j = 0; j < J_size; j++) {
      cplex.getValues(vals_x, var_x[j]);
      for(int w = 0; w < vals_x.getSize(); w++) {
        x_vector[j].push_back((int) vals_x[w]);
      }
    }

    env.end();
    return x_vector;
  }
  catch (IloException& e) {
    cerr << "Concert exception caught: " << e << endl;
  }
  catch (...) {
    cerr << "Unknown exception caught" << endl;
  }

  return vector<vector<int>>();
}