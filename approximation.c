#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int minimal_change = -1;
int** minimal_subgraph = NULL;
int permutation_coeff = 3;

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

static inline void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}


void fill_with_zeros(int **mat, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            mat[i][j] = 0;
        }
    }
}

void minimal_sub_graph_approximation(int **graph1, int n1, int **graph2, int n2, int *permutating) {
    
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

int main(int argc, char *argv[]) {

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        fprintf(stderr, "Error: could not open '%s'\n", argv[1]);
        return 1;
    }

    FILE *out = fopen(argv[2], "w");
    if (!out) {
        fprintf(stderr, "Error: could not open '%s' to write\n", argv[2]);
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

    int* permutating = malloc(n1 * sizeof(int));
    for (int i = 0; i < n1; i++) {
        permutating[i] = i;
    }

    permute(permutating, permutation_coeff, n1, n2, graph1, graph2);

    free(permutating);

    printf("\nMinimal change: %d\n", minimal_change);
    printf("Minimal subgraph:\n");
    printMatrix(minimal_subgraph, n2);

    fprintf(out, "%d\n", minimal_change);
    writeMatrix(out, minimal_subgraph, n2);
    fclose(out);




    freeMatrix(graph1, n1);
    freeMatrix(graph2, n2);
    freeMatrix(minimal_subgraph, n2);

    fclose(fp);
    return 0;
}
