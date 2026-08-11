#include <stdio.h>
#include <stdlib.h>

long comp_count = 0;

int binarySearch(int arr[], int n, int x) {
    int low = 0, high = n - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        comp_count++;
        if (arr[mid] == x) return mid;
        comp_count++;
        if (arr[mid] < x) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

int ternarySearch(int arr[], int n, int x) {
    int low = 0, high = n - 1;
    while (low <= high) {
        int mid1 = low + (high - low) / 3;
        int mid2 = high - (high - low) / 3;
        comp_count++;
        if (arr[mid1] == x) return mid1;
        comp_count++;
        if (arr[mid2] == x) return mid2;
        comp_count++;
        if (x < arr[mid1]) high = mid1 - 1;
        else {
            comp_count++;
            if (x > arr[mid2]) low = mid2 + 1;
            else { low = mid1 + 1; high = mid2 - 1; }
        }
    }
    return -1;
}

int main() {
    int n;
    printf("Enter size of sorted array: ");
    scanf("%d", &n);

    int *arr = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) arr[i] = i * 2;

    int x;
    printf("Enter element to search: ");
    scanf("%d", &x);

    comp_count = 0;
    int idx = binarySearch(arr, n, x);
    printf("\nBinary Search: index = %d, comparisons = %ld\n", idx, comp_count);

    comp_count = 0;
    idx = ternarySearch(arr, n, x);
    printf("Ternary Search: index = %d, comparisons = %ld\n", idx, comp_count);

    printf("\nWorst-case comparisons for n = %d:\n", n);
    printf("%-15s%-15s%-15s\n", "Size(n)", "Binary(logn)", "Ternary(logn)");
    for (int size = 10; size <= 1000000; size *= 10) {
        int *a = malloc(size * sizeof(int));
        for (int i = 0; i < size; i++) a[i] = i * 2;

        comp_count = 0;
        binarySearch(a, size, -1);
        long bcomp = comp_count;

        comp_count = 0;
        ternarySearch(a, size, -1);
        long tcomp = comp_count;

        printf("%-15d%-15ld%-15ld\n", size, bcomp, tcomp);
        free(a);
    }

    free(arr);
    return 0;
}
