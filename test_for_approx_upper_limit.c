#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "minimal_sub_graph.h"

#define NUM_OF_THREADS 8

struct Task
{
    // smaller
    int **G;
    int G_size;

    // bigger
    int **H;
    int H_size;
};

struct Result
{
    int exact;
    int approx;
};

struct Thread_Args
{
    int index;
    int tasks_size;
    struct Task *tasks;
    struct Result *results;
};

void* worker_function(void* args)
{
    struct Thread_Args *thread_args = (struct Thread_Args *)args;
    int index = thread_args->index;
    int tasks_size = thread_args->tasks_size;
    struct Task *tasks = thread_args->tasks;
    struct Result *results = thread_args->results;

    int i = index;
    for (; index < tasks_size; i += NUM_OF_THREADS)
    {
        if (i >= tasks_size)
        {
            break;
        }
        printf("Thread %d processing task %d/%d\n", index, i + 1, tasks_size);
        int minimal_change = -1;
        int **minimal_subgraph = NULL;
        int perm_coeff = 3;

        int n1, n2;
        n1 = tasks[i].G_size;
        int **graph1;
        graph1 = tasks[i].G;

        n2 = tasks[i].H_size;
        int **graph2;
        graph2 = tasks[i].H;

        // printf("Graph 1 (%dx%d):\n", n1, n1);
        // printMatrix(graph1, n1);

        // printf("\nGraph 2 (%dx%d):\n", n2, n2);
        // printMatrix(graph2, n2);

        int *permutating = malloc(n2 * sizeof(int));
        for (int i = 0; i < n2; i++)
        {
            permutating[i] = i;
        }
        int *permutating2 = malloc(n1 * sizeof(int));
        for (int i = 0; i < n1; i++)
        {
            permutating2[i] = i;
        }

        permute2(permutating2, perm_coeff, n1, n2, graph1, graph2, &minimal_change, minimal_subgraph);
        results[i].approx = minimal_change;
        minimal_change = -1;
        permute(permutating, n2, n1, n2, graph1, graph2, &minimal_change, minimal_subgraph);
        results[i].exact = minimal_change;

        free(permutating);
        if (minimal_subgraph)
        {
            freeMatrix(minimal_subgraph, n2);
        }
    }
    free(args);
    return NULL;
}

void save_results(struct Result *results, int task_count)
{
    FILE *fp = fopen("results.csv", "w");
    if (!fp)
    {
        perror("Error opening results.csv: ");
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "exact,approx\n");
    for (int i = 0; i < task_count; i++)
    {
        fprintf(fp, "%d,%d\n", results[i].exact, results[i].approx);
    }

    fclose(fp);
}

int main()
{
    int task_count = 1000 * 3;
    printf("Task count: %d\n", task_count);

    struct Task *tasks = NULL;
    tasks = calloc(task_count, sizeof(struct Task));
    if (!tasks)
    {
        perror("Error calloc(): ");
        exit(EXIT_FAILURE);
    }

    struct Result *results = NULL;
    results = calloc(task_count, sizeof(struct Result));
    if (!results)
    {
        perror("Error calloc(): ");
        exit(EXIT_FAILURE);
    }

    srand((unsigned int)time(NULL));
    printf("Generating random graphs...\n");
    for (int i = 0; i < task_count; i++)
    {
        tasks[i].G_size = 3;
        tasks[i].H_size = 9;

        int **graph1 = NULL;
        graph1 = allocMatrix(tasks[i].G_size);
        int **graph2 = NULL;
        graph2 = allocMatrix(tasks[i].H_size);

        if (!graph1 || !graph2)
        {
            perror("Error allocMatrix(): ");
            exit(EXIT_FAILURE);
        }

        for (int r = 0; r < tasks[i].G_size; r++)
        {
            for (int c = 0; c < tasks[i].G_size; c++)
            {
                graph1[r][c] = rand() % 10;
            }
        }

        for (int r = 0; r < tasks[i].H_size; r++)
        {
            for (int c = 0; c < tasks[i].H_size; c++)
            {
                graph2[r][c] = rand() % 10;
            }
        }

        for (int r = 0; r < tasks[i].G_size; r++)
        {
            graph1[r][r] = 0;
        }

        for (int r = 0; r < tasks[i].H_size; r++)
        {
            graph2[r][r] = 0;
        }

        tasks[i].G = graph1;
        tasks[i].H = graph2;
    }

    printf("Starting threads...\n");
    pthread_t threads[NUM_OF_THREADS];
    for (int i = 0; i < NUM_OF_THREADS; i++)
    {
        struct Thread_Args *args = malloc(sizeof(struct Thread_Args));
        args->index = i;
        args->tasks_size = task_count;
        args->tasks = tasks;
        args->results = (struct Result *)results;
        pthread_create(&threads[i], NULL, worker_function, args);
    }
    printf("Threads started.\n");

    for (int i = 0; i < NUM_OF_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("All threads finished. Saving results...\n");
    save_results(results, task_count);
    printf("Results saved to results.csv\n");

    for (int i = 0; i < task_count; i++)
    {
        free(tasks[i].G);
        free(tasks[i].H);
    }

    free(tasks);
    free(results);
    return 0;
}