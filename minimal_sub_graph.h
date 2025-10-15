#ifndef MINIMAL_SUB_GRAPH_H
#define MINIMAL_SUB_GRAPH_H

#include <stdio.h>
#include <stdlib.h>

void minimal_sub_graph_approximation(int **graph1, int n1, int **graph2, int n2, int *permutating, int *minimal_change, int **minimal_subgraph);
void minimal_sub_graph_exact(int **graph1, int n1, int **graph2, int n2, int *permutating, int *minimal_change, int **minimal_subgraph);

// allocate n×n matrix
int** allocMatrix(int n);

// read n×n matrix from file
void readMatrix(FILE *fp, int **mat, int n);

void writeMatrix(FILE *fp, int **mat, int n);

// print n×n matrix
void printMatrix(int **mat, int n);

// free matrix
void freeMatrix(int **mat, int n);

void swap(int *a, int *b);

// Recursive helper to generate permutations
void permutations(int *arr, int n, int idx, int n1, int n2, int **graph1, int **graph2, int *minimal_change, int **minimal_subgraph);

// Wrapper to call permutations
void permute(int *arr, int n, int n1, int n2, int **graph1, int **graph2, int *minimal_change, int **minimal_subgraph);

void fill_with_zeros(int **mat, int n);



void permute2(int *arr, int n, int n1, int n2, int **graph1, int **graph2, int *minimal_change, int **minimal_subgraph);
void permutations2(int *arr, int n, int idx, int n1, int n2, int **graph1, int **graph2, int *minimal_change, int **minimal_subgraph);

#endif // MINIMAL_SUB_GRAPH_H