#ifndef ACO
#define ACO

#include <bits/stdc++.h>
#include "NAMESPACE/aco_namespace.h"
#include "utils.cpp"
using namespace std;

// DADO UM VETOR P DE PROBABILIDADES AMOSTRA UM TRABALHADOR DE P
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

// FAZ COM O QUE O VALOR MAXIMO DO VETOR SEJA 1
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

// RETORNA OS TRABALHADORES QUE PODEM FAZER A TAREFA I (DADAS AS TAREFAS QUE ELES JA FAZEM)
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

// CONSTROI UMA SOLUCAO PARA O SMPTSP
vector<int> greedy_random_heuristic(double alpha, double beta, vector<vector<double> > &tau) {
    vector<int>  sol(aco::J_size, -1);

    sort(aco::J.begin(), aco::J.end(),compare_times );
    vector<vector<int > > ws(aco::W_size+aco::J_size, vector<int>()); // workers tasks set

    // tarefas que ainda podem ser feitas por cada worker
    vector<double> freq(aco::W_size+aco::J_size, 1/INF);
    for(int i = 0; i < aco::W_size; i++){
        freq[i] = aco::T[i].size();
    }


    for(int i = 0; i < aco::J_size; i++) {
        if(sol[i] != -1) continue;
        vector<int> wline = build_wline(i, ws); // trabalhadores que podem fazer J_i de boa

        vector<double> eta1(wline.size()); // soma da duração das tarefas atribuidas a cada worker
        vector<double> eta2(wline.size(), 0); // numero de tarefas que cada trabalhador ainda pode fazer
        vector<double> tauline(wline.size());
        // TODO: talvez o peso do worker invalido ainda seja muito alto
        
        for(int e = 0; e < wline.size(); e++) {
            eta1[e] = task_length(ws[wline[e]]) +1;
            eta2[e] = wline[e] >= aco::W_size ? 0 : 1/freq[wline[e]];

            tauline[e] = tau[i][wline[e]];
        }

        vector<double> eta = sum_up(eta1, eta2);
        normalize(eta);

        double total = 0;
        for(int k = 0; k < wline.size(); k++) {
            total += pow(tauline[k], alpha) * pow(eta[k], beta);
        }

        vector<double> p (wline.size());
        for(int e = 0; e < wline.size(); e++) {
            p[e] = (pow(tauline[e], alpha) * pow(eta[e], beta) / total) * (1/aco::b[wline[e]]);
            // p[e] = wline[e] >= aco::W_size ? 1/INF : 1; // 100% random
        }
        // p =vector<double> (wline.size(),1);
        normalize(p);
        // cout << "prob: "; printv(p);
        
        int e = wline[sample_worker(p)];
        // cout << "escolhido: " << e << endl;
        for(auto k :aco::P[i]){
            freq[k]--;
        }
        ws[e].push_back(i);
        sol[i] = e;

        //  cout << "tarefa " << i << " foi atribuida ao trabalhador " << e << endl;
        for(auto ts : aco::T[e]){ // pra cada task que o worker pode fazer
            if(sol[ts] != -1) continue;
            bool conflict = false;
            for(auto t : ws[e]){ // pra cada task que o worker ja faz
                if(aco::conflicts[ts][t]){
                    conflict = true;
                    break;
                }
            }
            if(not conflict){ // se nao houver conflito, ele pode pegar essa tb
                sol[ts] = e;
                ws[e].push_back(ts);
            }
            // cout<< "adding..."; printv(sol);
        }

    }
    return sol;
}


// PRE-COMPUTA OS CONFLITOS ENTRE AS TASKS RELATIVO A TEMPO
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


// ATUALIZA TODA A MATRIZ TAU COM BASE NO VALOR DE RHO
void update_tau(vector<vector<double> > &tau, double rho){
    for(int i = 0; i < aco::J_size; i++){
        for(int j = 0; j < aco::J_size+aco::W_size; j++){
            tau[i][j] = tau[i][j] *(1-rho);
        }
    }
}

double max_value(vector<vector<double> > &x){
    double max = 0;
    for(int i = 0; i < x.size(); i++){
        for(int j = 0; j < x[i].size(); j++){
            if(x[i][j] > max){
                max = x[i][j];
            }
        }
    }
    return max;
}

vector<int> aco_heuristic(int tam, double rho,double delta, double alpha, double beta, int limiter){
    double stop = false;
    vector<int> sol(aco::J_size);

    vector<vector<double> > tau(aco::J_size, vector<double>(aco::W_size+aco::J_size, 1));

    int non_improved = 0;

    vector<int> temp_sol(aco::J_size);
    int best_sol_cost = INF;

    while(not stop){
        int bestpop_sol_cost = INF;
        vector<int> bestpop_sol = temp_sol;

        int non_feasible = 0;
        for(int i = 0; i < tam; i++){
            temp_sol=  greedy_random_heuristic(alpha, beta, tau); // generating population
            vector<double> subv(temp_sol.begin(), temp_sol.begin()+10);
            int sol_cost = solution_cost(temp_sol);
            if (sol_cost < bestpop_sol_cost){
                bestpop_sol_cost = sol_cost;
                bestpop_sol = temp_sol;
            }
            if(sol_cost > aco::W_size){
                non_feasible++;
            }
        }
        cout << "non_feasible: " << non_feasible << endl;
        update_tau(tau, rho);

        for(int j = 0; j < aco::J_size; j++){
            tau[j][bestpop_sol[j]] += bestpop_sol[j] >= aco::J_size ? 0 : delta;
        }

        if (bestpop_sol_cost < best_sol_cost){
            best_sol_cost = bestpop_sol_cost;
            sol = bestpop_sol;
            non_improved = 0;
            cout << "melhorou... custo: "<< best_sol_cost << endl;
        }else{
            non_improved++;
        }

        if (non_improved > limiter){
            stop = true;
        }
    }
    return sol;
}

#endif