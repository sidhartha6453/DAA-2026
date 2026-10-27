/* Q2: Multi-source BFS for radius-constrained Voronoi cell partitioning.
   All hubs start in the queue at distance 0. Ties are resolved by keeping
   the smallest hub index among all shortest-path predecessors. */
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n, m, K, R, i;
    printf("Enter vertices, edges, number of hubs K and radius R: ");
    if (scanf("%d %d %d %d", &n, &m, &K, &R) != 4 || n < 1 || m < 0 ||
        K < 1 || K > n || R < 0) {
        printf("Invalid input.\n");
        return 1;
    }
    int *hub = malloc((K + 1) * sizeof(int));
    printf("Enter the %d hub vertices h1..hK: ", K);
    for (i = 1; i <= K; i++)
        if (scanf("%d", &hub[i]) != 1 || hub[i] < 1 || hub[i] > n) {
            printf("Invalid hub.\n");
            return 1;
        }
    int *head = malloc((n + 1) * sizeof(int));
    int *nxt = malloc((2 * m + 1) * sizeof(int));
    int *to = malloc((2 * m + 1) * sizeof(int));
    int ec = 0;
    for (i = 0; i <= n; i++) head[i] = -1;
    for (i = 0; i < m; i++) {
        int u, v;
        printf("Enter undirected edge %d (u v): ", i + 1);
        if (scanf("%d %d", &u, &v) != 2 || u < 1 || u > n || v < 1 || v > n) {
            printf("Invalid edge.\n");
            return 1;
        }
        to[ec] = v; nxt[ec] = head[u]; head[u] = ec++;
        to[ec] = u; nxt[ec] = head[v]; head[v] = ec++;
    }
    int *dist = malloc((n + 1) * sizeof(int));
    int *owner = malloc((n + 1) * sizeof(int));
    int *q = malloc((n + 1) * sizeof(int));
    int qh = 0, qt = 0;
    for (i = 0; i <= n; i++) { dist[i] = -1; owner[i] = -1; }
    for (i = 1; i <= K; i++) {             /* index order => smaller index wins */
        int h = hub[i];
        if (dist[h] == -1) { dist[h] = 0; owner[h] = i; q[qt++] = h; }
    }
    while (qh < qt) {
        int u = q[qh++], e;
        if (dist[u] == R) continue;        /* do not expand beyond radius R */
        for (e = head[u]; e != -1; e = nxt[e]) {
            int v = to[e];
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1; owner[v] = owner[u]; q[qt++] = v;
            } else if (dist[v] == dist[u] + 1 && owner[u] < owner[v]) {
                owner[v] = owner[u];       /* tie: smaller hub index */
            }
        }
    }
    for (i = 1; i <= n; i++) {
        if (owner[i] == -1) printf("Vertex %d: owner = -1 (outside coverage)\n", i);
        else printf("Vertex %d: owner = %d, distance = %d\n", i, owner[i], dist[i]);
    }
    printf("Time complexity: O(V + E + K); space complexity: O(V + E).\n");
#ifdef _WIN32
    system("pause");
#endif
    return 0;
}
