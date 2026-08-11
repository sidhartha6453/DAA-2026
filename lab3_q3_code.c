#include <stdio.h>
#include <stdlib.h>

long comp_count = 0;

void maxMin(int arr[], int low, int high, int *max, int *min) {
    if (low == high) {
        *max = *min = arr[low];
        return;
    }
    if (high == low + 1) {
        comp_count++;
        if (arr[low] > arr[high]) { *max = arr[low]; *min = arr[high]; }
        else { *max = arr[high]; *min = arr[low]; }
        return;
    }
    int mid = (low + high) / 2;
    int lmax, lmin, rmax, rmin;
    maxMin(arr, low, mid, &lmax, &lmin);
    maxMin(arr, mid + 1, high, &rmax, &rmin);

    comp_count++;
    *max = (lmax > rmax) ? lmax : rmax;
    comp_count++;
    *min = (lmin < rmin) ? lmin : rmin;
}

int main() {
    int n;
    printf("Enter size of array: ");
    scanf("%d", &n);

    int *arr = malloc(n * sizeof(int));
    srand(42);
    for (int i = 0; i < n; i++) arr[i] = rand() % 100000;

    int max, min;
    comp_count = 0;
    maxMin(arr, 0, n - 1, &max, &min);
    printf("Max = %d, Min = %d\n", max, min);
    printf("Comparisons = %ld, Bound(3n/2) = %.1f\n\n", comp_count, 1.5 * n);
    free(arr);

    printf("%-15s%-20s%-15s\n", "Size(n)", "Comparisons", "3n/2");
    for (int size = 10; size <= 1000000; size *= 10) {
        int *a = malloc(size * sizeof(int));
        for (int i = 0; i < size; i++) a[i] = rand() % 1000000;

        int mx, mn;
        comp_count = 0;
        maxMin(a, 0, size - 1, &mx, &mn);
        printf("%-15d%-20ld%-15.1f\n", size, comp_count, 1.5 * size);
        free(a);
    }
    return 0;
}
