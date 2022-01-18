#include <bits/stdc++.h>
#include "NAMESPACE/aco_namespace.h"
using namespace std;

const int INF = 0x3f3f3f3f;

void read_instance() {
  char trash;
  string input;
  getline(cin, input);
  getline(cin, input);
  getline(cin, input);
  getline(cin, input);

  cin >> input >> input >> aco::J_size;

  aco::J.resize(aco::J_size);
  aco::P.resize(aco::J_size);
  for(int i = 0; i < aco::J_size; i++) {
    int s, f; cin >> s >> f;
    aco::J[i] = {s, f, i};
  }

  cin >> input >> input >> aco::W_size;

  aco::T.resize(aco::W_size);
  for(int i = 0; i < aco::W_size; i++) {
    int num_tasks; cin >> num_tasks >> trash;
  
    aco::T[i].resize(num_tasks);
  
    for(int j = 0; j < num_tasks; j++) {
      cin >> aco::T[i][j];
      aco::P[aco::T[i][j]].push_back(i);
    }

    sort(aco::T[i].begin(), aco::T[i].end());
    }

    aco::b = vector<int>(aco::W_size, 1); // cost 1 for all workers
    for(int i = 0; i < aco::J_size; i++){
        aco::b.push_back(INF);
        aco::P[i].push_back(aco::W_size + i);
        aco::T.push_back({i});
    }


}

bool compare_times(const tuple<int, int, int> &a, const tuple<int, int, int> &b) {
    if( get<0>(a) == get<0>(b))
        return get<2>(a) < get<2>(b);
    return get<0>(a) < get<0>(b);
}

int task_length(vector<int> &tasks){
    int acum = 0;
    for(auto idx : tasks){
        acum += get<1>(aco::J[idx]) - get<0>(aco::J[idx]);
    }
    return acum;
}

int sample_worker(vector<double> &p){
    random_device rd;
    default_random_engine generator(rd());
    uniform_real_distribution<double> distribution(0, 1);
    double r = distribution(generator);
    double acum = 0;
    for(int i = 0; i < p.size(); i++){
        acum += p[i];
        if(r <= acum){
            return i;
        }
    }
    return 0;
}



// void normalize(vector<double> &p, double target){
//     double sum = 0;
//     double vmax = -INF;
//     for(auto n:p){
//         sum += n;
//         vmax = max(vmax, n);
//     }
//     if (sum == 0) return;
//     for(auto &n:p){
//         n /= vmax;
//         n *= target;
//     }
// }

vector<double> sum_up(vector<double> &a, vector<double> &b){
    vector<double> res(a.size());
    for(int i = 0; i < a.size(); i++){
        res[i] = a[i] + b[i];
    }
    return res;
}

void printv(vector<double> &v){
    for(auto n:v){
        cout << n << " ";
    }
    cout << endl;
}
void printv(vector<int> &v){
    for(auto n:v){
        cout << n << " ";
    }
    cout << endl;
}


void normalize(vector<double> &p){
    double sum = 0;
    for(auto n:p){
        sum += n;
    }
    if (sum == 0) return;
    for(auto &n:p){
        n /= sum;
    }
}

vector<int> build_wline(int i, vector<vector<int> > &ws){
    vector<int> res;
    for(auto w:aco::P[i]){
        bool overlap = false;
        for(auto tasks : ws[w]){
            if(aco::conflicts[tasks][i]){
                overlap = true;
                break;
            }
        }
        if(!overlap){
            res.push_back(w);
        }
    }
    return res;
}



vector<int>  greedy_random_heuristic(double alpha, double beta, vector<vector<double> > &tau) {
    vector<int>  sol(aco::J_size);

    sort(aco::J.begin(), aco::J.end(),compare_times );
    vector<vector<int> > S; // solutions set
    vector<vector<int > > ws(aco::W_size+aco::J_size, vector<int>()); // workers tasks set
    // cout << "J_size: " << aco::J_size << endl;
    // cout << "W_size: " << aco::W_size << endl;


    // tarefas que ainda podem ser feitas por cada worker
    vector<double> freq(aco::W_size+aco::J_size, 1/INF);
    for(int i = 0; i < aco::W_size+aco::J_size; i++){
        freq[i] = aco::T[i].size();
    } 


    for(int i = 0; i < aco::J_size; i++) {
        // cout << "ws: " << endl;
        // for(auto w:ws){
        //     printv(w);
        // }
        // cout << "--------" << endl;


        vector<int> wline = build_wline(i, ws); // trabalhadores que podem fazer J_i de boa

        // cout << "workers habilitados para a tarefa " << i << ": ";
        // for(auto n : wline){
        //     cout << n << " ";
        // }
        // cout << endl;

        vector<double> eta1(wline.size());
        vector<double> eta2(wline.size());
        vector<double> tauline(wline.size());
        // TODO: talvez o peso do worker invalido ainda seja muito alto
        for(int e = 0; e < wline.size(); e++) {
            eta1[e] = task_length(ws[wline[e]]) ;
            eta2[e] = freq[wline[e]];
            tauline[e] = tau[i][e];
        }


        // printv(eta1);
        // printv(eta2);
        normalize(eta1);
        normalize(eta2);
        normalize(tauline);
        // cout << "eta1: ";printv(eta1);
        // cout << "eta2: ";printv(eta2);
        vector<double> eta = sum_up(eta1, eta2);
        normalize(eta);
        // printv(eta);




        double total = 0;
        for(int k = 0; k < wline.size(); k++) {
            total += pow(tauline[k], alpha) * pow(eta[k], beta);
        }

        vector<double> p (wline.size());
        for(int e = 0; e < wline.size(); e++) {
            p[e] = (pow(tauline[e], alpha) * pow(eta[e], beta) / total) * (1/aco::b[wline[e]]);
        }
        normalize(p);
        // printv(p);

        int e = wline[sample_worker(p)];
        // cout << "escolhido: " << e << endl;
        for(int k = 0; k < aco::P[i].size(); k++ ){
            freq[aco::P[i][k]]--;
        }
        ws[e].push_back(i);
        sol[i] = e;
    }
    return sol;
}

void check_conflicts(vector<vector<bool> > &mat){
    mat = vector<vector<bool> >(aco::J_size, vector<bool>(aco::J_size, false));
    for(int i = 0; i < aco::J_size; i++){
        for(int j = 0; j < aco::J_size; j++){
            if(get<0> (aco::J[i]) <= get<1>(aco::J[j]) && get<1>(aco::J[i]) >= get<0>(aco::J[j])){
                mat[i][j] = true;
            }
        }
    }
}

int solution_cost(vector<int> &sol){
    set<int> workers;
    int cost = 0;
    for(int i = 0; i < sol.size(); i++){
        if(workers.find(sol[i]) == workers.end()){
            workers.insert(sol[i]);
            cost += aco::b[sol[i]];
        }
    }
    return workers.size();
}

void update_tau(vector<vector<double> > &tau, double rho){
    for(int i = 0; i < aco::J_size; i++){
        for(int j = 0; j < aco::J_size+aco::W_size; j++){
            tau[i][j] = tau[i][j] *(1-rho);
        }
    }
}

vector<int> aco_heuristic(int tam, double rho,double delta, double alpha, double beta, int limiter){
    double stop = false;
    vector<int> sol(aco::J_size);

    vector<vector<double> > tau(aco::J_size, vector<double>(aco::W_size+aco::J_size, 1));

    int non_improved = 0;

    // vector<vector<int> > population(tam, vector<int>(aco::J_size));
    vector<int> temp_sol(aco::J_size);
    int best_sol_cost = INF;

    while(not stop){
        int bestpop_sol_cost = INF;
        vector<int> bestpop_sol = temp_sol;
        for(int i = 0; i < tam; i++){
            temp_sol=  greedy_random_heuristic(alpha, beta, tau); // generating population
            int sol_cost = solution_cost(temp_sol);
            if (sol_cost < bestpop_sol_cost){
                bestpop_sol_cost = sol_cost;
                bestpop_sol = temp_sol;
            }
        }
        update_tau(tau, rho);

        for(int j = 0; j < aco::J_size; j++){
            tau[j][bestpop_sol[j]] += delta;
        }

        if (bestpop_sol_cost < best_sol_cost){
            best_sol_cost = bestpop_sol_cost;
            sol = bestpop_sol;
            non_improved = 0;
            cout << "melhorou..." << endl;
        }else{
            non_improved++;
        }

        if (non_improved > limiter){
            stop = true;
        }
    }
    return sol;
}


int main(){
    read_instance();

    // cout << "quem pode fazer oq: " << endl;
    // for(int i = 0; i < aco::W_size;i++){
    //     cout << i << ": ";
    //     for(auto n:aco::T[i]){
    //         cout << n << " ";
    //     }
    //     cout << endl;
    // }

    // cout << "oq pode ser feito por quem: " << endl;
    // for(int i = 0; i < aco::J_size;i++){
    //     cout << i << ": ";
    //     for(auto n:aco::P[i]){
    //         cout << n << " ";
    //     }
    //     cout << endl;
    // }

    check_conflicts(aco::conflicts);
    // for(auto n:aco::conflicts){
    //     for(auto m:n){
    //         cout << m << " ";
    //     }
    //     cout << endl;
    // }
    // vector<int> s = greedy_random_heuristic(1,1);
    // cout<<"gen sol: "; printv(s);
    vector<int> bestsol = aco_heuristic(100, 0.9, 0.1, 1.0, 0.8, 100);
    cout << "best: "; printv(bestsol);
    cout << "cost: " << solution_cost(bestsol) << endl;
    return 0;
}