#include <stdio.h>
#include <stdlib.h>

int cmp(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

/* Binary search for target in sorted array; returns index or -1 */
int binarySearch(int arr[], int n, int target) {
    int lo = 0, hi = n - 1;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;

        if (arr[mid] == target) return mid;
        if (arr[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }

    return -1;
}

/* O(n log n): sort S1, then for each element of S2 binary-search
   for (x - element) in S1 */
int findPair(int S1[], int n1, int S2[], int n2, int x, int *a, int *b) {
    int *sorted1 = malloc(n1 * sizeof(int));

    for (int i = 0; i < n1; i++)
        sorted1[i] = S1[i];

    qsort(sorted1, n1, sizeof(int), cmp);

    for (int j = 0; j < n2; j++) {
        int need = x - S2[j];

        if (binarySearch(sorted1, n1, need) != -1) {
            *a = need;
            *b = S2[j];
            free(sorted1);
            return 1;
        }
    }

    free(sorted1);
    return 0;
}

/* O(n^2) brute force, used only to validate correctness */
int bruteForce(int S1[], int n1, int S2[], int n2, int x) {
    for (int i = 0; i < n1; i++)
        for (int j = 0; j < n2; j++)
            if (S1[i] + S2[j] == x)
                return 1;

    return 0;
}

int main() {
    int n1, n2, x;

    printf("Enter size of S1: ");
    scanf("%d", &n1);

    int *S1 = malloc(n1 * sizeof(int));

    printf("Enter %d elements of S1: ", n1);
    for (int i = 0; i < n1; i++)
        scanf("%d", &S1[i]);

    printf("Enter size of S2: ");
    scanf("%d", &n2);

    int *S2 = malloc(n2 * sizeof(int));

    printf("Enter %d elements of S2: ", n2);
    for (int i = 0; i < n2; i++)
        scanf("%d", &S2[i]);

    printf("Enter target sum x: ");
    scanf("%d", &x);

    int a, b;

    int found = findPair(S1, n1, S2, n2, x, &a, &b);

    if (found)
        printf("\nPair found: %d (from S1) + %d (from S2) = %d\n",
               a, b, x);
    else
        printf("\nNo such pair exists\n");

    free(S1);
    free(S2);

    srand(1);

    printf("\nRandomised validation against brute force:\n");

    int mismatches = 0;

    for (int size = 10; size <= 5000; size *= 10) {
        for (int trial = 0; trial < 5; trial++) {

            int *a1 = malloc(size * sizeof(int));
            int *a2 = malloc(size * sizeof(int));

            for (int i = 0; i < size; i++) {
                a1[i] = rand() % 1000;
                a2[i] = rand() % 1000;
            }

            int target = rand() % 2000;

            int r1 = findPair(a1, size, a2, size, target, &a, &b);
            int r2 = bruteForce(a1, size, a2, size, target);

            if (r1 != r2)
                mismatches++;

            free(a1);
            free(a2);
        }

        printf("n = %-6d 5 trials done\n", size);
    }

    printf("Mismatches with brute force = %d\n", mismatches);

    return 0;
}