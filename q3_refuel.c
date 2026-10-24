/* Q3: Minimum number of refuelling stops (max-heap greedy) + minimum initial fuel (reverse greedy) */
#include <stdio.h>
#include <stdlib.h>
typedef struct { long long d, f; } St;
int cmpSt(const void *a, const void *b) { const St *p = a, *q = b; return (p->d > q->d) - (p->d < q->d); }
St h[100005]; int hs = 0;                       /* max-heap on fuel */
void push(St x) { int i = hs++; h[i] = x; while (i && h[i].f > h[(i - 1) / 2].f) { int p = (i - 1) / 2; St t = h[i]; h[i] = h[p]; h[p] = t; i = p; } }
St pop() { St r = h[0]; h[0] = h[--hs]; int i = 0;
    for (;;) { int l = 2 * i + 1, rr = l + 1, m = i;
        if (l < hs && h[l].f > h[m].f) m = l; if (rr < hs && h[rr].f > h[m].f) m = rr;
        if (m == i) break; St t = h[i]; h[i] = h[m]; h[m] = t; i = m; }
    return r; }

int main() {
    long long D, F; int n;
    printf("Enter target distance D, initial fuel F, number of stations n: "); scanf("%lld %lld %d", &D, &F, &n);
    St *s = malloc((n + 1) * sizeof(St));
    printf("Enter (distance, fuel) for each station:\n");
    for (int i = 0; i < n; i++) scanf("%lld %lld", &s[i].d, &s[i].f);
    qsort(s, n, sizeof(St), cmpSt);
    long long reach = F; int i = 0, stops = 0, ok = 1; St used[100005];
    while (reach < D) {
        while (i < n && s[i].d <= reach) push(s[i++]);   /* every station already reachable is a candidate */
        if (hs == 0) { ok = 0; break; }
        St b = pop(); reach += b.f; used[stops++] = b;   /* retroactively refuel at the biggest one */
    }
    if (ok) {
        printf("Minimum refuelling stops = %d\nStations used (distance:fuel): ", stops);
        for (int k = 0; k < stops; k++) printf("%lld:%lld ", used[k].d, used[k].f);
        printf("\n");
    } else printf("Target cannot be reached (returns -1)\n");
    /* Reverse greedy: smallest F that reaches D if every station may be used */
    long long need = D;
    for (int k = n - 1; k >= 0; k--) { need -= s[k].f; if (need < s[k].d) need = s[k].d; }
    printf("Minimum initial fuel needed (all stations usable) = %lld\n", need);
    return 0;
}
