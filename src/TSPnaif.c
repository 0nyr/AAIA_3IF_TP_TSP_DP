/*
 Implementation of a naive DP-based approach for the Travelling Salesman Problem (the approach is naive because it computes more than once a same state as it does not do memoisation; the goal of the practical session is to improve it with memoisation!)
 Copyright (C) 2023 Christine Solnon
 Ce programme est un logiciel libre ; vous pouvez le redistribuer et/ou le modifier au titre des clauses de la Licence Publique Générale GNU, telle que publiée par la Free Software Foundation. Ce programme est distribué dans l'espoir qu'il sera utile, mais SANS AUCUNE GARANTIE ; sans même une garantie implicite de COMMERCIABILITE ou DE CONFORMITE A UNE UTILISATION PARTICULIERE. Voir la Licence Publique Générale GNU pour plus de détails.
 
 Compile with:
    gcc -o tspnaif TSPnaif.c -O3 -Wall -lm
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>

typedef int set; // representation of a set with a bit vector

bool isIn(int e, set s){
    // Precondition: 1 <= e <= 32
    // Postrelation: return true if e belongs to s
    return ((s & (1 << (e-1))) != 0);
}

bool isEmpty(set s){
    // Postrelation: return true if s is empty
    return (s == 0);
}

set addElement(set s, int e){
    // Precondition: 1 <= e <= 32
    // Postrelation: return the set s U {e}
    return (s | (1 << (e-1)));
}

set removeElement(set s, int e){
    // Precondition: 1 <= e <= 32
    // Postrelation: return the set s \ {e}
    return (s ^ (1 << (e-1)));
}

set createSet(int n){
    // Precondition: 1 <= n <= 32
    // Postrelation: return the set that contains all integer ranging from 1 to n-1
    return (1 << (n - 1)) - 1;
}

void printSet(set s){
    // Postcondition: print all elements of s
    int i = 1;
    while (s != 0){
        if (s%2 != 0) printf(" %d",i);
        s /= 2;
        i++;
    }
}

int iseed = 1;  // Seed used for initialising the pseudo-random number generator

int nextRand(int n){
    // Postcondition: return an integer value in [0,n-1], according to a pseudo-random sequence
    int i = 16807 * (iseed % 127773) - 2836 * (iseed / 127773);
    if (i > 0) iseed = i;
    else iseed = 2147483647 + i;
    return iseed % n;
}

int** createCost(int n){
    // return a symmetrical cost matrix such that, for each i,j in [0,n-1], cost[i][j] = cost of arc (i,j)
    int x[n], y[n];
    int max = 1000;
    int** cost;
    cost = (int**) malloc(n*sizeof(int*));
    for (int i=0; i<n; i++){
        x[i] = nextRand(max);
        y[i] = nextRand(max);
        cost[i] = (int*)malloc(n*sizeof(int));
    }
    for (int i=0; i<n; i++){
        cost[i][i] = max*max;
        for (int j=i+1; j<n; j++){
            cost[i][j] = (int)sqrt((x[i]-x[j])*(x[i]-x[j]) + (y[i]-y[j])*(y[i]-y[j]));
            cost[j][i] = cost[i][j];
        }
    }
    return cost;
}

__uint64_t nb_calls = 0; // number of calls to computeD
__uint64_t nb_states = 0;          // nb of states in the memoisation table

/**
 * Held-Karp algorithm for the Travelling Salesman Problem
 */
int computeD(int i, set s, int n, int** cost){
    nb_calls += 1;
    // Preconditions: isIn(i,s) = false and isIn(0,s) = false
    // Postrelation: return the cost of the smallest path that starts from i, visits each vertex of s exactly once, and ends on 0
    if (isEmpty(s)) return cost[i][0];
    int min = INT_MAX;
    for (int j=1; j<n; j++){
        if (isIn(j,s)){
            int d = computeD(j, removeElement(s,j), n, cost);
            if (cost[i][j] + d < min) min = cost[i][j] + d;
        }
    }
    return min;
}

/**
 * computeD version with memoisation
 */
int computeD_memo(int i, set s, int n, int** cost, int** memo){
    nb_calls += 1;
    // Preconditions: isIn(i,s) = false and isIn(0,s) = false
    // Postrelation: return the cost of the smallest path that starts from i, visits each vertex of s exactly once, and ends on 0
    if (isEmpty(s)) return cost[i][0];
    if (memo[i][s] != 0) return memo[i][s];
    int min = INT_MAX;
    for (int j=1; j<n; j++){
        if (isIn(j,s)){
            int d = computeD_memo(j, removeElement(s,j), n, cost, memo);
            if (cost[i][j] + d < min) min = cost[i][j] + d;
        }
    }
    memo[i][s] = min;
    return min;
}

int heldKarp_iter(int n, int **cost, int **dp, int **succ){
    int FULL = 1 << (n-1); // stop when 2^(n-1) - 1 states are reached
    // initialize the dp and succ tables
    for(int i=0; i<n; ++i){
        for(int S=0; S<FULL; ++S){
            dp[i][S]   = INT_MAX;
            succ[i][S] = -1;
        }
    }

    // base case: start from depot (0) and go to the first vertex
    for(int i=1; i<n; ++i) dp[i][0] = cost[i][0];

    // compute the cost of all paths
    for(int S=1; S<FULL; ++S){ // all subsets of vertices
        for(int i=1; i<n; ++i){
            if(isIn(i,S)) continue; // i must not be in S
            int best = INT_MAX;
            int bestj = -1;
            for(int j=1; j<n; ++j){
                if(!isIn(j,S)) continue; // j must be in S
                int S2 = removeElement(S,j);
                int val = dp[j][S2];
                if(val==INT_MAX) continue; // no path from j to S2
                int alt = cost[i][j] + val;
                if(alt < best){
                    best  = alt;
                    bestj = j;
                }
            }
            dp[i][S] = best;
            succ[i][S] = bestj;
            nb_states++;
        }
    }

    // return to depot (0)
    int best = INT_MAX;
    int bestj = -1; 
    int ALL = FULL-1; // 2^(n-1) - 1, a full set 111...111 with n-1 bits
    for(int j=1; j<n; ++j){
        int val = dp[j][removeElement(ALL,j)];
        if(val==INT_MAX) continue;
        int alt = cost[0][j] + val;
        if(alt < best){
            best  = alt;
            bestj = j;
        }
    }
    succ[0][ALL] = bestj;
    return best;
}

/**
 * Print the tour, iterate over the successors
 * starting from the depot (0) and going to the first vertex
 * and then to the next vertex, etc.
 * The tour ends when we return to the depot (0)
 */
void printTour(int n, int **succ){
    int S = (1<<(n-1)) - 1;   // full set {1,2,...,n-1}
    int i = 0;
    printf("Circuit : 0");
    for(int k=0; k<n-1; ++k){
        int j = succ[i][S];
        if(j==-1){ printf(" ?"); break; }
        printf(" %d", j);
        S = removeElement(S, j);
        i = j;
    }
    printf(" 0\n");
}

int main(int argc, char** argv){
    int n, d;

    // Get parameters either from command line or from user
    if (argc > 1) {
        n = atoi(argv[1]);
    } else {
        printf("Number of vertices: "); fflush(stdout);
        if (scanf("%d", &n) != 1) {
            printf("Error reading the number of vertices.\n");
            return 0;
        }
    }
    if ((n > 32) || (n < 1)){
        printf("The number of vertices must be an integer value in [1,32].\n");
        return 0;
    }

    int** cost = createCost(n);
    set s = createSet(n); // s contains all integer values ranging between 1 and n-1
    clock_t t = clock();
    // int d = computeD(0, s, n, cost);
    float duration;
    // duration = ((double) (clock() - t)) / CLOCKS_PER_SEC;
    // printf("Length of the smallest hamiltonian circuit = %d; CPU time = %.3fs\n", d, duration);
    // printf("    - Number of calls to computeD = %lu\n", nb_calls);
    
    // Version with memoisation
    nb_calls = 0;
    t = clock();
    int** memo = (int**) malloc(n*sizeof(int*));
    /**
     * Allocate the memoisation table
     * There are n rows (one for each vertex).
     * The number of columns is 2^(n-1) (one for each subset of vertices).
     * The size of the memoisation table is thus n * 2^(n-1) integers.
     */
    for (int i=0; i<n; i++){
        memo[i] = (int*)malloc((pow(2, n - 1))*sizeof(int));
        for (int j=0; j<(pow(2, n - 1)); j++) memo[i][j] = 0;
    }
    printf("Alloc time = %.3fs\n", ((double) (clock() - t)) / CLOCKS_PER_SEC);

    t = clock();
    d = computeD_memo(0, s, n, cost, memo);
    duration = ((double) (clock() - t)) / CLOCKS_PER_SEC;
    printf("Length of the smallest hamiltonian circuit (with memoisation) = %d; CPU time = %.3fs\n", d, duration);
    printf("    - Number of calls to computeD_memo = %lu\n", nb_calls);

    // Version with dynamic programming
    nb_calls = 0;
    t = clock();
    int **dp = (int**) malloc(n*sizeof(int*));
    int **succ = (int**) malloc(n*sizeof(int*));
    for (int i=0; i<n; i++){
        dp[i] = (int*)malloc((pow(2, n - 1))*sizeof(int));
        succ[i] = (int*)malloc((pow(2, n - 1))*sizeof(int));
    }
    d = heldKarp_iter(n, cost, dp, succ);
    duration = ((double) (clock() - t)) / CLOCKS_PER_SEC;
    printf("Length of the smallest hamiltonian circuit (with dynamic programming) = %d; CPU time = %.3fs\n", d, duration);
    printf("    - Number of states in the memoisation table = %lu\n", nb_states);
    printTour(n, succ);

    // for (n = 1; n < 10; n++){
    //     nb_calls = 0;
    //     set s = createSet(n);
    //     computeD(0, s, n, cost);
    //     printf("n = %d; Number of calls to computeD = %lu\n", n, nb_calls);
    // }

    // Free the cost matrix
    for (int i=0; i<n; i++){
        free(cost[i]);
    }
    free(cost);
    
    return 0;
}
