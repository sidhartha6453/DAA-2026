#include <stdio.h>
#include <stdlib.h>

int cmp(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

int *S;
int n;
long bsearch_calls = 0;

/* binary search for target within S[lo..hi] */
int binarySearchRange(int lo, int hi, long target) {
    bsearch_calls++;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;

        if (S[mid] == target)
            return mid;

        if (S[mid] < target)
            lo = mid + 1;
        else
            hi = mid - 1;
    }

    return -1;
}

/* O(n log n): for each i, binary-search S[i+1..n-1]
   for (target - S[i]) */
int twoSum(int start, long target, int *idx) {
    for (int i = start; i < n - 1; i++) {
        int j = binarySearchRange(i + 1, n - 1,
                                  target - S[i]);

        if (j != -1) {
            idx[0] = i;
            idx[1] = j;
            return 1;
        }
    }

    return 0;
}

/* Fixes k-2 elements via recursion (n^(k-2) combinations),
   then solves the final pair with twoSum
   -> overall O(n^(k-1) * log n). */
int solveK(int start, int remaining, long target, int *idx) {

    if (remaining == 1) {
        int pos = binarySearchRange(start, n - 1, target);

        if (pos == -1)
            return 0;

        idx[0] = pos;
        return 1;
    }

    if (remaining == 2)
        return twoSum(start, target, idx);

    for (int i = start; i <= n - remaining; i++) {

        if (solveK(i + 1,
                   remaining - 1,
                   target - S[i],
                   idx + 1)) {

            idx[0] = i;
            return 1;
        }
    }

    return 0;
}

/* Plain O(n^k) brute force, used only to validate correctness */
int bruteHelper(int start, int remaining, long target,
                int a[], int n2) {

    if (remaining == 0)
        return target == 0;

    for (int i = start; i <= n2 - remaining; i++)
        if (bruteHelper(i + 1,
                        remaining - 1,
                        target - a[i],
                        a, n2))
            return 1;

    return 0;
}

int bruteForce(int a[], int n2, int k, long T) {
    return bruteHelper(0, k, T, a, n2);
}

int main() {

    int k;
    long T;

    printf("Enter n: ");
    scanf("%d", &n);

    S = malloc(n * sizeof(int));

    printf("Enter %d integers: ", n);
    for (int i = 0; i < n; i++)
        scanf("%d", &S[i]);

    printf("Enter k: ");
    scanf("%d", &k);

    printf("Enter target T: ");
    scanf("%ld", &T);

    qsort(S, n, sizeof(int), cmp);

    int *idx = malloc(k * sizeof(int));

    bsearch_calls = 0;

    int found = solveK(0, k, T, idx);

    if (found) {
        printf("\nFound %d numbers summing to %ld: ",
               k, T);

        for (int i = 0; i < k; i++)
            printf("%d ", S[idx[i]]);

        printf("\n");
    }
    else {
        printf("\nNo %d numbers sum to %ld\n",
               k, T);
    }

    printf("Binary search calls used = %ld\n",
           bsearch_calls);

    free(idx);
    free(S);

    srand(1);

    printf("\nValidation against brute force (small n):\n");

    int mismatches = 0;

    for (int size = 6; size <= 12; size++) {

        for (int kk = 2; kk <= 4; kk++) {

            int *a = malloc(size * sizeof(int));

            for (int i = 0; i < size; i++)
                a[i] = rand() % 20;

            long target = (rand() % 40) - 10;

            S = malloc(size * sizeof(int));

            for (int i = 0; i < size; i++)
                S[i] = a[i];

            n = size;

            qsort(S, n, sizeof(int), cmp);

            int *id = malloc(kk * sizeof(int));

            int r1 = solveK(0, kk, target, id);
            int r2 = bruteForce(a, size, kk, target);

            if (r1 != r2) {
                mismatches++;
                printf("MISMATCH n=%d k=%d T=%ld\n",
                       size, kk, target);
            }

            free(id);
            free(S);
            free(a);
        }
    }

    printf("Total mismatches = %d\n\n", mismatches);

    printf("Worst-case binary search calls vs n^(k-1) (no k-subset sums to T):\n");

    int kfix = 3;

    printf("%-10s%-20s%-15s\n",
           "n", "bsearch_calls", "n^(k-1)");

    for (int size = 10; size <= 200; size += 30) {

        S = malloc(size * sizeof(int));

        for (int i = 0; i < size; i++)
            S[i] = 2 * i;

        n = size;

        qsort(S, n, sizeof(int), cmp);

        int *id = malloc(kfix * sizeof(int));

        bsearch_calls = 0;

        solveK(0, kfix, 1, id);

        long bound = 1;

        for (int p = 0; p < kfix - 1; p++)
            bound *= size;

        printf("%-10d%-20ld%-15ld\n",
               size, bsearch_calls, bound);

        free(id);
        free(S);
    }

    return 0;
}