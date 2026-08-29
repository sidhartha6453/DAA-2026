/*
 * DAA LAB-5, Q1: Find median of N numbers WITHOUT sorting the list.
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
 *   QUICKSELECT(A, low, high, k):        // returns k-th smallest (0-indexed)
 *       if low == high: return A[low]
 *       p = RANDOMIZED-PARTITION(A, low, high)
 *       if k == p: return A[p]
 *       else if k < p: return QUICKSELECT(A, low, p-1, k)
 *       else: return QUICKSELECT(A, p+1, high, k)
 *
 *   MEDIAN(A, N):
 *       if N is odd:  return QUICKSELECT(A, 0, N-1, N/2)
 *       if N is even: return avg( QUICKSELECT(.., N/2 - 1), QUICKSELECT(.., N/2) )
 *
 * COMPLEXITY ANALYSIS:
 *   Partition:            O(n) for a call on a subarray of size n
 *   Quickselect recurses only into ONE side (unlike quicksort's two),
 *   so on average the subarray size shrinks geometrically (n, n/2, n/4, ...)
 *       -> Average case: T(n) = T(n/2) + O(n)  => O(n)
 *       -> Worst case (bad pivot every time): T(n) = T(n-1) + O(n) => O(n^2)
 *   Using median-of-medians pivot selection guarantees O(n) worst case
 *   (not implemented here, but random pivot gives O(n) expected time).
 *   Overall median-finding: O(n) average / O(n^2) worst case, O(1) extra space.
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
    int n;
    srand(time(NULL));
    printf("Enter N: ");
    scanf("%d", &n);
    int *A = malloc(n * sizeof(int));
    printf("Enter %d numbers: ", n);
    for (int i = 0; i < n; i++) scanf("%d", &A[i]);

    if (n % 2 == 1) {
        int med = quickSelect(A, 0, n - 1, n / 2);
        printf("Median = %d\n", med);
    } else {
        int a = quickSelect(A, 0, n - 1, n / 2 - 1);
        int b = quickSelect(A, 0, n - 1, n / 2);
        printf("Median = %.2f\n", (a + b) / 2.0);
    }
    free(A);
    return 0;
}
