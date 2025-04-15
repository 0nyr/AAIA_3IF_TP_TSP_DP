/*
 Implementation of a naive DP-based approach for the Travelling Salesman Problem (the approach is naive because it computes more than once a same state as it does not do memoisation; the goal of the practical session is to improve it with memoisation!)
 Copyright (C) 2023 Christine Solnon
 Ce programme est un logiciel libre ; vous pouvez le redistribuer et/ou le modifier au titre des clauses de la Licence Publique Générale GNU, telle que publiée par la Free Software Foundation. Ce programme est distribué dans l'espoir qu'il sera utile, mais SANS AUCUNE GARANTIE ; sans même une garantie implicite de COMMERCIABILITE ou DE CONFORMITE A UNE UTILISATION PARTICULIERE. Voir la Licence Publique Générale GNU pour plus de détails.
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

int computeD(int i, set s, int n, int** cost){
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

int main(){
    int n;
    printf("Number of vertices: "); fflush(stdout);
    if ((scanf("%d",&n) <= 0) || (n > 32) || (n < 1)){
        printf("The number of vertices must be an integer value in [1,32].\n");
        return 0;
    }
    int** cost = createCost(n);
    set s = createSet(n); // s contains all integer values ranging between 1 and n-1
    clock_t t = clock();
    int d = computeD(0, s, n, cost);
    float duration = ((double) (clock() - t)) / CLOCKS_PER_SEC;
    printf("Length of the smallest hamiltonian circuit = %d; CPU time = %.3fs\n", d, duration);
    return 0;
}
