/* Pseudocode: 
SELECTION-SORT(A, n)
  for i = 1 to n - 1
      min = i
      for j = i + 1 to n
          if A[j] < A[min]
              min = j
      exchange A[i] with A[min]

Loop invariant: At the start of each iteration of the outer loop (indexed by i), the subarray A[1‥i−1] contains the i−1 smallest elements of the whole array, in sorted order.
Initialization: Before the first iteration (i=1), A[1‥0] is empty — vacuously the "0 smallest elements, sorted." True.
Maintenance: Given A[1‥i−1] holds the i−1 smallest elements sorted, the inner loop finds the minimum of the remaining A[i‥n] and swaps it into A[i]. Now A[1‥i] holds the i smallest elements sorted, restoring the invariant for i+1.
Termination: The loop ends when i = n. The invariant then says A[1‥n−1] holds the n−1 smallest elements sorted — and since A[n] must be the single remaining (and therefore largest) element, the whole array A[1‥n] is sorted.

Why only the first n−1 elements need processing: after n−1 iterations, all elements except the last are placed correctly, and only one element (necessarily the maximum) remains in A[n] — it's already in its correct final position by elimination, so a final "find the min of a 1-element subarray" pass would be redundant.
Running time: the inner loop always runs, regardless of input order — it scans every unexamined element to find the minimum. Total comparisons = (n−1) + (n−2) + ... + 1 = n(n−1)/2, so both worst-case and best-case running time are Θ(n²). Unlike insertion sort, selection sort gets no benefit from an already-sorted input, since it doesn't stop scanning early — the comparison count is fixed by n alone.
Validation: the program sorts a user-supplied array, verifies correctness, then benchmarks comparisons across random, sorted, and reverse-sorted inputs for n up to 3200. The comparison count is identical in all three cases and matches n(n−1)/2 exactly (e.g., n=3200 → 5,118,400 for every input order) — confirming Θ(n²) in both the best and worst case, with no best-case speedup
*/


#include <stdio.h>
#include <stdlib.h>

long comp_count = 0, swap_count = 0;

void selectionSort(int A[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            comp_count++;
            if (A[j] < A[minIdx]) minIdx = j;
        }
        if (minIdx != i) {
            int t = A[i]; A[i] = A[minIdx]; A[minIdx] = t;
            swap_count++;
        }
    }
}

int isSorted(int A[], int n) {
    for (int i = 1; i < n; i++) if (A[i - 1] > A[i]) return 0;
    return 1;
}

void fillArray(int A[], int n, int mode) {
    for (int i = 0; i < n; i++) {
        if (mode == 0) A[i] = rand() % 100000;      /* random    -> worst case */
        else if (mode == 1) A[i] = i;                /* sorted    -> best case  */
        else A[i] = n - i;                            /* reverse sorted          */
    }
}

int main() {
    int n;
    printf("Enter size of array: ");
    scanf("%d", &n);

    int *arr = malloc(n * sizeof(int));
    printf("Enter %d elements: ", n);
    for (int i = 0; i < n; i++) scanf("%d", &arr[i]);

    comp_count = swap_count = 0;
    selectionSort(arr, n);

    printf("\nSorted array: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\nSorted correctly: %s\n", isSorted(arr, n) ? "YES" : "NO");
    printf("Comparisons = %ld, Swaps = %ld\n\n", comp_count, swap_count);
    free(arr);

    srand(1);
    printf("%-10s%-15s%-20s%-15s\n", "n", "Case", "Comparisons", "n(n-1)/2");
    for (int size = 100; size <= 3200; size *= 2) {
        for (int mode = 0; mode <= 2; mode++) {
            int *a = malloc(size * sizeof(int));
            fillArray(a, size, mode);
            comp_count = swap_count = 0;
            selectionSort(a, size);
            const char *label = mode == 0 ? "random" : mode == 1 ? "sorted(best)" : "reverse(worst)";
            printf("%-10d%-15s%-20ld%-15ld\n", size, label, comp_count, (long)size * (size - 1) / 2);
            free(a);
        }
    }
    return 0;
}
