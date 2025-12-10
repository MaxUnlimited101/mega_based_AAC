#include "funcs.h"
#include <stdio.h>
#include <stdlib.h>

// allocate n×n matrix
int** allocMatrix(int n) {
    int **mat = malloc(n * sizeof(int*));
    if (!mat) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        mat[i] = malloc(n * sizeof(int));
        if (!mat[i]) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
    }
    return mat;
}

// read n×n matrix from file
void readMatrix(FILE *fp, int **mat, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (fscanf(fp, "%d", &mat[i][j]) != 1) {
                fprintf(stderr, "Error reading matrix element (%d, %d)\n", i, j);
                exit(1);
            }
        }
    }
}

void writeMatrix(FILE *fp, int **mat, int n) {
    fprintf(fp, "%d\n", n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(fp, "%d ", mat[i][j]);
        }
        fprintf(fp, "\n");
    }
}

// print n×n matrix
void printMatrix(int **mat, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
}

// free matrix
void freeMatrix(int **mat, int n) {
    for (int i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);
}

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

// Recursive helper to generate permutations
void permutations(int *arr, int n, int idx, int n1, int n2, int **graph1, int **graph2, int *minimal_change, int ***minimal_subgraph) {
    if (idx == n) {
        minimal_sub_graph(graph1, n1, graph2, n2, arr, minimal_change, minimal_subgraph);
        return;
    }

    for (int i = idx; i < n; ++i) {
        swap(&arr[idx], &arr[i]);              // Choose
        permutations(arr, n, idx + 1, n1, n2, graph1, graph2, minimal_change, minimal_subgraph);  // Explore
        swap(&arr[idx], &arr[i]);              // Backtrack
    }
}

// Wrapper to call permutations
void permute(int *arr, int n, int n1, int n2, int **graph1, int **graph2, int *minimal_change, int ***minimal_subgraph) {
    permutations(arr, n, 0, n1, n2, graph1, graph2, minimal_change, minimal_subgraph);
}

void fill_with_zeros(int **mat, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            mat[i][j] = 0;
        }
    }
}

void minimal_sub_graph(int **graph1, int n1, int **graph2, int n2, int *permutating, int *minimal_change, int ***minimal_subgraph) {
    
    int** current = allocMatrix(n2);
    fill_with_zeros(current, n2);
    int current_change = 0;

    for(int i = 0; i < n1; i++) {
        for (int j = 0; j < n1; j++) {
            if(graph1[i][j] > graph2[permutating[i]][permutating[j]]) {
                current_change += abs(graph1[i][j] - graph2[permutating[i]][permutating[j]]);
                current[permutating[i]][permutating[j]] = graph1[i][j] - graph2[permutating[i]][permutating[j]];
            }
        }
    }

    if (*minimal_change == -1 || current_change < *minimal_change) {
        *minimal_change = current_change;
        *minimal_subgraph = current;
    } else {
        freeMatrix(current, n2);
    }
}

void minimal_sub_graph_approximation(int **graph1, int n1, int **graph2, int n2, int *minimal_change, int ***minimal_subgraph) {
    
    int** current = allocMatrix(n2);
    int current_change = 0;

    for (int i = 0; i <= n2 - n1; i++){
    
            fill_with_zeros(current, n2);
            current_change = 0;

            for(int k = 0; k < n1; k++) {
                for (int l = 0; l < n1; l++) {
                    if(graph1[k][l] > graph2[i + k][i + l]) {
                        current_change += abs(graph1[k][l] - graph2[i + k][i + l]);
                        current[i + k][i + l] = graph1[k][l] - graph2[i + k][i + l];
                    }
                }
            }

            if (*minimal_change == -1 || current_change < *minimal_change) {
                *minimal_change = current_change;
                *minimal_subgraph = current;
            }
        
    }
    if (*minimal_subgraph != current) {
        freeMatrix(current, n2);
    }
}
