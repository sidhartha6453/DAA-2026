/* Q2: Number of Islands II using DSU and coordinate flattening.
   Grid starts as water. Each added cell connects to its four land neighbors. */
#include <stdio.h>
#define MAXR 1000
#define MAXC 1000
#define MAXCELLS 1000000

int parent[MAXCELLS], sizeDSU[MAXCELLS], land[MAXCELLS];

int findRoot(int x) {
    if (parent[x] != x) parent[x] = findRoot(parent[x]);
    return parent[x];
}
int unite(int a, int b) {
    int ra = findRoot(a), rb = findRoot(b);
    if (ra == rb) return 0;
    if (sizeDSU[ra] < sizeDSU[rb]) {
        int t = ra; ra = rb; rb = t;
    }
    parent[rb] = ra;
    sizeDSU[ra] += sizeDSU[rb];
    return 1;
}
int main(void) {
    int R, C, K, i, r, c, islands = 0;
    int dr[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};

    printf("Enter rows, columns, and number of land additions: ");
    if (scanf("%d %d %d", &R, &C, &K) != 3 ||
        R < 1 || R > MAXR || C < 1 || C > MAXC ||
        (long long)R * C > MAXCELLS || K < 0) {
        printf("Invalid grid dimensions or operation count.\n");
        return 1;
    }
    for (i = 0; i < R * C; i++) {
        parent[i] = i;
        sizeDSU[i] = 1;
        land[i] = 0;
    }

    for (i = 0; i < K; i++) {
        int id, d, nr, nc;
        printf("Enter land cell %d (row column, 1-based): ", i + 1);
        if (scanf("%d %d", &r, &c) != 2 || r < 1 || r > R || c < 1 || c > C) {
            printf("Invalid cell position.\n");
            return 1;
        }
        r--; c--;
        id = r * C + c;
        if (!land[id]) {
            land[id] = 1;
            islands++;
            for (d = 0; d < 4; d++) {
                nr = r + dr[d];
                nc = c + dc[d];
                if (nr >= 0 && nr < R && nc >= 0 && nc < C) {
                    int nid = nr * C + nc;
                    if (land[nid] && unite(id, nid)) islands--;
                }
            }
        }
        printf("Number of islands after operation %d: %d\n", i + 1, islands);
    }
    printf("Time complexity: O((R*C + K) alpha(R*C)); space complexity: O(R*C).\n");
    return 0;
}
