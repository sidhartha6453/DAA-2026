/* Q3: Maximum-weight connected component whose size is at most K.
   After every new edge, report the best eligible component.
   If a component grows beyond K, it remains in the graph but is not eligible. */
#include <stdio.h>
#define MAXV 100000

int parent[MAXV + 1], rankDSU[MAXV + 1], compSize[MAXV + 1];
long long weight[MAXV + 1], compWeight[MAXV + 1];

int findRoot(int x) {
    if (parent[x] != x) parent[x] = findRoot(parent[x]);
    return parent[x];
}
void unite(int u, int v) {
    int ru = findRoot(u), rv = findRoot(v);
    if (ru == rv) return;
    if (rankDSU[ru] < rankDSU[rv]) {
        int t = ru; ru = rv; rv = t;
    }
    parent[rv] = ru;
    compSize[ru] += compSize[rv];
    compWeight[ru] += compWeight[rv];
    if (rankDSU[ru] == rankDSU[rv]) rankDSU[ru]++;
}
long long getBest(int V, int K) {
    int i;
    long long best = 0;
    for (i = 1; i <= V; i++) {
        if (findRoot(i) == i && compSize[i] <= K && compWeight[i] > best)
            best = compWeight[i];
    }
    return best;
}
int main(void) {
    int V, E, K, i, u, v;
    printf("Enter vertices, edges, and size limit K: ");
    if (scanf("%d %d %d", &V, &E, &K) != 3 ||
        V < 1 || V > MAXV || E < 0 || K < 1 || K > V) {
        printf("Invalid input.\n");
        return 1;
    }
    for (i = 1; i <= V; i++) {
        printf("Enter weight of vertex %d: ", i);
        if (scanf("%lld", &weight[i]) != 1 || weight[i] < 0) {
            printf("Invalid weight.\n"); return 1;
        }
        parent[i] = i;
        rankDSU[i] = 0;
        compSize[i] = 1;
        compWeight[i] = weight[i];
    }
    for (i = 1; i <= E; i++) {
        printf("Enter edge %d (u v): ", i);
        if (scanf("%d %d", &u, &v) != 2 || u < 1 || u > V || v < 1 || v > V) {
            printf("Invalid edge.\n"); return 1;
        }
        unite(u, v);
        printf("Maximum eligible component weight after edge %d: %lld\n", i, getBest(V, K));
    }
    printf("Time complexity: O(E*V alpha(V)) with a full scan after each edge; space complexity: O(V).\n");
    return 0;
}
