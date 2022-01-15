#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

typedef IloArray<IloIntArray> IntMatrix;

// Data
IloInt J_size, W_size;
IloArray<IloNumArray> P;
IloArray<IloArray<IloNumArray>> C_w;

void read_data(IloEnv env, string file_path) {
  ifstream f(file_path, ios_base::in);
  if(!f) {
    cerr << "Data file not found!" << endl;
    exit(0);
  }

  P = IloArray<IloNumArray>(env);
  C_w = IloArray<IloNumArray>(env);

  f >> J_size;
  f >> W_size;
  f >> P;
  f >> C_w;
} // END read_data

void usage (const char *progname) {
  cerr << "Usage: " << progname << " <path to data file (OR-CPLEX)>" << endl;
  cerr << " Exiting..." << endl;
} // END usage

void define_problem(IloModel model, const IloArray<IloBoolVarArray> x, const IloBoolVarArray y) {
  IloEnv env = model.getEnv();

  // Objective function
  IloExpr total_cost(env);
  for(int w = 0; w < smet::W_size; w++)
      total_cost += y[w];
  model.add(IloMinimize(env, total_cost));

  // Constraint 2
  IloArray<IloExpr> c2(env, smet::J_size);
  for(int j = 0; j < smet::J_size; j++) c2[j] = IloExpr(env);

  for(int j = 0; j < smet::J_size; j++) {
    c2[j] = x[j][0]*0;
    for(int w = 0; w < smet::P[j].size(); w++) c2[j] += x[j][w];
    model.add(c2[j] == 1);
  }

  // Constraint 3
  IloArray<IloExprArray> c3(env, smet::W_size);
  for(int w = 0; w < smet::W_size; w++) c3[w] = IloExprArray(env, smet::C_w[w].size());

  for(int w = 0; w < smet::W_size; w++) {
    for(int k = 0; k < smet::C_w[w].size(); k++) {
      c3[w][k] = x[w][0]*0;
      for(auto j : smet::C_w[w][k]){
        int pos = lower_bound(smet::P[j].begin(), smet::P[j].end(), w) - smet::P[j].begin();
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
  for(int j = 0; j < smet::J_size; j++) {
    cplex.getValues(vals_x, var_x[j]);
    env.out() << "j = " << j << ": " << vals_x << endl;
  }
}


int main (int argc, char **argv) {
  cout << "main" << endl;
  IloEnv env;
  try {
    // Create model
    IloModel model(env);
    cout << "model" << endl;

    // Check parameter of program
    if (( argc != 2 )) {
      usage (argv[0]);
      throw(-1);
    }

    // Read data
    string file_path(argv[1]);
    read_data(env, file_path);
    cout << "data " << J_size << " " << W_size << endl;

    // Create variables
    IloArray<IloBoolVarArray> var_x(env, J_size);
    IloBoolVarArray var_y(env, W_size);
    for(int j = 0; j < J_size; j++) var_x[j] = IloBoolVarArray(env, W);
    cout << "vars" << endl;

    // Declare cplex object
    IloCplex cplex(model);

    // Define model
    env.out() << "gonna read" << endl;
    define_problem(model, var_x, var_y);

    // Export model
    cplex.exportModel("smptsp.lp");

    // Define time and memory restrictions
    // 7200 seconds of execution time
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
