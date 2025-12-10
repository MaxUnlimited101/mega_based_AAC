#ifndef FUNCS_H
#define FUNCS_H

#include <stdio.h>

// Matrix allocation and deallocation
int** allocMatrix(int n);
void freeMatrix(int **mat, int n);

// Matrix I/O operations
void readMatrix(FILE *fp, int **mat, int n);
void writeMatrix(FILE *fp, int **mat, int n);
void printMatrix(int **mat, int n);

// Matrix utilities
void fill_with_zeros(int **mat, int n);

// Utility functions
void swap(int *a, int *b);

// Permutation functions
void permutations(int *arr, int n, int idx, int n1, int n2, int **graph1, int **graph2, int *minimal_change, int ***minimal_subgraph);
void permute(int *arr, int n, int n1, int n2, int **graph1, int **graph2, int *minimal_change, int ***minimal_subgraph);

// Graph algorithms
void minimal_sub_graph(int **graph1, int n1, int **graph2, int n2, int *permutating, int *minimal_change, int ***minimal_subgraph);
void minimal_sub_graph_approximation(int **graph1, int n1, int **graph2, int n2, int *minimal_change, int ***minimal_subgraph);

#endif // FUNCS_H
