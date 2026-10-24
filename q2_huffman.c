/* Q2: Huffman coding with canonical codebook */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXS 64
typedef struct { long long f; int l, r; int order; } Node;
Node T[2 * MAXS]; int heap[2 * MAXS], hs = 0;
int less(int a, int b) { return T[a].f != T[b].f ? T[a].f < T[b].f : T[a].order < T[b].order; }
void push(int x) { int i = hs++; heap[i] = x; while (i && less(heap[i], heap[(i - 1) / 2])) { int p = (i - 1) / 2, t = heap[i]; heap[i] = heap[p]; heap[p] = t; i = p; } }
int pop() {
    int r = heap[0]; heap[0] = heap[--hs]; int i = 0;
    for (;;) { int l = 2 * i + 1, rr = l + 1, m = i;
        if (l < hs && less(heap[l], heap[m])) m = l;
        if (rr < hs && less(heap[rr], heap[m])) m = rr;
        if (m == i) break; int t = heap[i]; heap[i] = heap[m]; heap[m] = t; i = m; }
    return r;
}
int len[MAXS];
void depth(int u, int d) { if (T[u].l < 0) { len[u] = d ? d : 1; return; } depth(T[u].l, d + 1); depth(T[u].r, d + 1); }
typedef struct { char sym; long long f; int len; unsigned long long code; } Sym;
int cmpSym(const void *a, const void *b) { const Sym *p = a, *q = b; if (p->len != q->len) return p->len - q->len; return (unsigned char)p->sym - (unsigned char)q->sym; }

int main() {
    int n; char c[MAXS]; long long f[MAXS];
    printf("Enter number of symbols (max %d): ", MAXS); scanf("%d", &n);
    if (n < 1 || n > MAXS) { printf("bad n\n"); return 1; }
    printf("Enter symbol and frequency per line:\n");
    for (int i = 0; i < n; i++) scanf(" %c %lld", &c[i], &f[i]);
    int cnt = n;
    for (int i = 0; i < n; i++) { T[i] = (Node){f[i], -1, -1, i}; push(i); }
    while (hs > 1) {                                   /* greedy: merge the two lightest trees */
        int x = pop(), y = pop();
        T[cnt] = (Node){T[x].f + T[y].f, x, y, cnt}; push(cnt++);
    }
    depth(pop(), 0);
    Sym s[MAXS];
    for (int i = 0; i < n; i++) s[i] = (Sym){c[i], f[i], len[i], 0};
    qsort(s, n, sizeof(Sym), cmpSym);                  /* canonical: by (length, symbol) */
    unsigned long long code = 0;
    for (int i = 0; i < n; i++) {
        if (i) code = (code + 1) << (s[i].len - s[i - 1].len);
        s[i].code = code;
    }
    long long total = 0, bits = 0; double kraft = 0;
    printf("\nCanonical Huffman codebook\n%-8s %-10s %-8s %s\n", "Symbol", "Frequency", "Length", "Code");
    for (int i = 0; i < n; i++) {
        char buf[70]; for (int b = 0; b < s[i].len; b++) buf[b] = ((s[i].code >> (s[i].len - 1 - b)) & 1) ? '1' : '0';
        buf[s[i].len] = 0;
        printf("%-8c %-10lld %-8d %s\n", s[i].sym, s[i].f, s[i].len, buf);
        total += s[i].f; bits += s[i].f * s[i].len; kraft += 1.0 / (double)(1ULL << s[i].len);
    }
    printf("\nTotal encoded bits = %lld, expected length = %.4f bits/symbol, Kraft sum = %.4f\n", bits, (double)bits / total, kraft);
    int ok = 1;                                         /* prefix-free check */
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) if (i != j && s[i].len <= s[j].len && (s[j].code >> (s[j].len - s[i].len)) == s[i].code) ok = 0;
    printf("Prefix-free check: %s\n", ok ? "PASS" : "FAIL");
    return 0;
}
