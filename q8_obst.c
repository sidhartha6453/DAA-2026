// Input: n, p1..pn, q0..qn.  Time O(n^3), Space O(n^2)
#include <stdio.h>
#define N 102
double p[N], q[N], e[N][N], w[N][N];
int root[N][N];
void show(int i, int j, int par, const char *side) {
    if (i > j) { printf("  d%d is %s child of k%d\n", j, side, par); return; }
    int r = root[i][j];
    if (par) printf("  k%d is %s child of k%d\n", r, side, par);
    else     printf("  k%d is the root\n", r);
    show(i, r-1, r, "left"), show(r+1, j, r, "right");
}
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%lf", &p[i]);
    for (int i = 0; i <= n; i++) scanf("%lf", &q[i]);
    for (int i = 1; i <= n + 1; i++) e[i][i-1] = w[i][i-1] = q[i-1];
    for (int len = 1; len <= n; len++)
        for (int i = 1, j; (j = i + len - 1) <= n; i++) {
            e[i][j] = 1e18;
            w[i][j] = w[i][j-1] + p[j] + q[j];
            for (int r = i; r <= j; r++) {
                double t = e[i][r-1] + e[r+1][j] + w[i][j];
                if (t < e[i][j]) e[i][j] = t, root[i][j] = r;
            }
        }
    printf("Min expected cost = %.4f\n", e[1][n]);
    show(1, n, 0, "");
}
