/*
 * mergek_compare.c
 *
 * Given k sorted arrays, each of size n, compares two ways of combining
 * them into one sorted array of size k*n:
 *
 *   Method 1 (sequential):  merge arr[0] with arr[1], merge that result
 *                            with arr[2], then with arr[3], ..., with arr[k-1].
 *                            Worst case: T(n,k) = Theta(n * k^2)
 *
 *   Method 2 (balanced/pairwise): repeatedly pair up all current arrays and
 *                            merge each pair (a queue-based implementation of
 *                            the balanced binary merge tree), until one array
 *                            remains.
 *                            Worst case: T(n,k) = Theta(n * k * log k)
 *
 * The program measures wall-clock time AND raw element-comparison counts
 * (comparisons are a noise-free proxy for running time) for both methods:
 *
 *   Experiment A: fix n, vary k       -> shows k^2 vs k*log(k) growth
 *   Experiment B: fix k, vary n       -> shows both are linear in n
 *
 * Results are written to two CSV files: results_vary_k.csv, results_vary_n.csv
 *
 * Compile:  gcc -O2 -o mergek_compare mergek_compare.c -lm
 * Run:      ./mergek_compare
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

static long long g_comparisons = 0;

/* ---------------------------------------------------------------------
 * Basic 2-way merge of two sorted int arrays a[0..na-1], b[0..nb-1]
 * into a freshly malloc'd array of size na+nb.
 * --------------------------------------------------------------------- */
static int *merge2(const int *a, int na, const int *b, int nb) {
    int *out = (int *)malloc((size_t)(na + nb) * sizeof(int));
    int i = 0, j = 0, k = 0;
    while (i < na && j < nb) {
        g_comparisons++;
        if (a[i] <= b[j]) out[k++] = a[i++];
        else               out[k++] = b[j++];
    }
    while (i < na) out[k++] = a[i++];
    while (j < nb) out[k++] = b[j++];
    return out;
}

/* ---------------------------------------------------------------------
 * METHOD 1: Sequential merging
 *   result = arrays[0]
 *   result = merge(result, arrays[1])
 *   result = merge(result, arrays[2])
 *   ...
 * arrays[]: array of k pointers, each pointing to a sorted array of size n.
 * Returns a newly malloc'd sorted array of size k*n. Original arrays
 * are left untouched (we work on copies internally).
 * --------------------------------------------------------------------- */
static int *mergeK_sequential(int **arrays, int k, int n) {
    /* start with a copy of arrays[0] */
    int *result = (int *)malloc((size_t)n * sizeof(int));
    memcpy(result, arrays[0], (size_t)n * sizeof(int));
    int result_len = n;

    for (int idx = 1; idx < k; idx++) {
        int *merged = merge2(result, result_len, arrays[idx], n);
        free(result);
        result = merged;
        result_len += n;
    }
    return result; /* size k*n */
}

/* ---------------------------------------------------------------------
 * METHOD 2: Balanced pairwise merging using a simple queue.
 * We keep a queue of (pointer, length) pairs. While more than one array
 * remains, dequeue two, merge them, enqueue the result. Because merges
 * are always performed on arrays currently at the FRONT of the queue,
 * arrays of equal size get merged with each other first, which
 * reproduces the round-by-round balanced binary tree even when k is not
 * a power of two.
 * --------------------------------------------------------------------- */
typedef struct {
    int *data;
    int len;
} Chunk;

static int *mergeK_balanced(int **arrays, int k, int n) {
    /* queue implemented as a simple circular buffer sized generously */
    int capacity = 4 * k + 8;
    Chunk *queue = (Chunk *)malloc((size_t)capacity * sizeof(Chunk));
    int head = 0, tail = 0, count = 0;

    /* enqueue helper (inline for clarity) */
    #define ENQUEUE(c) do { queue[tail] = (c); tail = (tail + 1) % capacity; count++; } while (0)
    #define DEQUEUE()  ( queue[head], head = (head + 1) % capacity, count-- ) /* placeholder not used directly */

    for (int i = 0; i < k; i++) {
        int *copy = (int *)malloc((size_t)n * sizeof(int));
        memcpy(copy, arrays[i], (size_t)n * sizeof(int));
        Chunk c = { copy, n };
        ENQUEUE(c);
    }

    while (count > 1) {
        /* dequeue first */
        Chunk c1 = queue[head]; head = (head + 1) % capacity; count--;
        /* dequeue second */
        Chunk c2 = queue[head]; head = (head + 1) % capacity; count--;

        int *merged = merge2(c1.data, c1.len, c2.data, c2.len);
        int merged_len = c1.len + c2.len;
        free(c1.data);
        free(c2.data);

        Chunk cm = { merged, merged_len };
        ENQUEUE(cm);
    }

    Chunk final = queue[head];
    free(queue);
    return final.data; /* size k*n */

    #undef ENQUEUE
    #undef DEQUEUE
}

/* ---------------------------------------------------------------------
 * Utilities
 * --------------------------------------------------------------------- */

/* Generate k sorted arrays of size n each (worst-case-ish: random data,
 * each array independently sorted). Returns array of k int* pointers. */
static int **generate_sorted_arrays(int k, int n, unsigned int seed) {
    int **arrays = (int **)malloc((size_t)k * sizeof(int *));
    srand(seed);
    for (int i = 0; i < k; i++) {
        int *a = (int *)malloc((size_t)n * sizeof(int));
        for (int j = 0; j < n; j++) a[j] = rand();
        /* simple insertion-free sort via qsort for speed */
        arrays[i] = a;
    }
    /* sort each array */
    for (int i = 0; i < k; i++) {
        int *a = arrays[i];
        /* qsort with a comparator */
        int cmp(const void *x, const void *y) {
            return (*(const int *)x - *(const int *)y);
        }
        qsort(a, (size_t)n, sizeof(int), cmp);
    }
    return arrays;
}

static void free_arrays(int **arrays, int k) {
    for (int i = 0; i < k; i++) free(arrays[i]);
    free(arrays);
}

static int is_sorted(const int *arr, int len) {
    for (int i = 1; i < len; i++) if (arr[i - 1] > arr[i]) return 0;
    return 1;
}

/* ---------------------------------------------------------------------
 * Experiment A: fix n, vary k
 * --------------------------------------------------------------------- */
static void experiment_vary_k(void) {
    FILE *f = fopen("results_vary_k.csv", "w");
    fprintf(f, "n,k,time_method1_sec,comparisons_method1,time_method2_sec,comparisons_method2,k2_ref,klogk_ref\n");

    int n = 500; /* fixed array size */
    int ks[] = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};
    int num_k = sizeof(ks) / sizeof(ks[0]);
    int trials = 3;

    for (int t = 0; t < num_k; t++) {
        int k = ks[t];
        double total_time1 = 0.0, total_time2 = 0.0;
        long long total_cmp1 = 0, total_cmp2 = 0;

        for (int trial = 0; trial < trials; trial++) {
            unsigned int seed = (unsigned int)(9000 + trial * 13 + k);
            int **arrays = generate_sorted_arrays(k, n, seed);

            g_comparisons = 0;
            clock_t start = clock();
            int *res1 = mergeK_sequential(arrays, k, n);
            clock_t end = clock();
            if (!is_sorted(res1, k * n)) fprintf(stderr, "ERROR method1 not sorted (k=%d,n=%d)\n", k, n);
            total_time1 += (double)(end - start) / CLOCKS_PER_SEC;
            total_cmp1 += g_comparisons;
            free(res1);

            g_comparisons = 0;
            start = clock();
            int *res2 = mergeK_balanced(arrays, k, n);
            end = clock();
            if (!is_sorted(res2, k * n)) fprintf(stderr, "ERROR method2 not sorted (k=%d,n=%d)\n", k, n);
            total_time2 += (double)(end - start) / CLOCKS_PER_SEC;
            total_cmp2 += g_comparisons;
            free(res2);

            free_arrays(arrays, k);
        }

        double avg_t1 = total_time1 / trials, avg_t2 = total_time2 / trials;
        long long avg_c1 = total_cmp1 / trials, avg_c2 = total_cmp2 / trials;
        double k2_ref = (double)n * (double)k * (double)k;
        double klogk_ref = (double)n * (double)k * log2((double)k);

        fprintf(f, "%d,%d,%.6f,%lld,%.6f,%lld,%.2f,%.2f\n",
                n, k, avg_t1, avg_c1, avg_t2, avg_c2, k2_ref, klogk_ref);
        printf("vary_k: n=%d k=%d  M1: t=%.6fs cmp=%lld   M2: t=%.6fs cmp=%lld\n",
               n, k, avg_t1, avg_c1, avg_t2, avg_c2);
    }
    fclose(f);
}

/* ---------------------------------------------------------------------
 * Experiment B: fix k, vary n
 * --------------------------------------------------------------------- */
static void experiment_vary_n(void) {
    FILE *f = fopen("results_vary_n.csv", "w");
    fprintf(f, "n,k,time_method1_sec,comparisons_method1,time_method2_sec,comparisons_method2,n_ref\n");

    int k = 64; /* fixed number of arrays */
    int ns[] = {200, 400, 800, 1600, 3200, 6400, 12800, 25600, 51200, 102400};
    int num_n = sizeof(ns) / sizeof(ns[0]);
    int trials = 3;

    for (int t = 0; t < num_n; t++) {
        int n = ns[t];
        double total_time1 = 0.0, total_time2 = 0.0;
        long long total_cmp1 = 0, total_cmp2 = 0;

        for (int trial = 0; trial < trials; trial++) {
            unsigned int seed = (unsigned int)(5000 + trial * 17 + n);
            int **arrays = generate_sorted_arrays(k, n, seed);

            g_comparisons = 0;
            clock_t start = clock();
            int *res1 = mergeK_sequential(arrays, k, n);
            clock_t end = clock();
            if (!is_sorted(res1, k * n)) fprintf(stderr, "ERROR method1 not sorted (k=%d,n=%d)\n", k, n);
            total_time1 += (double)(end - start) / CLOCKS_PER_SEC;
            total_cmp1 += g_comparisons;
            free(res1);

            g_comparisons = 0;
            start = clock();
            int *res2 = mergeK_balanced(arrays, k, n);
            end = clock();
            if (!is_sorted(res2, k * n)) fprintf(stderr, "ERROR method2 not sorted (k=%d,n=%d)\n", k, n);
            total_time2 += (double)(end - start) / CLOCKS_PER_SEC;
            total_cmp2 += g_comparisons;
            free(res2);

            free_arrays(arrays, k);
        }

        double avg_t1 = total_time1 / trials, avg_t2 = total_time2 / trials;
        long long avg_c1 = total_cmp1 / trials, avg_c2 = total_cmp2 / trials;
        double n_ref = (double)n;

        fprintf(f, "%d,%d,%.6f,%lld,%.6f,%lld,%.2f\n",
                n, k, avg_t1, avg_c1, avg_t2, avg_c2, n_ref);
        printf("vary_n: n=%d k=%d  M1: t=%.6fs cmp=%lld   M2: t=%.6fs cmp=%lld\n",
               n, k, avg_t1, avg_c1, avg_t2, avg_c2);
    }
    fclose(f);
}

int main(void) {
    printf("=== Experiment A: fixed n, varying k (expect M1 ~ k^2, M2 ~ k log k) ===\n");
    experiment_vary_k();
    printf("\n=== Experiment B: fixed k, varying n (expect both linear in n) ===\n");
    experiment_vary_n();
    printf("\nDone. Wrote results_vary_k.csv and results_vary_n.csv\n");
    return 0;
}
