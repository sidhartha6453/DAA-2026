/*
 * DAA LAB-5, Q3: Quick Sort of N random elements stored in a file.
 *
 * PSEUDOCODE:
 *   GENERATE: create N random integers, write to "input.txt" (one per line)
 *
 *   PARTITION(A, low, high):
 *       pivot = A[high]; i = low - 1
 *       for j = low to high-1:
 *           if A[j] <= pivot: i++; swap A[i], A[j]
 *       swap A[i+1], A[high]
 *       return i+1
 *
 *   QUICKSORT(A, low, high):
 *       if low < high:
 *           p = PARTITION(A, low, high)
 *           QUICKSORT(A, low, p-1)
 *           QUICKSORT(A, p+1, high)
 *
 *   MAIN: read N numbers from "input.txt" into array,
 *         QUICKSORT(array), write sorted array to "output.txt"
 *
 * COMPLEXITY ANALYSIS:
 *   Partition: O(n) per call.
 *   Recurrence: T(n) = T(k) + T(n-1-k) + O(n), k = size of left partition.
 *       Best/Average case (balanced splits, k ~ n/2): T(n) = O(n log n)
 *       Worst case (already sorted / bad pivot, k = 0 or n-1): T(n) = O(n^2)
 *   Space: O(log n) avg / O(n) worst case recursion stack (in-place sort).
 *   File I/O: O(n) to read + O(n) to write.
 *   Overall: O(n log n) average, O(n^2) worst case.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *a, int *b) { int t = *a; *a = *b; *b = t; }

int partition(int A[], int low, int high) {
    int pivot = A[high], i = low - 1;
    for (int j = low; j < high; j++)
        if (A[j] <= pivot) { i++; swap(&A[i], &A[j]); }
    swap(&A[i + 1], &A[high]);
    return i + 1;
}

void quickSort(int A[], int low, int high) {
    if (low < high) {
        int p = partition(A, low, high);
        quickSort(A, low, p - 1);
        quickSort(A, p + 1, high);
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

    /* Step 3: quick sort */
    quickSort(A, 0, n - 1);

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
