/*
 * DAA LAB-5, Q2: Find the K'th smallest element WITHOUT sorting the list.
 *
 * PSEUDOCODE:
 *   RANDOMIZED-PARTITION(A, low, high):
 *       pick random pivot index p in [low,high]; swap A[p], A[high]
 *       pivot = A[high]; i = low - 1
 *       for j = low to high-1:
 *           if A[j] <= pivot: i++; swap A[i], A[j]
 *       swap A[i+1], A[high]
 *       return i+1
 *
 *   QUICKSELECT(A, low, high, k):     // k is 0-indexed rank wanted
 *       if low == high: return A[low]
 *       p = RANDOMIZED-PARTITION(A, low, high)
 *       if k == p: return A[p]
 *       else if k < p: return QUICKSELECT(A, low, p-1, k)
 *       else: return QUICKSELECT(A, p+1, high, k)
 *
 *   To get the K-th smallest (1-indexed), call QUICKSELECT(A, 0, N-1, K-1)
 *
 * COMPLEXITY ANALYSIS:
 *   Partition step: O(n) per call.
 *   Only one side is recursed into, so average subarray size halves each call:
 *       Average case: T(n) = T(n/2) + O(n) => O(n)
 *       Worst case (unlucky pivots): T(n) = T(n-1) + O(n) => O(n^2)
 *   Space: O(1) extra (in-place), O(log n) avg recursion stack.
 *   Overall: O(n) average, O(n^2) worst case.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *a, int *b) { int t = *a; *a = *b; *b = t; }

int partition(int A[], int low, int high) {
    int r = low + rand() % (high - low + 1);
    swap(&A[r], &A[high]);
    int pivot = A[high], i = low - 1;
    for (int j = low; j < high; j++)
        if (A[j] <= pivot) { i++; swap(&A[i], &A[j]); }
    swap(&A[i + 1], &A[high]);
    return i + 1;
}

int quickSelect(int A[], int low, int high, int k) {
    if (low == high) return A[low];
    int p = partition(A, low, high);
    if (k == p) return A[p];
    else if (k < p) return quickSelect(A, low, p - 1, k);
    else return quickSelect(A, p + 1, high, k);
}

int main() {
    int n, k;
    srand(time(NULL));
    printf("Enter N: ");
    scanf("%d", &n);
    int *A = malloc(n * sizeof(int));
    printf("Enter %d numbers: ", n);
    for (int i = 0; i < n; i++) scanf("%d", &A[i]);
    printf("Enter K: ");
    scanf("%d", &k);

    if (k < 1 || k > n) {
        printf("Invalid K\n");
    } else {
        int result = quickSelect(A, 0, n - 1, k - 1);
        printf("%d-th smallest element = %d\n", k, result);
    }
    free(A);
    return 0;
}
