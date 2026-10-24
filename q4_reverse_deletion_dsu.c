/* Q4: Maximum component weight after each sequential vertex deletion.
   Process deletions backward: activate vertices in reverse order and union active neighbors. */
#include <stdio.h>
#define MAXV 100000
#define MAXE 200000

int parent[MAXV + 1], sizeDSU[MAXV + 1], active[MAXV + 1];
long long weight[MAXV + 1], compWeight[MAXV + 1], answer[MAXV + 1];
int eu[MAXE + 1], ev[MAXE + 1], deletion[MAXV + 1];

int findRoot(int x) {
    if (parent[x] != x) parent[x] = findRoot(parent[x]);
    return parent[x];
}
void unite(int a, int b, long long *best) {
    int ra = findRoot(a), rb = findRoot(b);
    if (ra == rb) return;
    if (sizeDSU[ra] < sizeDSU[rb]) {
        int t = ra; ra = rb; rb = t;
    }
    parent[rb] = ra;
    sizeDSU[ra] += sizeDSU[rb];
    compWeight[ra] += compWeight[rb];
    if (compWeight[ra] > *best) *best = compWeight[ra];
}
int main(void) {
    int V, E, i;
    long long best = 0;
    printf("Enter number of vertices and edges: ");
    if (scanf("%d %d", &V, &E) != 2 || V < 1 || V > MAXV || E < 0 || E > MAXE) {
        printf("Invalid input.\n"); return 1;
    }
    for (i = 1; i <= V; i++) {
        printf("Enter weight of vertex %d: ", i);
        if (scanf("%lld", &weight[i]) != 1 || weight[i] < 0) {
            printf("Invalid weight.\n"); return 1;
        }
    }
    for (i = 1; i <= E; i++) {
        printf("Enter edge %d (u v): ", i);
        if (scanf("%d %d", &eu[i], &ev[i]) != 2 ||
            eu[i] < 1 || eu[i] > V || ev[i] < 1 || ev[i] > V) {
            printf("Invalid edge.\n"); return 1;
        }
    }
    printf("Enter deletion order (%d vertex numbers): ", V);
    for (i = 1; i <= V; i++) {
        if (scanf("%d", &deletion[i]) != 1 || deletion[i] < 1 || deletion[i] > V) {
            printf("Invalid deletion order.\n"); return 1;
        }
        {
            int j;
            for (j = 1; j < i; j++) if (deletion[j] == deletion[i]) {
                printf("Each vertex must appear exactly once.\n"); return 1;
            }
        }
    }

    for (i = 1; i <= V; i++) {
        parent[i] = i;
        sizeDSU[i] = 1;
        active[i] = 0;
        compWeight[i] = weight[i];
    }

    /* Before adding back deletion[i], active vertices represent the graph after
       deletions deletion[1] through deletion[i]. */
    for (i = V; i >= 1; i--) {
        int j, x = deletion[i];
        answer[i] = best;
        active[x] = 1;
        parent[x] = x;
        sizeDSU[x] = 1;
        compWeight[x] = weight[x];
        if (weight[x] > best) best = weight[x];
        for (j = 1; j <= E; j++) {
            int y = 0;
            if (eu[j] == x) y = ev[j];
            else if (ev[j] == x) y = eu[j];
            if (y != 0 && active[y]) unite(x, y, &best);
        }
    }
    printf("Maximum component weight after each deletion:\n");
    for (i = 1; i <= V; i++)
        printf("After deleting vertex %d: %lld\n", deletion[i], answer[i]);
    printf("Time complexity: O(V*E + (V+E) alpha(V)) in this straightforward implementation; space complexity: O(V+E).\n");
    return 0;
}
