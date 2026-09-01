/* Sorting via reversal - Part1: O(n) reversal count. Part2: O(n log^2 n) reversal cost.
 * See writeup for pseudocode/proofs. */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

static long g_reversalCount = 0;
static long long g_reversalCost = 0;

void reverse(int p[], int i, int j) {                    /* cost = j-i+1 */
    g_reversalCount++;
    g_reversalCost += (j - i + 1);
    while (i < j) { int t = p[i]; p[i] = p[j]; p[j] = t; i++; j--; }
}

void printArray(const char *label, int p[], int n) {
    printf("%s [", label);
    for (int i = 0; i < n; i++) printf("%d%s", p[i], i == n - 1 ? "" : ", ");
    printf("]\n");
}
int isSorted(int p[], int n) { for (int i = 0; i+1 < n; i++) if (p[i] > p[i+1]) return 0; return 1; }

/* Part 1: selection-sort by reversal -- <= n-1 reversal OPERATIONS */
void selectionSortByReversal(int p[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int target = i + 1, pos = -1;
        for (int k = i; k < n; k++) if (p[k] == target) { pos = k; break; }
        if (pos != i) reverse(p, i, pos);
    }
}

/* Part 2: merge sort using rotate-by-3-reversals + binary search -- O(n log^2 n) total COST */
void rotate3(int p[], int l, int m, int r) {              /* swap [l,m) and [m,r); cost O(r-l) */
    if (l >= m || m >= r) return;
    reverse(p, l, m - 1);
    reverse(p, m, r - 1);
    reverse(p, l, r - 1);
}

int lowerBound(int p[], int l, int r, int key) {
    int lo = l, hi = r;
    while (lo < hi) { int mid = lo + (hi-lo)/2; if (p[mid] < key) lo = mid+1; else hi = mid; }
    return lo;
}

void mergeByRotate(int p[], int l, int m, int r) {        /* O((r-l) log(r-l)) */
    if (l >= m || m >= r) return;
    int lenL = m - l, lenR = r - m;

    /* size-1 base cases avoid degenerate zero-progress recursion */
    if (lenL == 1) { int pos = lowerBound(p, m, r, p[l]); rotate3(p, l, m, pos); return; }
    if (lenR == 1) { int pos = lowerBound(p, l, m, p[m]); rotate3(p, pos, m, m+1); return; }

    int mid1, mid2, pivot;
    if (lenL <= lenR) { mid1 = l + lenL/2; pivot = p[mid1]; mid2 = lowerBound(p, m, r, pivot); }
    else { mid2 = m + lenR/2; pivot = p[mid2]; mid1 = lowerBound(p, l, m, pivot); }

    rotate3(p, mid1, m, mid2);
    int newMid = mid1 + (mid2 - m);
    mergeByRotate(p, l, mid1, newMid);
    mergeByRotate(p, newMid, mid2, r);
}

void mergeSortByRotate(int p[], int l, int r) {
    if (r - l <= 1) return;
    int mid = l + (r - l) / 2;
    mergeSortByRotate(p, l, mid);
    mergeSortByRotate(p, mid, r);
    mergeByRotate(p, l, mid, r);
}

void generatePermutation(int p[], int n) {
    for (int i = 0; i < n; i++) p[i] = i + 1;
    for (int i = n - 1; i > 0; i--) { int j = rand() % (i+1); int t = p[i]; p[i] = p[j]; p[j] = t; }
}

int main(void) {
    srand(42);

    int ex[] = {1, 4, 3, 2, 5};
    int en = sizeof(ex) / sizeof(ex[0]);
    g_reversalCount = 0; g_reversalCost = 0;
    printArray("Example input:", ex, en);
    mergeSortByRotate(ex, 0, en);
    printArray("Sorted:", ex, en);
    printf("reversals=%ld cost=%lld sorted=%s\n\n", g_reversalCount, g_reversalCost, isSorted(ex, en) ? "YES" : "NO");

    printf("=== Part 1: O(n) reversal COUNT ===\n%8s %15s %15s %10s\n", "n", "#reversals", "n-1", "sorted?");
    for (int n = 100; n <= 6400; n *= 2) {
        int *p = malloc(n * sizeof(int));
        generatePermutation(p, n);
        g_reversalCount = 0; g_reversalCost = 0;
        selectionSortByReversal(p, n);
        printf("%8d %15ld %15d %10s\n", n, g_reversalCount, n-1, isSorted(p, n) ? "yes" : "NO");
        free(p);
    }

    printf("\n=== Part 2: O(n log^2 n) total COST ===\n%8s %15s %18s %12s %10s\n", "n", "total cost", "n*log2(n)^2", "cost/bound", "sorted?");
    for (int n = 100; n <= 6400; n *= 2) {
        int *p = malloc(n * sizeof(int));
        generatePermutation(p, n);
        g_reversalCount = 0; g_reversalCost = 0;
        mergeSortByRotate(p, 0, n);
        double bound = n * pow(log2((double) n), 2.0);
        printf("%8d %15lld %18.1f %12.3f %10s\n", n, g_reversalCost, bound, g_reversalCost / bound, isSorted(p, n) ? "yes" : "NO");
        free(p);
    }
    return 0;
}