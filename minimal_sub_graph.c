#include <stdio.h>
#include <stdlib.h>
#include "minimal_sub_graph.h"

// allocate n×n matrix
int **allocMatrix(int n)
{
    int **mat = malloc(n * sizeof(int *));
    if (!mat)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    for (int i = 0; i < n; i++)
    {
        mat[i] = malloc(n * sizeof(int));
        if (!mat[i])
        {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
    }
    return mat;
}

// read n×n matrix from file
void readMatrix(FILE *fp, int **mat, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (fscanf(fp, "%d", &mat[i][j]) != 1)
            {
                fprintf(stderr, "Error reading matrix element\n");
                exit(1);
            }
        }
    }
}

void writeMatrix(FILE *fp, int **mat, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            fprintf(fp, "%d ", mat[i][j]);
        }
        fprintf(fp, "\n");
    }
}

// print n×n matrix
void printMatrix(int **mat, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
}

// free matrix
void freeMatrix(int **mat, int n)
{
    for (int i = 0; i < n; i++)
    {
        free(mat[i]);
    }
    free(mat);
}

void swap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

// Recursive helper to generate permutations
void permutations(int *arr, int n, int idx, int n1, int n2, int **graph1, int **graph2, int *minimal_change, int **minimal_subgraph)
{
    if (idx == n)
    {
        // Print one permutation
        // for (int i = 0; i < n; ++i) {
        //     printf("%d ", arr[i]);
        // }
        // printf("\n");
        minimal_sub_graph_exact(graph1, n1, graph2, n2, arr, minimal_change, minimal_subgraph);
        return;
    }

    for (int i = idx; i < n; ++i)
    {
        swap(&arr[idx], &arr[i]);                                                                // Choose
        permutations(arr, n, idx + 1, n1, n2, graph1, graph2, minimal_change, minimal_subgraph); // Explore
        swap(&arr[idx], &arr[i]);                                                                // Backtrack
    }
}

// Wrapper to call permutations
void permute(int *arr, int n, int n1, int n2, int **graph1, int **graph2, int *minimal_change, int **minimal_subgraph)
{
    permutations(arr, n, 0, n1, n2, graph1, graph2, minimal_change, minimal_subgraph);
}

void fill_with_zeros(int **mat, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            mat[i][j] = 0;
        }
    }
}

void minimal_sub_graph_exact(int **graph1, int n1, int **graph2, int n2, int *permutating, int *minimal_change, int **minimal_subgraph)
{

    int **current = allocMatrix(n2);
    fill_with_zeros(current, n2);
    int current_change = 0;

    for (int i = 0; i < n1; i++)
    {
        for (int j = 0; j < n1; j++)
        {
            if (graph1[i][j] > graph2[permutating[i]][permutating[j]])
            {
                current_change += abs(graph1[i][j] - graph2[permutating[i]][permutating[j]]);
                current[permutating[i]][permutating[j]] = graph1[i][j] - graph2[permutating[i]][permutating[j]];
            }
        }
    }

    if (*minimal_change == -1 || current_change < *minimal_change)
    {
        *minimal_change = current_change;
        minimal_subgraph = current;
    }
    else
    {
        freeMatrix(current, n2);
    }
}

void minimal_sub_graph_approximation(int **graph1, int n1, int **graph2, int n2, int *permutating, int *minimal_change, int **minimal_subgraph) {
    
    int** current = allocMatrix(n2);
    int current_change = 0;
    int max_stride = (n2 - n1) / (n1 - 1);

    for(int stride = 0; stride <= max_stride; stride++){
        for (int i = 0; (n1 - 1)*stride + n1 + i < n2; i++){
        
                fill_with_zeros(current, n2);
                current_change = 0;



                for(int row = 0; row < n1; row++) {
                    for (int coloumn = 0; coloumn < n1; coloumn++) {
                        if(graph1[permutating[row]][permutating[coloumn]] > graph2[i + row + stride*row][i + coloumn + stride*coloumn]) {
                            current_change += graph1[permutating[row]][permutating[coloumn]] - graph2[i + row + stride*row][i + coloumn + stride*coloumn];
                            current[i + row + stride*row][i + coloumn + stride*coloumn] = 
                            graph1[permutating[row]][permutating[coloumn]] - graph2[i + row + stride*row][i + coloumn + stride*coloumn];
                        }
                    }
                }

                if (minimal_change == -1 || current_change < minimal_change) {
                    minimal_change = current_change;
                    minimal_subgraph = current;
                }
            
        }
    }
    for(int stride = 0; stride <= max_stride; stride++){
        for (int i = 0; (n1 - 1)*stride + n1 + i < n2; i++){
        
                fill_with_zeros(current, n2);
                current_change = 0;

                for(int row = 0; row < n1; row++) {
                    for (int coloumn = 0; coloumn < n1; coloumn++) {
                        if(graph1[n1 - row - 1][n1 - coloumn - 1] > graph2[i + row + stride*row][i + coloumn + stride*coloumn]) {
                            current_change += graph1[n1 - row - 1][n1 - coloumn - 1] - graph2[i + row + stride*row][i + coloumn + stride*coloumn];
                            current[i + row + stride*row][i + coloumn + stride*coloumn] = 
                            graph1[n1 - row - 1][n1 - coloumn - 1] - graph2[i + row + stride*row][i + coloumn + stride*coloumn];
                        }
                    }
                }

                if (minimal_change == -1 || current_change < minimal_change) {
                    minimal_change = current_change;
                    minimal_subgraph = current;
                }
            
        }
    }
    if (minimal_subgraph != current) {
        freeMatrix(current, n2);
    }
}

void permutations2(int *arr, int n, int idx, int n1, int n2, int **graph1, int **graph2, int *minimal_change, int **minimal_subgraph) {
    if (idx == n) {
        minimal_sub_graph_approximation(graph1, n1, graph2, n2, arr, minimal_change, minimal_subgraph);
        return;
    }

    for (int i = idx; i < n; ++i) {
        swap(&arr[idx], &arr[i]);              // Choose
        permutations2(arr, n, idx + 1, n1, n2, graph1, graph2, minimal_change, minimal_subgraph);  // Explore
        swap(&arr[idx], &arr[i]);              // Backtrack
    }
}

// Wrapper to call permutations
void permute2(int *arr, int n, int n1, int n2, int **graph1, int **graph2, int *minimal_change, int **minimal_subgraph) {
    permutations2(arr, n, 0, n1, n2, graph1, graph2, minimal_change, minimal_subgraph);
}
