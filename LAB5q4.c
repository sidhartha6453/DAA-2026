/*
 * DAA LAB-5, Q4: Heap Sort of N randomly generated elements stored in a file.
 *
 * PSEUDOCODE:
 *   GENERATE: create N random integers, write to "input.txt"
 *
 *   HEAPIFY(A, n, i):          // sift-down, max-heap of size n rooted at i
 *       largest = i; l = 2*i+1; r = 2*i+2
 *       if l < n and A[l] > A[largest]: largest = l
 *       if r < n and A[r] > A[largest]: largest = r
 *       if largest != i:
 *           swap A[i], A[largest]
 *           HEAPIFY(A, n, largest)
 *
 *   BUILD-MAX-HEAP(A, n):
 *       for i = n/2 - 1 down to 0: HEAPIFY(A, n, i)
 *
 *   HEAPSORT(A, n):
 *       BUILD-MAX-HEAP(A, n)
 *       for i = n-1 down to 1:
 *           swap A[0], A[i]        // move max to end
 *           HEAPIFY(A, i, 0)       // restore heap on reduced size i
 *
 *   MAIN: read N numbers from "input.txt", HEAPSORT(array),
 *         write sorted array to "output.txt"
 *
 * COMPLEXITY ANALYSIS:
 *   HEAPIFY on a subtree of size n: O(log n)  (height of heap)
 *   BUILD-MAX-HEAP: n/2 calls to heapify, but tighter analysis gives O(n)
 *                   (not n log n) because most nodes are near the bottom.
 *   Main loop: n-1 iterations, each does a swap O(1) + heapify O(log n)
 *              => O(n log n)
 *   Overall: O(n) [build heap] + O(n log n) [extract max n-1 times]
 *          = O(n log n) in best, average, AND worst case (no bad-input case).
 *   Space: O(1) extra (in-place), O(log n) recursion stack for heapify.
 *   File I/O: O(n) read + O(n) write.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *a, int *b) { int t = *a; *a = *b; *b = t; }

void heapify(int A[], int n, int i) {
    int largest = i, l = 2 * i + 1, r = 2 * i + 2;
    if (l < n && A[l] > A[largest]) largest = l;
    if (r < n && A[r] > A[largest]) largest = r;
    if (largest != i) {
        swap(&A[i], &A[largest]);
        heapify(A, n, largest);
    }
}

void heapSort(int A[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) heapify(A, n, i);
    for (int i = n - 1; i > 0; i--) {
        swap(&A[0], &A[i]);
        heapify(A, i, 0);
    }
}

int main() {
    int n;
    srand(time(NULL));
    printf("Enter N (number of random elements): ");
    scanf("%d", &n);

    /* Step 1: generate N random elements and store in file */
    FILE *fin = fopen("input.txt", "w");
    for (int i = 0; i < n; i++)
        fprintf(fin, "%d\n", rand() % 10000);
    fclose(fin);

    /* Step 2: read elements back from file */
    int *A = malloc(n * sizeof(int));
    fin = fopen("input.txt", "r");
    for (int i = 0; i < n; i++) fscanf(fin, "%d", &A[i]);
    fclose(fin);

    /* Step 3: heap sort */
    heapSort(A, n);

    /* Step 4: write sorted result to output file */
    FILE *fout = fopen("output.txt", "w");
    for (int i = 0; i < n; i++) fprintf(fout, "%d\n", A[i]);
    fclose(fout);

    printf("Sorted (first %d shown):\n", n < 20 ? n : 20);
    for (int i = 0; i < n && i < 20; i++) printf("%d ", A[i]);
    printf("\nFull result written to output.txt\n");

    free(A);
    return 0;
}
