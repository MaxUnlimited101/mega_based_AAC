#include <stdio.h>
#include <stdlib.h>

int minimal_change = -1;
int** minimal_subgraph = NULL;

void minimal_sub_graph(int **graph1, int n1, int **graph2, int n2, int *permutating);

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
                fprintf(stderr, "Error reading matrix element\n");
                exit(1);
            }
        }
    }
}

void writeMatrix(FILE *fp, int **mat, int n) {
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

// static inline void swap(int *a, int *b) {
//     int t = *a;
//     *a = *b;
//     *b = t;
// }


void fill_with_zeros(int **mat, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            mat[i][j] = 0;
        }
    }
}

void minimal_sub_graph_approximation(int **graph1, int n1, int **graph2, int n2) {
    
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

            if (minimal_change == -1 || current_change < minimal_change) {
                minimal_change = current_change;
                minimal_subgraph = current;
            }
        
    }
    if (minimal_subgraph != current) {
        freeMatrix(current, n2);
    }
}

int main(void) {
    FILE *fp = fopen("input.txt", "r");
    if (!fp) {
        fprintf(stderr, "Error: could not open input.txt\n");
        return 1;
    }

    FILE *out = fopen("output.txt", "w");
    if (!out) {
        fprintf(stderr, "Error: could not open output.txt\n");
        fclose(fp);
        return 1;
    }

    int n1, n2;
    if (fscanf(fp, "%d", &n1) != 1) {
        fprintf(stderr, "Error reading size of first graph\n");
        return 1;
    }
    int **graph1 = allocMatrix(n1);
    readMatrix(fp, graph1, n1);

    if (fscanf(fp, "%d", &n2) != 1) {
        fprintf(stderr, "Error reading size of second graph\n");
        return 1;
    }
    int **graph2 = allocMatrix(n2);
    readMatrix(fp, graph2, n2);

    printf("Graph 1 (%dx%d):\n", n1, n1);
    printMatrix(graph1, n1);

    printf("\nGraph 2 (%dx%d):\n", n2, n2);
    printMatrix(graph2, n2);



    minimal_sub_graph_approximation(graph1, n1, graph2, n2);

    printf("\nMinimal change: %d\n", minimal_change);
    printf("Minimal subgraph:\n");
    printMatrix(minimal_subgraph, n2);

    writeMatrix(out, minimal_subgraph, n2);
    fclose(out);




    freeMatrix(graph1, n1);
    freeMatrix(graph2, n2);
    freeMatrix(minimal_subgraph, n2);

    fclose(fp);
    return 0;
}
