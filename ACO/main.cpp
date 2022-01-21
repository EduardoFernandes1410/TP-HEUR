#include <bits/stdc++.h>
#include "NAMESPACE/aco_namespace.h"
#include "utils.cpp"
#include "aco.cpp"
using namespace std;


int main(){
    read_instance();
    cout << "J_size: " << aco::J_size << endl;
    cout << "W_size: " << aco::W_size << endl;
    check_conflicts(aco::conflicts);

    // aco_heuristic -    POP SIZE | RHO | DELTA | ALPHA | BETA | LIMITER
    vector<int> bestsol = aco_heuristic(500, 0.2, 0.6, 0.8, 0.2, 100);
    cout << "best: "; printv(bestsol);
    cout << "cost: " << solution_cost(bestsol) << endl;
    return 0;
}