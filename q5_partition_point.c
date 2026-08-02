/*
 * DAA Lab-01, Q5: Find the partition point
 * ---------------------------------------------------------------
 * We're given an array that looks like this:
 *     0 0 0 0 0 1 1 1 1
 * (some 0's, then some 1's). We want to find exactly where the
 * 1's start.
 *
 * The slow way: walk through the array one element at a time until
 * we hit a 1. That takes Theta(n) time in the worst case.
 *
 * The fast way: since the array is already sorted (all the 0's come
 * before all the 1's), we can use BINARY SEARCH -- the same trick
 * you'd use to find a word in a dictionary. Look at the middle
 * element:
 *   - If it's a 1, the answer is at this position or somewhere to
 *     the left, so search the left half (including the middle).
 *   - If it's a 0, the answer must be somewhere to the right, so
 *     search the right half.
 * This takes only Theta(log n) time -- much faster for large arrays.
 */

#include <stdio.h>
#include <stdlib.h>

/* The slow way: check each element one by one.
   Returns the index of the first 1, or "size" if there is no 1 at all. */
int find_partition_slow(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == 1) {
            return i;
        }
    }
    return size;   /* no 1 found -- the whole array is 0's */
}

/* The fast way: binary search for the first 1.
   Returns the index of the first 1, or "size" if there is no 1 at all. */
int find_partition_fast(int arr[], int size) {
    int low = 0;
    int high = size;   /* "size" is our starting guess for "no 1 found" */

    while (low < high) {
        int middle = (low + high) / 2;

        if (arr[middle] == 1) {
            high = middle;       /* first 1 is here or to the left */
        } else {
            low = middle + 1;    /* first 1 must be further right */
        }
    }

    return low;   /* low and high have met -- this is the answer */
}

/* Build a test array: "num_zeros" zeros followed by 1's, total "size" long */
void build_test_array(int arr[], int size, int num_zeros) {
    for (int i = 0; i < size; i++) {
        arr[i] = (i < num_zeros) ? 0 : 1;
    }
}

int main(void) {
    printf("=== Checking that both methods agree on several test cases ===\n");
    printf("%-8s %-12s %-14s %-14s %-8s\n", "size", "num_zeros", "slow method", "fast method", "match?");

    int test_sizes[]      = {10, 10, 10, 1, 1, 1000, 100000};
    int test_num_zeros[]  = {4,  0,  10, 0, 1, 371,  63000};
    int num_tests = sizeof(test_sizes) / sizeof(test_sizes[0]);

    for (int t = 0; t < num_tests; t++) {
        int size = test_sizes[t];
        int zeros = test_num_zeros[t];

        int *arr = malloc(size * sizeof(int));
        build_test_array(arr, size, zeros);

        int slow_answer = find_partition_slow(arr, size);
        int fast_answer = find_partition_fast(arr, size);

        printf("%-8d %-12d %-14d %-14d %-8s\n",
               size, zeros, slow_answer, fast_answer,
               (slow_answer == fast_answer) ? "yes" : "NO -- BUG!");

        free(arr);
    }

    printf("\n=== A small example you can see with your own eyes ===\n");
    int example_size = 15;
    int *example = malloc(example_size * sizeof(int));
    build_test_array(example, example_size, 7);

    printf("Array: ");
    for (int i = 0; i < example_size; i++) {
        printf("%d ", example[i]);
    }
    printf("\nThe first 1 is at index %d\n", find_partition_fast(example, example_size));
    free(example);

    printf("\nWhy this matters: on a HUGE array, the slow method might have\n");
    printf("to check millions of elements, but binary search only needs\n");
    printf("about log2(millions) ~ 20 checks. That's the power of binary\n");
    printf("search on sorted data.\n");

    return 0;
}
