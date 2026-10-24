/* Q1: Online graph bipartiteness using DSU with parity.
   Vertices are numbered 1 through V. An edge that violates bipartiteness is skipped. */
#include <stdio.h>
#define MAXV 100000

int parent[MAXV + 1], rankDSU[MAXV + 1], parity[MAXV + 1];

int findRoot(int x) {
    if (parent[x] == x) return x;
    {
        int oldParent = parent[x];
        parent[x] = findRoot(parent[x]);
        parity[x] ^= parity[oldParent];
    }
    return parent[x];
}

/* Returns 1 if the edge can be added without breaking bipartiteness; otherwise 0. */
int addEdge(int u, int v) {
    int ru = findRoot(u), rv = findRoot(v);
    int pu = parity[u], pv = parity[v];

    if (ru == rv) {
        /* Endpoints must have opposite colors: parity[u] XOR parity[v] must be 1. */
        return ((pu ^ pv) == 1);
    }

    /* Merge components and enforce opposite colors for u and v. */
    if (rankDSU[ru] < rankDSU[rv]) {
        parent[ru] = rv;
        parity[ru] = pu ^ pv ^ 1;
    } else {
        parent[rv] = ru;
        parity[rv] = pu ^ pv ^ 1;
        if (rankDSU[ru] == rankDSU[rv]) rankDSU[ru]++;
    }
    return 1;
}

int main(void) {
    int V, E, i, u, v;
    printf("Enter number of vertices and incoming edges: ");
    if (scanf("%d %d", &V, &E) != 2 || V < 1 || V > MAXV || E < 0) {
        printf("Invalid input.\n");
        return 1;
    }
    for (i = 1; i <= V; i++) {
        parent[i] = i;
        rankDSU[i] = 0;
        parity[i] = 0;
    }

    for (i = 1; i <= E; i++) {
        printf("Enter edge %d (u v): ", i);
        if (scanf("%d %d", &u, &v) != 2 || u < 1 || u > V || v < 1 || v > V) {
            printf("Invalid edge.\n");
            return 1;
        }
        if (addEdge(u, v)) {
            printf("Edge (%d, %d) accepted. Graph remains bipartite.\n", u, v);
        } else {
            printf("Edge (%d, %d) rejected: it violates bipartiteness.\n", u, v);
        }
    }
    printf("Time complexity: O((V + E) alpha(V)) amortized; space complexity: O(V).\n");
    return 0;
}
