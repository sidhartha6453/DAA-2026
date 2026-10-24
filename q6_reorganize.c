/* Q6: Reorganise string so equal characters are >= K apart (max-heap + cool-down queue) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct { int cnt; unsigned char ch; } E;
E h[256]; int hs = 0;
int better(E a, E b) { return a.cnt != b.cnt ? a.cnt > b.cnt : a.ch < b.ch; }
void push(E x) { int i = hs++; h[i] = x; while (i && better(h[i], h[(i - 1) / 2])) { int p = (i - 1) / 2; E t = h[i]; h[i] = h[p]; h[p] = t; i = p; } }
E pop() { E r = h[0]; h[0] = h[--hs]; int i = 0;
    for (;;) { int l = 2 * i + 1, rr = l + 1, m = i;
        if (l < hs && better(h[l], h[m])) m = l; if (rr < hs && better(h[rr], h[m])) m = rr;
        if (m == i) break; E t = h[i]; h[i] = h[m]; h[m] = t; i = m; }
    return r; }

int main() {
    static char S[100005]; int K;
    printf("Enter string S: "); scanf("%100000s", S);
    printf("Enter K: "); scanf("%d", &K);
    int n = strlen(S), freq[256] = {0};
    for (int i = 0; i < n; i++) freq[(unsigned char)S[i]]++;
    if (K <= 1) { printf("Result: %s\n", S); return 0; }        /* no restriction */
    for (int c = 0; c < 256; c++) if (freq[c]) push((E){freq[c], (unsigned char)c});
    char *res = malloc(n + 1); E *q = malloc((n + 1) * sizeof(E)); int qh = 0, qt = 0, len = 0, ok = 1;
    while (len < n) {
        if (hs == 0) { ok = 0; break; }                           /* every remaining char is cooling down */
        E e = pop(); res[len++] = e.ch; e.cnt--;
        q[qt++] = e;                                              /* wait K positions before re-use */
        if (qt - qh >= K) { E f = q[qh++]; if (f.cnt > 0) push(f); }
    }
    if (!ok) { printf("Result: \"\" (impossible)\n"); return 0; }
    res[n] = 0; printf("Result: %s\n", res);
    int last[256]; for (int c = 0; c < 256; c++) last[c] = -1000000; int good = 1;
    for (int i = 0; i < n; i++) { unsigned char c = res[i]; if (i - last[c] < K) good = 0; last[c] = i; }
    printf("Distance check (every repeat >= %d apart): %s\n", K, good ? "PASS" : "FAIL");
    return 0;
}
