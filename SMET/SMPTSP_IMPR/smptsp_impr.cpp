#include <ilcplex/ilocplex.h>
#include <bits/stdc++.h>
#include "smptsp_impr.h"
ILOSTLBEGIN

SMPTSP_IMPR::SMPTSP_IMPR(int k_, int J_size_, int W_size_, vector<vector<int>> P_, vector<vector<vector<int>>> C_w_, vector<vector<int>> initial_x_) {
  k = k_;
  J_size = J_size_;
  W_size = W_size_;
  P = P_;
  initial_x = initial_x_;
  C_w = C_w_;
}

void SMPTSP_IMPR::define_problem(IloModel model, const IloArray<IloBoolVarArray> x, const IloBoolVarArray y) {
  IloEnv env = model.getEnv();

  // Objective function
  IloExpr total_cost(env);
  for(int w = 0; w < W_size; w++)
      total_cost += y[w];
  model.add(IloMinimize(env, total_cost));

  env.out() << "Objective done" << endl;

  // Constraint 2
  IloArray<IloExpr> c2(env, J_size);
  for(int j = 0; j < J_size; j++) c2[j] = IloExpr(env);

  for(int j = 0; j < J_size; j++) {
    c2[j] = x[j][0]*0;
    for(int w = 0; w < P[j].size(); w++) c2[j] += x[j][w];
    model.add(c2[j] == 1);
  }

  env.out() << "C2 done" << endl;

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

  env.out() << "C3 done" << endl;

  // Constraint 7
  IloExpr c7(env);
  c7 = x[0][0]*0;
  for(int j = 0; j < J_size; j++) {
    for(int w = 0; w < P[j].size(); w++) {
      c7 += (initial_x[j][w] * (1 - x[j][w]));
    }
  }
  model.add(c7 <= k);

  env.out() << "C7 done" << endl;  

} // END define_problem


void SMPTSP_IMPR::print_solution(IloModel &model, IloCplex &cplex, const IloArray<IloBoolVarArray> &var_x, const IloBoolVarArray &var_y) {
  IloEnv env = model.getEnv();

  IloNumArray vals_x(env), vals_y(env);
  env.out() << "Solution status = " << fixed << setprecision(3) << cplex.getStatus() << endl;
  env.out() << "Solution value  = " << cplex.getObjValue() << endl;

  bool full = false;
  if(!full) return;

  cplex.getValues(vals_y, var_y);
  env.out() << "Y:" << vals_y;

  env.out() << endl << "X:" << endl;
  for(int j = 0; j < J_size; j++) {
    cplex.getValues(vals_x, var_x[j]);
    env.out() << "j = " << j << ": " << vals_x << endl;
  }
}


pair<vector<int>, vector<vector<int>>> SMPTSP_IMPR::solve() {
  cout << "main" << endl;
  IloEnv env;
  try {
    // Create model
    IloModel model(env);
    cout << "model" << endl;

    // Read data
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

    // Return solution
    vector<vector<int>> x_vector(J_size);
    vector<int> y_vector(W_size);
    IloNumArray vals_x(env), vals_y(env);

    cplex.getValues(vals_y, var_y);
    for(int w = 0; w < W_size; w++) {
      y_vector[w] = (int) vals_y[w];
    }

    for(int j = 0; j < J_size; j++) {
      cplex.getValues(vals_x, var_x[j]);
      for(int w = 0; w < vals_x.getSize(); w++) {
        x_vector[j].push_back((int) vals_x[w]);
      }
    }

    env.end();
    return {y_vector, x_vector};
  }
  catch (IloException& e) {
    cerr << "Concert exception caught: " << e << endl;
    throw -1;
  }
  catch (...) {
    cerr << "Unknown exception caught" << endl;
    throw -1;
  }
}