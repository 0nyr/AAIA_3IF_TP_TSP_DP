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
        cand[j] = cand[--nbCand]; // remove the chosen candidate
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

int greedyLS(int n, int* sol, int total, int** cost){
    // Input: sol[0..n-1] contains a permutation of [0,n-1], and total = length of the tour associated with sol
    // Output: sol[0..n-1] contains a permutation of [Ø,n-1] such that the corresponding tour does not have crossing edges
    // Return the length of the tour associated with sol
    // Insert your code for greedily improving sol[0..n-1] here!
    return total;
}

int main(){
    int n;
    printf("Number of vertices: "); fflush(stdout);
    scanf("%d",&n);
    int nbTrials;
    printf("Number of random tour constructions: "); fflush(stdout);
    scanf("%d",&nbTrials);
    FILE* fd  = fopen("script.py", "w");
    int** cost = createCost(n, fd);
    int sol[n];
    for (int i=0; i<nbTrials; i++){
        int total = generateRandomTour(n, cost, i, sol);
        printf("Trial %d: Initial tour length = %d; ", i, total);
        clock_t t = clock();
        total = greedyLS(n, sol, total, cost);
        float d = ((double) (clock() - t)) / CLOCKS_PER_SEC;
        printf("Tour length after GreedyLS = %d; CPU time = %.3fs\n", total, d);
        print(sol, n, total, fd);
    }
    return 0;
};





