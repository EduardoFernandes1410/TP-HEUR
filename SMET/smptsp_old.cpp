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
  for(int w = 0; w < W_size; w++)
      total_cost += y[w];
  model.add(IloMinimize(env, total_cost));

  // Constraint 2
  IloArray<IloExpr> c2(env, J_size);
  for(int j = 0; j < J_size; j++) c2[j] = IloExpr(env);

  for(int j = 0; j < J_size; j++) {
    c2[j] = x[j][0]*0;
    for(int w = 0; w < P[j].size(); w++) c2[j] += x[j][P[j][w]];
    model.add(c2[j] = 1);
  }

  // Constraint 3
  IloArray<IloExprArray> c3(env, W_size);
  for(int w = 0; w < W_size; w++) c3[w] = IloExprArray(env, C_w[w].size());

  for(int w = 0; w < W_size; w++) {
    for(int k = 0; k < C_w[w].size(); k++) {
      c3[w][k] = x[w][0]*0;
      for(int j = 0; j < C_w[w][k]; j++) c3[w][k] += x[j][w];
      model.add(c3[w][k] <= y[w]);
    }
  }
} // END define_problem


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

    // Print results
    IloNumArray vals_x(env), vals_y(env);
    env.out() << "Solution status = " << fixed << setprecision(3) << cplex.getStatus() << endl;
    env.out() << "Solution value  = " << cplex.getObjValue() << endl;
    // env.out() << cplex.getObjValue() << endl;
    // cplex.getValues(vals_y, var_y);
    // cplex.getValues(vals_u, var_u);
    cplex.getValues(vals_y, var_y);
    env.out() << "Y:" << vals_y;

    env.out() << endl << "X:" << endl;
    for(int j = 0; j < J_size; j++) {
      cplex.getValues(vals_x, var_x[j]);
      env.out() << "j = " << j << ": " << vals_x << endl;
    }

    // Write solution in file
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
