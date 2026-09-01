/* 1D array operations - see complexity table in accompanying writeup */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

void printArray(const char *label, int arr[], int n) {
    printf("%s [", label);
    for (int i = 0; i < n; i++) printf("%d%s", arr[i], i == n - 1 ? "" : ", ");
    printf("]\n");
}

int findMax(int arr[], int n) {                       /* O(n) */
    int mx = arr[0];
    for (int i = 1; i < n; i++) if (arr[i] > mx) mx = arr[i];
    return mx;
}

void findFirstSecondLargest(int arr[], int n, int *first, int *second) { /* O(n) */
    *first = arr[0] > arr[1] ? arr[0] : arr[1];
    *second = arr[0] > arr[1] ? arr[1] : arr[0];
    for (int i = 2; i < n; i++) {
        if (arr[i] > *first) { *second = *first; *first = arr[i]; }
        else if (arr[i] > *second) *second = arr[i];
    }
}

double findMean(int arr[], int n) {                    /* O(n) */
    long long sum = 0;
    for (int i = 0; i < n; i++) sum += arr[i];
    return (double) sum / n;
}

int cmp_int(const void *a, const void *b) { return *(int *)a - *(int *)b; }

double findMedian(int arr[], int n) {                  /* O(n log n): sort + O(1) lookup */
    int *tmp = malloc(n * sizeof(int));
    memcpy(tmp, arr, n * sizeof(int));
    qsort(tmp, n, sizeof(int), cmp_int);
    double median = (n % 2 == 0) ? (tmp[n/2-1] + tmp[n/2]) / 2.0 : tmp[n/2];
    free(tmp);
    return median;
}

double findStdDev(int arr[], int n) {                  /* O(n): mean pass + deviation pass */
    double mean = findMean(arr, n), s = 0;
    for (int i = 0; i < n; i++) { double d = arr[i] - mean; s += d * d; }
    return sqrt(s / n);
}

#define HASH_SIZE 1024
typedef struct HashNode { int key, count; struct HashNode *next; } HashNode;
unsigned hashFunc(int key) { return (unsigned) key % HASH_SIZE; }

int findMode(int arr[], int n) {                       /* O(n) average via hashing */
    HashNode *table[HASH_SIZE] = {0};
    int modeVal = arr[0], modeCount = 0;
    for (int i = 0; i < n; i++) {
        unsigned h = hashFunc(arr[i]);
        HashNode *node = table[h];
        while (node && node->key != arr[i]) node = node->next;
        if (!node) {
            node = malloc(sizeof(HashNode));
            *node = (HashNode){arr[i], 1, table[h]};
            table[h] = node;
        } else node->count++;
        if (node->count > modeCount) { modeCount = node->count; modeVal = node->key; }
    }
    for (int i = 0; i < HASH_SIZE; i++)
        for (HashNode *n2 = table[i]; n2; ) { HashNode *nx = n2->next; free(n2); n2 = nx; }
    return modeVal;
}

int removeDuplicates(int arr[], int n, int result[]) {  /* O(n) average via hashing */
    HashNode *table[HASH_SIZE] = {0};
    int newN = 0;
    for (int i = 0; i < n; i++) {
        unsigned h = hashFunc(arr[i]);
        HashNode *node = table[h];
        int found = 0;
        while (node) { if (node->key == arr[i]) { found = 1; break; } node = node->next; }
        if (!found) {
            HashNode *nn = malloc(sizeof(HashNode));
            *nn = (HashNode){arr[i], 1, table[h]};
            table[h] = nn;
            result[newN++] = arr[i];
        }
    }
    for (int i = 0; i < HASH_SIZE; i++)
        for (HashNode *n2 = table[i]; n2; ) { HashNode *nx = n2->next; free(n2); n2 = nx; }
    return newN;
}

void reverseArray(int arr[], int n) {                   /* O(n) */
    for (int l = 0, r = n - 1; l < r; l++, r--) { int t = arr[l]; arr[l] = arr[r]; arr[r] = t; }
}

int partitionGE_first(int arr[], int n) {                /* O(n): >= pivot placed first */
    if (n == 0) return -1;
    srand((unsigned) time(NULL));
    int p = rand() % n;
    int t = arr[0]; arr[0] = arr[p]; arr[p] = t;
    int pivot = arr[0], store = 1;
    for (int i = 1; i < n; i++)
        if (arr[i] >= pivot) { t = arr[i]; arr[i] = arr[store]; arr[store] = t; store++; }
    store--;
    t = arr[0]; arr[0] = arr[store]; arr[store] = t;
    return store;
}

int main(void) {
    int arr[] = {12, 45, 3, 45, 67, 2, 12, 89, 34, 45, 1, 67, 23};
    int n = sizeof(arr) / sizeof(arr[0]);
    printArray("Original:", arr, n);

    printf("(i)   Max              : %d\n", findMax(arr, n));
    int f, s; findFirstSecondLargest(arr, n, &f, &s);
    printf("(ii)  1st & 2nd largest : %d, %d\n", f, s);
    printf("(iii) Mean              : %.4f\n", findMean(arr, n));
    printf("(iv)  Median            : %.4f\n", findMedian(arr, n));
    printf("(v)   Std deviation     : %.4f\n", findStdDev(arr, n));
    printf("(vi)  Mode              : %d\n", findMode(arr, n));

    int *dedup = malloc(n * sizeof(int));
    int dn = removeDuplicates(arr, n, dedup);
    printArray("(vii) Dedup ->", dedup, dn);
    free(dedup);

    int rev[13]; memcpy(rev, arr, sizeof(arr));
    reverseArray(rev, n);
    printArray("(viii) Reversed ->", rev, n);

    int part[13]; memcpy(part, arr, sizeof(arr));
    int pi = partitionGE_first(part, n);
    printArray("(ix) Partitioned ->", part, n);
    printf("     pivot=%d at index %d (>=pivot before, <pivot after)\n", part[pi], pi);
    return 0;
}