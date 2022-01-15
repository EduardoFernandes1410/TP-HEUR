#include <bits/stdc++.h>
#include <ilcplex/ilocplex.h>
#include "improvement.h"
#include "../NAMESPACE/smet_namespace.h"
#include "../SMPTSP_IMPR/smptsp_impr.h"
using namespace std;

Improvement::Improvement(double f_) : f(f_) {
  J = smet::J;
  P = smet::P;
  T = smet::T;
  C = smet::C;
  C_w = smet::C_w;
}


int Improvement::evaluate(vector<int> &y) {
  return accumulate(y.begin(), y.end(), 0);
}


// pair<vector<int>, vector<vector<int>>> Constructive::construct() {
void Improvement::improve(int lower_bound, vector<vector<int>> initial_x, vector<int> initial_y) {
  int k = (int) (f * sqrt(smet::J_size));
  int F_x_line = evaluate(initial_y);

  while(F_x_line != lower_bound) {
    SMPTSP_IMPR smptsp_impr(k, smet::J_size, smet::W_size, P, C_w, initial_x);
    try {
      auto [y_sol, x_sol] = smptsp_impr.solve();

      int F_x = evaluate(y_sol);
      if(F_x < F_x_line) {
        F_x_line = F_x;
        initial_x = x_sol;
        initial_y = y_sol;
      } else {
        break;
      }
    } catch(int e) {
      cout << "Not feasible" << endl;
      k++;
    }
  }




  cout << "Solucao final:" << endl;
  cout << "Valor objetivo: " << F_x_line << endl;
  // cout << "Y:" << endl;
  // for(int w = 0; w < smet::W_size; w++) {
  //   cout << "W=" << w << ": " << initial_y[w] << endl;
  // }

  // cout << endl << "X:" << endl;
  // for(int j = 0; j < smet::J_size; j++) {
  //   cout << "J=" << j << ": ";
  //   for(auto w : initial_x[j]) cout << w << " ";
  //   cout << endl;
  // }
}