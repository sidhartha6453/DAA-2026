/*
 * mergesort_compare.c
 *
 * Compares standard (2-way) merge sort against a modified 3-way merge sort.
 *
 * Standard merge sort:   T(n) = 2T(n/2) + O(n)  ->  O(n log n)
 * Modified merge sort:   T(n) = 3T(n/3) + O(n)  ->  O(n log n)   (same order)
 *
 * The program times both algorithms on random arrays of increasing size
 * and writes the results (n, comparisons, time_seconds) for each algorithm
 * to a CSV file so the order of growth can be plotted and compared against
 * a reference n*log(n) curve.
 *
 * Compile:  gcc -O2 -o mergesort_compare mergesort_compare.c
 * Run:      ./mergesort_compare > results.csv
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

/* ---------- Global comparison counters (reset before each run) ---------- */
static long long g_comparisons_2way = 0;
static long long g_comparisons_3way = 0;

/* =========================================================================
 *  STANDARD (2-WAY) MERGE SORT
 * ========================================================================= */

/* Merge two sorted subarrays arr[lo..mid] and arr[mid+1..hi] using a
 * temporary buffer 'temp'. */
static void merge2(int *arr, int *temp, int lo, int mid, int hi) {
    int i = lo, j = mid + 1, k = lo;

    while (i <= mid && j <= hi) {
        g_comparisons_2way++;
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= hi)  temp[k++] = arr[j++];

    for (int t = lo; t <= hi; t++) arr[t] = temp[t];
}

static void mergeSort2Helper(int *arr, int *temp, int lo, int hi) {
    if (lo >= hi) return;
    int mid = lo + (hi - lo) / 2;
    mergeSort2Helper(arr, temp, lo, mid);
    mergeSort2Helper(arr, temp, mid + 1, hi);
    merge2(arr, temp, lo, mid, hi);
}

static void mergeSort2(int *arr, int n) {
    if (n <= 1) return;
    int *temp = (int *)malloc(n * sizeof(int));
    mergeSort2Helper(arr, temp, 0, n - 1);
    free(temp);
}

/* =========================================================================
 *  MODIFIED (3-WAY) MERGE SORT
 * ========================================================================= */

/* Merge three sorted subarrays:
 *   arr[lo   .. m1]
 *   arr[m1+1 .. m2]
 *   arr[m2+1 .. hi]
 * using a temporary buffer 'temp'.
 */
static void merge3(int *arr, int *temp, int lo, int m1, int m2, int hi) {
    int i = lo, j = m1 + 1, k = m2 + 1, t = lo;

    while (i <= m1 && j <= m2 && k <= hi) {
        /* find minimum of arr[i], arr[j], arr[k]  (<=2 comparisons) */
        if (arr[i] <= arr[j]) {
            g_comparisons_3way++;
            if (arr[i] <= arr[k]) {
                g_comparisons_3way++;
                temp[t++] = arr[i++];
            } else {
                g_comparisons_3way++;
                temp[t++] = arr[k++];
            }
        } else {
            g_comparisons_3way++;
            if (arr[j] <= arr[k]) {
                g_comparisons_3way++;
                temp[t++] = arr[j++];
            } else {
                g_comparisons_3way++;
                temp[t++] = arr[k++];
            }
        }
    }

    /* One list is exhausted; merge the remaining two, then drain the last */
    while (i <= m1 && j <= m2) {
        g_comparisons_3way++;
        if (arr[i] <= arr[j]) temp[t++] = arr[i++];
        else                  temp[t++] = arr[j++];
    }
    while (j <= m2 && k <= hi) {
        g_comparisons_3way++;
        if (arr[j] <= arr[k]) temp[t++] = arr[j++];
        else                  temp[t++] = arr[k++];
    }
    while (i <= m1 && k <= hi) {
        g_comparisons_3way++;
        if (arr[i] <= arr[k]) temp[t++] = arr[i++];
        else                  temp[t++] = arr[k++];
    }
    while (i <= m1) temp[t++] = arr[i++];
    while (j <= m2) temp[t++] = arr[j++];
    while (k <= hi) temp[t++] = arr[k++];

    for (int x = lo; x <= hi; x++) arr[x] = temp[x];
}

static void mergeSort3Helper(int *arr, int *temp, int lo, int hi) {
    if (lo >= hi) return;
    int len = hi - lo + 1;
    if (len < 3) {
        /* base case: fall back to a trivial 2-way split/merge for len==2 */
        int mid = lo + (hi - lo) / 2;
        mergeSort3Helper(arr, temp, lo, mid);
        mergeSort3Helper(arr, temp, mid + 1, hi);
        merge2(arr, temp, lo, mid, hi); /* not counted in g_comparisons_3way on purpose? see note */
        return;
    }
    int third = len / 3;
    int m1 = lo + third - 1;
    int m2 = lo + 2 * third - 1;

    mergeSort3Helper(arr, temp, lo, m1);
    mergeSort3Helper(arr, temp, m1 + 1, m2);
    mergeSort3Helper(arr, temp, m2 + 1, hi);
    merge3(arr, temp, lo, m1, m2, hi);
}

static void mergeSort3(int *arr, int n) {
    if (n <= 1) return;
    int *temp = (int *)malloc(n * sizeof(int));
    mergeSort3Helper(arr, temp, 0, n - 1);
    free(temp);
}

/* =========================================================================
 *  UTILITIES
 * ========================================================================= */

static int *generate_random_array(int n, unsigned int seed) {
    int *arr = (int *)malloc(n * sizeof(int));
    srand(seed);
    for (int i = 0; i < n; i++) arr[i] = rand();
    return arr;
}

static int is_sorted(int *arr, int n) {
    for (int i = 1; i < n; i++)
        if (arr[i - 1] > arr[i]) return 0;
    return 1;
}

/* =========================================================================
 *  MAIN: run both algorithms across a range of sizes, time them, and
 *  print CSV: n,time_2way_sec,comparisons_2way,time_3way_sec,comparisons_3way
 * ========================================================================= */

int main(void) {
    /* sizes to test - grows geometrically */
    int sizes[] = {1000, 2000, 4000, 8000, 16000, 32000, 64000,
                   128000, 256000, 512000, 1000000, 2000000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int trials = 3; /* average over a few trials to reduce noise */

    printf("n,time_2way_sec,comparisons_2way,time_3way_sec,comparisons_3way,nlogn_ref\n");

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        double total_time_2 = 0.0, total_time_3 = 0.0;
        long long total_cmp_2 = 0, total_cmp_3 = 0;

        for (int trial = 0; trial < trials; trial++) {
            unsigned int seed = (unsigned int)(1234 + trial * 37 + n);

            /* --- 2-way merge sort --- */
            int *arr2 = generate_random_array(n, seed);
            g_comparisons_2way = 0;
            clock_t start = clock();
            mergeSort2(arr2, n);
            clock_t end = clock();
            if (!is_sorted(arr2, n)) {
                fprintf(stderr, "ERROR: 2-way merge sort failed to sort n=%d\n", n);
            }
            total_time_2 += (double)(end - start) / CLOCKS_PER_SEC;
            total_cmp_2 += g_comparisons_2way;
            free(arr2);

            /* --- 3-way merge sort (same input, same seed) --- */
            int *arr3 = generate_random_array(n, seed);
            g_comparisons_3way = 0;
            start = clock();
            mergeSort3(arr3, n);
            end = clock();
            if (!is_sorted(arr3, n)) {
                fprintf(stderr, "ERROR: 3-way merge sort failed to sort n=%d\n", n);
            }
            total_time_3 += (double)(end - start) / CLOCKS_PER_SEC;
            total_cmp_3 += g_comparisons_3way;
            free(arr3);
        }

        double avg_time_2 = total_time_2 / trials;
        double avg_time_3 = total_time_3 / trials;
        long long avg_cmp_2 = total_cmp_2 / trials;
        long long avg_cmp_3 = total_cmp_3 / trials;
        double nlogn = (double)n * log2((double)n);

        printf("%d,%.6f,%lld,%.6f,%lld,%.2f\n",
               n, avg_time_2, avg_cmp_2, avg_time_3, avg_cmp_3, nlogn);
        fflush(stdout);
    }

    return 0;
}
