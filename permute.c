#include <stdio.h>
#include <stdlib.h>

// Swap helper
static inline void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

// Recursive helper to generate permutations
void permutations(int *arr, int n, int idx) {
    if (idx == n) {
        // Print one permutation
        for (int i = 0; i < n; ++i) {
            printf("%d ", arr[i]);
        }
        printf("\n");
        return;
    }

    for (int i = idx; i < n; ++i) {
        swap(&arr[idx], &arr[i]);              // Choose
        permutations(arr, n, idx + 1);         // Explore
        swap(&arr[idx], &arr[i]);              // Backtrack
    }
}

// Wrapper to call permutations
void permute(int *arr, int n) {
    permutations(arr, n, 0);
}

int main(void) {
    int arr[] = {1, 2, 3};
    int n = sizeof(arr) / sizeof(arr[0]);
    permute(arr, n);
    return 0;
}
