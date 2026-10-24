/* Q5: Lexicographically smallest permutation using swaps along graph edges.
   All values in one connected component can be rearranged among its vertices. */
#include <stdio.h>
#define MAXN 100000

int parent[MAXN + 1], rankDSU[MAXN + 1];
int A[MAXN + 1], component[MAXN + 1], values[MAXN + 1], positions[MAXN + 1];

int findRoot(int x) {
    if (parent[x] != x) parent[x] = findRoot(parent[x]);
    return parent[x];
}
void unite(int a, int b) {
    int ra = findRoot(a), rb = findRoot(b);
    if (ra == rb) return;
    if (rankDSU[ra] < rankDSU[rb]) {
        int t = ra; ra = rb; rb = t;
    }
    parent[rb] = ra;
    if (rankDSU[ra] == rankDSU[rb]) rankDSU[ra]++;
}
int main(void) {
    int n, m, i, u, v;
    printf("Enter n and number of allowed-swap edges: ");
    if (scanf("%d %d", &n, &m) != 2 || n < 1 || n > MAXN || m < 0) {
        printf("Invalid input.\n"); return 1;
    }
    for (i = 1; i <= n; i++) {
        parent[i] = i;
        rankDSU[i] = 0;
        printf("Enter A[%d]: ", i);
        if (scanf("%d", &A[i]) != 1) { printf("Invalid permutation value.\n"); return 1; }
    }
    for (i = 1; i <= m; i++) {
        printf("Enter allowed swap edge %d (u v): ", i);
        if (scanf("%d %d", &u, &v) != 2 || u < 1 || u > n || v < 1 || v > n) {
            printf("Invalid edge.\n"); return 1;
        }
        unite(u, v);
    }

    /* Repeatedly select the smallest unused value and place it in the smallest
       still-unfilled position belonging to the same component. This simple
       implementation is O(n^2), but straightforward and portable. */
    for (i = 1; i <= n; i++) component[i] = findRoot(i);
    for (i = 1; i <= n; i++) {
        int j, bestValueIndex = -1, bestPositionIndex = -1;
        for (j = 1; j <= n; j++) {
            if (values[j] == 0 && (bestValueIndex == -1 || A[j] < A[bestValueIndex]))
                bestValueIndex = j;
        }
        /* Use positions[] as a used marker for output positions. */
        for (j = 1; j <= n; j++) {
            if (positions[j] == 0 && component[j] == component[bestValueIndex]) {
                bestPositionIndex = j;
                break;
            }
        }
        if (bestValueIndex == -1 || bestPositionIndex == -1) {
            printf("Input must be a permutation of 1..n.\n");
            return 1;
        }
        values[bestValueIndex] = 1; /* marks the input value as used */
        positions[bestPositionIndex] = A[bestValueIndex];
    }

    printf("Lexicographically smallest permutation: ");
    for (i = 1; i <= n; i++) printf("%d ", positions[i]);
    printf("\nTime complexity: O(n^2 + m alpha(n)); space complexity: O(n).\n");
    return 0;
}
