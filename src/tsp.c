/*
 Code framework for solving the Travelling Salesman Problem with local search
 Copyright (C) 2023 Christine Solnon
 Ce programme est un logiciel libre ; vous pouvez le redistribuer et/ou le modifier au titre des clauses de la Licence Publique Générale GNU, telle que publiée par la Free Software Foundation. Ce programme est distribué dans l'espoir qu'il sera utile, mais SANS AUCUNE GARANTIE ; sans même une garantie implicite de COMMERCIABILITE ou DE CONFORMITE A UNE UTILISATION PARTICULIERE. Voir la Licence Publique Générale GNU pour plus de détails.
 */

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

int iseed = 1;

int nextRand(int n){
    // Postcondition: return an integer value in [0,n-1], according to a pseudo-random sequence
    int i = 16807 * (iseed % 127773) - 2836 * (iseed / 127773);
    if (i > 0) iseed = i;
    else iseed = 2147483647 + i;
    return iseed % n;
}

int** createCost(int n, FILE* fd){
    // input: the number n of vertices and a file descriptor fd
    // return a symmetrical cost matrix such that, for each i,j in [0,n-1], cost[i][j] = cost of arc (i,j)
    // side effect: print in fd a Python script for defining turtle coordinates associated with vertices
    int x[n], y[n];
    int max = 1000;
    int** cost;
    int iseed = 1;

    // Allocate 1st dimension of cost
    cost = (int**) malloc(n*sizeof(int*));

    fprintf(fd, "import turtle\n");
    fprintf(fd, "turtle.setworldcoordinates(0, 0, %d, %d)\n", max, max+100);
    
    // Init coordinates and allocate 2nd dimension of cost
    for (int i=0; i<n; i++){
        x[i] = nextRand(max);
        y[i] = nextRand(max);
        fprintf(fd, "p%d=(%d,%d)\n", i, x[i], y[i]);
        cost[i] = (int*)malloc(n*sizeof(int));
    }

    // Fill cost matrix by computing euclidean distances
    // between all pairs of vertices
    // and set the diagonal to a large value
    for (int i=0; i<n; i++){
        cost[i][i] = max*max;
        for (int j=i+1; j<n; j++){
            cost[i][j] = (int)sqrt((x[i]-x[j])*(x[i]-x[j]) + (y[i]-y[j])*(y[i]-y[j]));
            cost[j][i] = cost[i][j];
        }
    }
    return cost;
}

int generateRandomTour(int n, int** cost, int seed, int* sol){
    // input: the number of vertices n, the cost matrix such that for all i,j in [0,n-1], cost[i][j] = cost of arc (i,j), the seed for the random number generator
    // output: sol[0..n-1] is a random permutation of [0..n-1]
    // postcondition: return cost[n-1][0] + the sum of cost[i][i+1] for i in [0,n-2]
    int cand[n]; // candidates for the next vertex
    for (int i=0; i<n; i++) cand[i] = i;
    sol[0] = nextRand(n); // randomly choose the first vertex
    cand[sol[0]] = n-1;
    int total = 0;
    int nbCand = n-1;

    // Build a random tour
    for (int i=1; i<n; i++){
        int j = nextRand(nbCand);
        sol[i] = cand[j];
        cand[j] = cand[--nbCand]; // remove the chosen candidate, replace it by another (unvisited) candidate
        total += cost[sol[i-1]][sol[i]];
    }
    total += cost[sol[n-1]][sol[0]]; // don't forget the return to the depot
    return total;
}

void print(int* sol, int n, int totalLength, FILE* fd){
    // input: n = number n of vertices; sol[0..n-1] = permutation of [0,n-1]; fd = file descriptor
    // side effect: print in fd the Python script for displaying the tour associated with sol
    fprintf(fd, "turtle.clear()\n");
    fprintf(fd, "turtle.tracer(0,0)\n");
    fprintf(fd, "turtle.penup()\n");
    fprintf(fd, "turtle.goto(0,%d)\n", 1050);
    fprintf(fd, "turtle.write(\"Total length = %d\")\n", totalLength);
    fprintf(fd, "turtle.speed(0)\n");
    fprintf(fd, "turtle.goto(p%d)\n", sol[0]);
    fprintf(fd, "turtle.pendown()\n");
    for (int i=1; i<n; i++) fprintf(fd, "turtle.goto(p%d)\n", sol[i]);
    fprintf(fd, "turtle.goto(p%d)\n", sol[0]);
    fprintf(fd, "turtle.update()\n");
    fprintf(fd, "wait = input(\"Enter return to continue\")\n");
}

/**
 * Check if the edges (node0->node1) and (nodeLast->nodeNew) "cross"
 */
bool isCrossing(
    int node0, // v_i
    int node1, // v{i+1}
    int nodeLast, // v_j
    int nodeNew, // v{j+1}
    int** cost
) {
    /* check each edge before the last one

    Example:
    ╎     ╎
    Ⓝ◁---①
    ╎◸   ◹╎
    ╎ ╲ ╱ ╎
    ╎  ╳  ╎
    ╎ ╱ ╲ ╎
    ╎╱   ╲╎
    ⓪---▷Ⓛ
    ╎     ╎
    We check that any edge (⓪->① + Ⓛ->Ⓝ) <= (⓪->Ⓛ + ①->Ⓝ)
    */
    // cost Ⓛ->Ⓝ
    int costLastToNew = cost[nodeLast][nodeNew];
    // cost ⓪->①
    int cost0to1 = cost[node0][node1];
    // cost ⓪->Ⓛ
    int cost0toLast = cost[node0][nodeLast];
    // cost ①->Ⓝ
    int cost1toNew = cost[node1][nodeNew];
    if (
        (cost0toLast + cost1toNew) // cost ⓪->Ⓛ + ①->Ⓝ
        < (cost0to1 + costLastToNew) // cost ⓪->① + Ⓛ->Ⓝ
    ) {
        return true;
    } else {
        return false;
    }
}

void swap(int* sol, int i, int j){
    // input: sol[0..n-1] = permutation of [0,n-1]; i,j = indices of two vertices in sol
    // side effect: swap the two vertices in sol
    int tmp = sol[i];
    sol[i] = sol[j];
    sol[j] = tmp;
}

bool while_procedure(int n, int* sol, int total, int** cost){
    int v_i0 = 0;
    int v_i1 = 0;
    int v_j0 = 0;
    int v_j1 = 0;
    for (int i=0; i<n-1; i++) {
        for (int j=i+2; j<n; j++) {
            v_i0 = sol[i];
            v_i1 = sol[i+1];
            v_j0 = sol[j];
            if (j == n-1) 
                v_j1 = sol[0];
            else
                v_j1 = sol[j+1];
            
            if (isCrossing(v_i0, v_i1, v_j0, v_j1, cost)) {
                // swap arcs (v_i0->v_i1), (v_j0->v_j1) with (v_i0->v_j0), (v_i1->v_j1)
                // To do that, we swap v_j0 and v_i1
                // as we swap two by two any intermediate vertices
                // between v_i1 and v_j0
                int nb_swaps = ceil((j-i)/2.0);
                for (int k=0; k<nb_swaps; k++) {
                    swap(sol, i+1+k, j-k);
                }
                return true; // crossing detected
            }
        }
    }
    return false; // no crossing detected
}

void print_sol(int* sol, int n){
    // input: n = number n of vertices; sol[0..n-1] = permutation of [0,n-1]
    // side effect: print the tour associated with sol
    printf("Tour: ");
    for (int i=0; i<n; i++) {
        printf("%d ", sol[i]);
    }
    printf("\n");
}

int compute_sol_length(int* sol, int n, int** cost){
    // input: n = number n of vertices; sol[0..n-1] = permutation of [0,n-1]
    // return the length of the tour associated with sol
    int total = 0;
    for (int i=0; i<n-1; i++) {
        total += cost[sol[i]][sol[i+1]];
    }
    total += cost[sol[n-1]][sol[0]]; // don't forget the return to the depot
    return total;
}

void print_sol_with_cost(int* sol, int n, int** cost){
    // input: n = number n of vertices; sol[0..n-1] = permutation of [0,n-1]; total = length of the tour associated with sol
    // side effect: print the tour associated with sol and its length
    printf("Tour: ");
    for (int i=0; i<n; i++) {
        printf("%d ", sol[i]);
    }
    printf(" - Total length = %d\n", compute_sol_length(sol, n, cost));
}

int greedyLS(int n, int* sol, int total, int** cost){
    // Input: sol[0..n-1] contains a permutation of [0,n-1], and total = length of the tour associated with sol
    // Output: sol[0..n-1] contains a permutation of [Ø,n-1] such that the corresponding tour does not have crossing edges
    // Return the length of the tour associated with sol
    bool has_crossing;
    while (true){
        #ifdef DEBUG
        print_sol_with_cost(sol, n, cost);
        #endif
        has_crossing = while_procedure(n, sol, total, cost);
        if (!has_crossing) break;
    }

    return compute_sol_length(sol, n, cost);
}

/**
 * Iterative Greedy Local Search
 * 
 * Preconditions: sol contains a random valid tour
 */
int iterative_greedy_LS(
    int n, 
    int* sol, 
    int total, 
    int** cost, 
    int nb_iterations,
    int nb_perturbations
) {
    // Get initial solution
    int UB = greedyLS(n, sol, total, cost);
    int new_sol[n];
    
    for (int i=0; i<nb_iterations; i++) {
        // Copy the current solution
        memcpy(new_sol, sol, n*sizeof(int));

        // Perturb the solution
        for (int j=0; j<nb_perturbations; j++) {
            // Exchange randownly two vertices
            int rand_index_1, rand_index_2;
            do {
                rand_index_1 = nextRand(n);
                rand_index_2 = nextRand(n);
            } while (rand_index_1 == rand_index_2);
            swap(sol, rand_index_1, rand_index_2);
        }
        int new_length = greedyLS(n, new_sol, total, cost);
        if (new_length < UB) {
            // Update the best solution
            UB = new_length;
            memcpy(sol, new_sol, n*sizeof(int));
            printf("New best solution found: ");
            print_sol_with_cost(sol, n, cost);
        }
    }

    return UB;
}

int main(int argc, char** argv){
    int n;

    // Get parameters either from command line or from user
    int nb_iterations;
    int nb_perturbations;
    if (argc > 3) {
        n = atoi(argv[1]);
        nb_iterations = atoi(argv[2]);
        nb_perturbations = atoi(argv[3]);
    } else {
        printf("Number of vertices: "); fflush(stdout);
        scanf("%d",&n);
        printf("Number of random tour constructions / iterations: "); fflush(stdout);
        scanf("%d",&nb_iterations);
        printf("Number of perturbations: "); fflush(stdout);
        scanf("%d",&nb_perturbations);
    }

    FILE* fd  = fopen("script.py", "w");
    int** cost = createCost(n, fd);
    int sol[n];
    int total = generateRandomTour(n, cost, iseed, sol);

    // for (int i=0; i<nb_iterations; i++){
    //     total = generateRandomTour(n, cost, i, sol);
    //     printf("Trial %d: Initial tour length = %d; ", i, total);
    //     clock_t t = clock();
    //     total = greedyLS(n, sol, total, cost);
    //     float d = ((double) (clock() - t)) / CLOCKS_PER_SEC;
    //     printf("Tour length after GreedyLS = %d; CPU time = %.3fs\n", total, d);
    //     print(sol, n, total, fd);
    // }

    // ILS
    clock_t t = clock();
    total = iterative_greedy_LS(n, sol, total, cost, nb_iterations, nb_perturbations);
    float d = ((double) (clock() - t)) / CLOCKS_PER_SEC;
    printf("Tour length after ILS = %d; CPU time = %.3fs\n", total, d);
    print(sol, n, total, fd);
    
    
    fclose(fd);
    return 0;
};





