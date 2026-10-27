/* Q1: Shortest path on a parity-constrained graph using 0-1 BFS.
   Weights are 0/1, so W(P) = k1(P). Cost = k1 (k1 even) or k1 + C (k1 odd).
   State graph (v, p): p = parity of the number of weight-1 edges used so far. */
#include <stdio.h>
#include <stdlib.h>
#define INF 0x3f3f3f3f3f3f3f3fLL
typedef long long ll;

int main(void) {
    int n, m, s, i;
    ll C;
    printf("Enter vertices, edges, source S and penalty C: ");
    if (scanf("%d %d %d %lld", &n, &m, &s, &C) != 4 || n < 1 || m < 0 ||
        s < 1 || s > n || C < 0) {
        printf("Invalid input.\n");
        return 1;
    }
    int *head = malloc((n + 1) * sizeof(int));
    int *nxt = malloc((m + 1) * sizeof(int));
    int *to = malloc((m + 1) * sizeof(int));
    int *wt = malloc((m + 1) * sizeof(int));
    for (i = 0; i <= n; i++) head[i] = -1;
    for (i = 0; i < m; i++) {
        int u, v, w;
        printf("Enter directed edge %d (u v w, w = 0 or 1): ", i + 1);
        if (scanf("%d %d %d", &u, &v, &w) != 3 || u < 1 || u > n ||
            v < 1 || v > n || (w != 0 && w != 1)) {
            printf("Invalid edge.\n");
            return 1;
        }
        to[i] = v; wt[i] = w; nxt[i] = head[u]; head[u] = i;
    }
    int states = 2 * (n + 1);
    ll *dist = malloc(states * sizeof(ll));
    char *done = calloc(states, 1);
    for (i = 0; i < states; i++) dist[i] = INF;
    /* Deque in an array: front pushes go left of mid, back pushes go right.
       Every push is a successful relaxation, so at most 2m + 1 of each. */
    long cap = 2 * (2L * m + 2) + 4;
    int *dq = malloc(cap * sizeof(int));
    long hd = cap / 2, tl = cap / 2;
    dist[2 * s] = 0;
    dq[tl++] = 2 * s;
    while (hd < tl) {
        int st = dq[hd++];
        if (done[st]) continue;
        done[st] = 1;
        int u = st >> 1, p = st & 1, e;
        for (e = head[u]; e != -1; e = nxt[e]) {
            int w = wt[e], ns = 2 * to[e] + (p ^ w);
            ll nd = dist[st] + w;
            if (nd < dist[ns]) {
                dist[ns] = nd;
                if (w == 0) dq[--hd] = ns; else dq[tl++] = ns;
            }
        }
    }
    for (i = 1; i <= n; i++) {
        ll best = dist[2 * i];
        if (dist[2 * i + 1] < INF && dist[2 * i + 1] + C < best)
            best = dist[2 * i + 1] + C;
        if (best >= INF) printf("Vertex %d: unreachable\n", i);
        else printf("Vertex %d: minimum cost = %lld\n", i, best);
    }
    printf("Time complexity: O(V + E); space complexity: O(V + E).\n");
#ifdef _WIN32
    system("pause");
#endif
    return 0;
}
