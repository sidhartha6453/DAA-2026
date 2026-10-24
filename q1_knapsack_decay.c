/* Q1: Fractional Knapsack with Deterioration Rate
   Model: items are consumed one after another at 1 unit of weight per unit time.
   A unit of item i consumed at time t is worth  d_i - lambda_i * t,  d_i = v_i / w_i.
   Weights/capacity are integers; an item may be taken partially (any number of units). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXN 20
#define MAXW 2000
typedef struct { int id, w; double v, d, lam; } Item;

/* value of taking x units of item it, starting at time t (continuous decay inside the block) */
double gain(const Item *it, int x, int t) { return x * it->d - it->lam * (x * (double)t + x * (double)x / 2.0); }

int cmpLam(const void *a, const void *b) {            /* greedy ordering: larger decay rate first */
    const Item *p = a, *q = b;
    if (p->lam != q->lam) return (p->lam < q->lam) ? 1 : -1;
    return p->id - q->id;
}

/* ---- exact optimum by brute force over every permutation and every amount (small n only) ---- */
double bestBrute;
void brute(Item *a, int n, int mask, int t, int W, double val) {
    if (val > bestBrute) bestBrute = val;
    for (int i = 0; i < n; i++) if (!(mask >> i & 1))
        for (int x = 1; x <= a[i].w && t + x <= W; x++) brute(a, n, mask | 1 << i, t + x, W, val + gain(&a[i], x, t));
}

/* ---- naive "pick the best current density every time unit" heuristic, for comparison ---- */
double dynamicGreedy(Item *a, int n, int W) {
    int rem[MAXN]; double val = 0;
    for (int i = 0; i < n; i++) rem[i] = a[i].w;
    for (int t = 0; t < W; t++) {
        int b = -1; double bv = 0;
        for (int i = 0; i < n; i++) if (rem[i] > 0) {
            double c = a[i].d - a[i].lam * (t + 0.5);
            if (c > bv) { bv = c; b = i; }
        }
        if (b < 0) break;
        rem[b]--; val += bv;
    }
    return val;
}

static double dp[MAXN + 1][MAXW + 1];
static int ch[MAXN + 1][MAXW + 1];

int main() {
    int n, W; Item a[MAXN];
    printf("Enter n and capacity W: "); scanf("%d %d", &n, &W);
    if (n < 1 || n > MAXN || W < 1 || W > MAXW) { printf("n must be 1..%d, W 1..%d\n", MAXN, MAXW); return 1; }
    printf("Enter value, weight, decay-rate for each item:\n");
    for (int i = 0; i < n; i++) {
        a[i].id = i + 1; scanf("%lf %d %lf", &a[i].v, &a[i].w, &a[i].lam);
        a[i].d = a[i].v / a[i].w;
    }
    Item o[MAXN]; memcpy(o, a, sizeof(Item) * n);
    qsort(o, n, sizeof(Item), cmpLam);                 /* step 1: exchange-argument order */

    for (int t = 0; t <= W; t++) dp[0][t] = (t == 0) ? 0 : -1e18;     /* step 2: choose the amounts */
    for (int i = 1; i <= n; i++)
        for (int t = 0; t <= W; t++) {
            dp[i][t] = dp[i - 1][t]; ch[i][t] = 0;
            for (int x = 1; x <= o[i - 1].w && x <= t; x++) {
                if (dp[i - 1][t - x] < -1e17) continue;
                double c = dp[i - 1][t - x] + gain(&o[i - 1], x, t - x);
                if (c > dp[i][t]) { dp[i][t] = c; ch[i][t] = x; }
            }
        }
    int bt = 0;
    for (int t = 0; t <= W; t++) if (dp[n][t] > dp[n][bt]) bt = t;
    int take[MAXN + 1] = {0}, t = bt;
    for (int i = n; i >= 1; i--) { take[i] = ch[i][t]; t -= take[i]; }

    printf("\nOptimal consumption schedule (decay-rate descending):\n");
    printf("%-6s %-8s %-8s %-8s %-8s %-10s\n", "Item", "Start t", "Units", "Of w_i", "lambda", "Value");
    int clock = 0;
    for (int i = 1; i <= n; i++) if (take[i]) {
        printf("%-6d %-8d %-8d %-8d %-8.2f %-10.3f\n", o[i - 1].id, clock, take[i], o[i - 1].w, o[i - 1].lam, gain(&o[i - 1], take[i], clock));
        clock += take[i];
    }
    printf("Total weight used = %d / %d\nMaximum total value = %.3f\n", bt, W, dp[n][bt]);
    printf("Naive 'best current density' greedy gives     = %.3f\n", dynamicGreedy(a, n, W));
    if (n <= 6) {
        bestBrute = 0; brute(a, n, 0, 0, W, 0);
        printf("Brute force over all orders/amounts gives     = %.3f  -> %s\n", bestBrute, (bestBrute - dp[n][bt] < 1e-6 && dp[n][bt] - bestBrute < 1e-6) ? "MATCH" : "MISMATCH");
    }
    return 0;
}
