#include "funcs.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

typedef struct {
    int n1;
    int n2;
    int **graph1;
    int **graph2;
    int exact_change;
    int approx_change;
} ExperimentResult;

typedef struct {
    int n1;
    int n2;
    int **graph1;
    int **graph2;
    int *minimal_change;
    int ***minimal_subgraph;
} ThreadArgs;

typedef struct {
    int test_index;
    int n1;
    int n2;
    int max_value;
    ExperimentResult *result;
    unsigned int seed;
} ExperimentThreadArgs;

typedef struct {
    int start_idx;
    int end_idx;
    ExperimentThreadArgs *test_args;
} WorkerThreadArgs;

void generate_random_matrix(int **mat, int n, int max_value, unsigned int *seed) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            mat[i][j] = rand_r(seed) % (max_value + 1);
        }
    }
}

int** copy_matrix(int **mat, int n) {
    int **copy = allocMatrix(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            copy[i][j] = mat[i][j];
        }
    }
    return copy;
}

void* run_exact(void* arg) {
    ThreadArgs *args = (ThreadArgs*)arg;
    
    int* permutating = malloc(args->n2 * sizeof(int));
    for (int i = 0; i < args->n2; i++) {
        permutating[i] = i;
    }
    
    permute(permutating, args->n2, args->n1, args->n2, 
            args->graph1, args->graph2, 
            args->minimal_change, args->minimal_subgraph);
    
    free(permutating);
    return NULL;
}

void* run_approximation(void* arg) {
    ThreadArgs *args = (ThreadArgs*)arg;
    
    minimal_sub_graph_approximation(args->graph1, args->n1, 
                                   args->graph2, args->n2,
                                   args->minimal_change, args->minimal_subgraph);
    
    return NULL;
}

ExperimentResult run_experiment(int n1, int n2, int max_value, unsigned int *seed) {
    ExperimentResult result;
    result.n1 = n1;
    result.n2 = n2;
    
    // Generate random matrices
    int **graph1 = allocMatrix(n1);
    int **graph2 = allocMatrix(n2);
    
    generate_random_matrix(graph1, n1, max_value, seed);
    generate_random_matrix(graph2, n2, max_value, seed);
    
    result.graph1 = graph1;
    result.graph2 = graph2;
    
    // Run exact solution
    int exact_minimal_change = -1;
    int **exact_minimal_subgraph = NULL;
    
    ThreadArgs arg1 = {n1, n2, graph1, graph2, &exact_minimal_change, &exact_minimal_subgraph};
    run_exact(&arg1);
    
    result.exact_change = exact_minimal_change;
    
    if (exact_minimal_subgraph != NULL) {
        freeMatrix(exact_minimal_subgraph, n2);
    }
    
    // Run approximation solution
    int approx_minimal_change = -1;
    int **approx_minimal_subgraph = NULL;
    
    ThreadArgs approx_args = {n1, n2, graph1, graph2, &approx_minimal_change, &approx_minimal_subgraph};
    run_approximation(&approx_args);
    
    result.approx_change = approx_minimal_change;
    
    if (approx_minimal_subgraph != NULL) {
        freeMatrix(approx_minimal_subgraph, n2);
    }
    
    return result;
}

void* run_experiment_thread(void* arg) {
    ExperimentThreadArgs *args = (ExperimentThreadArgs*)arg;
    
    // printf("Starting test %d: n1=%d, n2=%d\n", args->test_index + 1, args->n1, args->n2);
    // fflush(stdout);
    
    *(args->result) = run_experiment(args->n1, args->n2, args->max_value, &args->seed);
    
    // printf("Completed test %d: n1=%d, n2=%d - Exact: %d (%.4fs), Approx: %d (%.4fs)\n",
    //        args->test_index + 1, args->n1, args->n2,
    //        args->result->exact_change, args->result->exact_time,
    //        args->result->approx_change, args->result->approx_time);
    // fflush(stdout);
    
    return NULL;
}

void* worker_thread(void* arg) {
    WorkerThreadArgs *worker = (WorkerThreadArgs*)arg;
    
    for (int i = worker->start_idx; i < worker->end_idx; i++) {
        ExperimentThreadArgs *test = &worker->test_args[i];
        *(test->result) = run_experiment(test->n1, test->n2, test->max_value, &test->seed);
        
        if ((i - worker->start_idx + 1) % 100 == 0) {
            printf("Worker completed %d tests...\n", i - worker->start_idx + 1);
            fflush(stdout);
        }
    }
    
    return NULL;
}

void save_results_to_csv(ExperimentResult *results, int num_results, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "Error: could not open '%s' for writing\n", filename);
        return;
    }
    
    fprintf(fp, "n1,n2,exact_change,approx_change\n");
    
    for (int i = 0; i < num_results; i++) {
        
        fprintf(fp, "%d,%d,%d,%d\n",
                results[i].n1, results[i].n2,
                results[i].exact_change, results[i].approx_change);
    }
    
    fclose(fp);
    printf("Results saved to %s\n", filename);
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    
    const char *output_file = "experiment_results.csv";
    int max_value = 20;
    int num_tests = 10000;
    int min_n1 = 3;
    int max_n1 = 8;
    int min_n2 = 4;
    int max_n2 = 10;
    int num_workers = 5;
    
    if (argc > 1) {
        output_file = argv[1];
    }
    if (argc > 2) {
        max_value = atoi(argv[2]);
    }
    if (argc > 3) {
        num_tests = atoi(argv[3]);
    }
    if (argc > 4) {
        num_workers = atoi(argv[4]);
    }
    
    ExperimentResult *results = malloc(num_tests * sizeof(ExperimentResult));
    ExperimentThreadArgs *test_args = malloc(num_tests * sizeof(ExperimentThreadArgs));
    
    printf("Generating %d random test cases with max_value=%d...\n", num_tests, max_value);
    printf("Matrix size ranges: n1=[%d,%d], n2=[%d,%d] (where n1 < n2)\n", 
           min_n1, max_n1, min_n2, max_n2);
    printf("Using %d worker threads\n\n", num_workers);
    
    // Generate random test cases
    for (int i = 0; i < num_tests; i++) {
        unsigned int seed = (unsigned int)time(NULL) + i * 1000;
        int n1 = min_n1 + rand_r(&seed) % (max_n1 - min_n1 + 1);
        int n2 = (n1 + 1) + rand_r(&seed) % (max_n2 - n1);  // Ensure n2 > n1
        
        test_args[i].test_index = i;
        test_args[i].n1 = n1;
        test_args[i].n2 = n2;
        test_args[i].max_value = max_value;
        test_args[i].result = &results[i];
        test_args[i].seed = seed;
    }
    
    printf("Running experiments with %d worker threads...\n\n", num_workers);
    
    // Create worker threads
    pthread_t *workers = malloc(num_workers * sizeof(pthread_t));
    WorkerThreadArgs *worker_args = malloc(num_workers * sizeof(WorkerThreadArgs));
    
    int tests_per_worker = num_tests / num_workers;
    int remaining = num_tests % num_workers;
    
    int current_idx = 0;
    for (int i = 0; i < num_workers; i++) {
        worker_args[i].start_idx = current_idx;
        worker_args[i].end_idx = current_idx + tests_per_worker + (i < remaining ? 1 : 0);
        worker_args[i].test_args = test_args;
        
        printf("Worker %d: processing tests %d to %d (%d tests)\n", 
               i + 1, worker_args[i].start_idx, worker_args[i].end_idx - 1,
               worker_args[i].end_idx - worker_args[i].start_idx);
        
        pthread_create(&workers[i], NULL, worker_thread, &worker_args[i]);
        current_idx = worker_args[i].end_idx;
    }
    
    printf("\n");
    
    // Wait for all worker threads to complete
    for (int i = 0; i < num_workers; i++) {
        pthread_join(workers[i], NULL);
    }
    
    printf("\nAll experiments completed!\n\n");
    
    // Free the matrices
    for (int i = 0; i < num_tests; i++) {
        freeMatrix(results[i].graph1, results[i].n1);
        freeMatrix(results[i].graph2, results[i].n2);
    }
    
    free(workers);
    free(worker_args);
    free(test_args);
    
    printf("\n");
    save_results_to_csv(results, num_tests, output_file);
    
    free(results);
    
    return 0;
}
