/* Q10: Greedy shortest-common-superstring vs exact optimum (bitmask DP) - measures the approximation ratio */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXS 16
#define LEN 256
char S[MAXS][LEN]; int n;
int overlap(const char *a, const char *b) {          /* longest suffix of a that is a prefix of b (proper) */
    int la = strlen(a), lb = strlen(b), m = la < lb ? la : lb;
    for (int k = m; k > 0; k--) if (memcmp(a + la - k, b, k) == 0) return k;
    return 0;
}
int main() {
    char in[MAXS][LEN]; int m;
    printf("Enter number of strings (max %d): ", MAXS); scanf("%d", &m);
    printf("Enter the strings:\n");
    for (int i = 0; i < m; i++) scanf("%255s", in[i]);
    n = 0;                                            /* drop duplicates and strings contained in another string */
    for (int i = 0; i < m; i++) {
        int drop = 0;
        for (int j = 0; j < m && !drop; j++) if (i != j && strstr(in[j], in[i]) && (strcmp(in[j], in[i]) != 0 || j < i)) drop = 1;
        if (!drop) strcpy(S[n++], in[i]);
    }
    /* ---- greedy: merge the pair with maximum overlap until one string remains ---- */
    char G[MAXS][LEN]; int g = n; for (int i = 0; i < n; i++) strcpy(G[i], S[i]);
    while (g > 1) {
        int bi = 0, bj = 1, bo = -1;
        for (int i = 0; i < g; i++) for (int j = 0; j < g; j++) if (i != j) { int o = overlap(G[i], G[j]); if (o > bo) { bo = o; bi = i; bj = j; } }
        char t[LEN * 2]; strcpy(t, G[bi]); strcat(t, G[bj] + bo);
        if (strlen(t) >= LEN) { printf("merged string too long\n"); return 1; }
        strcpy(G[bi], t); strcpy(G[bj], G[g - 1]); if (bi == g - 1) bi = bj; g--;
    }
    printf("Greedy superstring  (len %zu): %s\n", strlen(G[0]), G[0]);
    /* ---- exact: dp[mask][last] = shortest superstring covering mask and ending with string 'last' ---- */
    static int ov[MAXS][MAXS], dp[1 << MAXS][MAXS], par[1 << MAXS][MAXS];
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) ov[i][j] = i == j ? 0 : overlap(S[i], S[j]);
    int full = (1 << n) - 1;
    for (int mk = 0; mk <= full; mk++) for (int i = 0; i < n; i++) dp[mk][i] = 1 << 28;
    for (int i = 0; i < n; i++) { dp[1 << i][i] = strlen(S[i]); par[1 << i][i] = -1; }
    for (int mk = 1; mk <= full; mk++) for (int i = 0; i < n; i++) if ((mk >> i & 1) && dp[mk][i] < (1 << 28))
        for (int j = 0; j < n; j++) if (!(mk >> j & 1)) {
            int nm = mk | 1 << j, v = dp[mk][i] + (int)strlen(S[j]) - ov[i][j];
            if (v < dp[nm][j]) { dp[nm][j] = v; par[nm][j] = i; }
        }
    int bl = 0; for (int i = 1; i < n; i++) if (dp[full][i] < dp[full][bl]) bl = i;
    int order[MAXS], k = n, mk = full, cur = bl;
    while (cur != -1) { order[--k] = cur; int p = par[mk][cur]; mk ^= 1 << cur; cur = p; }
    char O[LEN * 2]; strcpy(O, S[order[0]]);
    for (int i = 1; i < n; i++) strcat(O, S[order[i]] + ov[order[i - 1]][order[i]]);
    printf("Optimal superstring (len %zu): %s\n", strlen(O), O);
    printf("Ratio greedy/optimal = %.4f\n", (double)strlen(G[0]) / strlen(O));
    return 0;
}
