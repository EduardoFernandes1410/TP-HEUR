#include <bits/stdc++.h>
#include "NAMESPACE/aco_namespace.h"
#include "utils.cpp"
#include "aco.cpp"
using namespace std;


int main(int argc, char **argv){
    if(argc != 6) {
        cerr << "Uso do programa: " << argv[0] << " <popsize> <rho> <delta> <alpha> <beta>" << endl;
        exit(-1);
    }
    read_instance();
    // cout << "J_size: " << aco::J_size << endl;
    // cout << "W_size: " << aco::W_size << endl;
    check_conflicts(aco::conflicts);

    // aco_heuristic -    POP SIZE | RHO | DELTA | ALPHA | BETA | LIMITER
    vector<int> bestsol = aco_heuristic(stoi(argv[1]), stod(argv[2]), stod(argv[3]), stod(argv[4]), stod(argv[5]), 50);
    // cout << "best: "; printv(bestsol);
    cout << "cost: " << solution_cost(bestsol) << endl;

    bool feasible = true;
    for(auto n : bestsol){
        if(n >= aco::W_size)
            feasible = false;
    }
    cout << "Viavel: " << feasible << endl;
    return 0;
}