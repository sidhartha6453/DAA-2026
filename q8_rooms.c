/* Q8: Minimum number of meeting rooms. Intervals are half-open [s, e): a room freed at time e can host a meeting starting at e. */
#include <stdio.h>
#include <stdlib.h>
typedef struct { int s, e, id, room; } M;
int cmpInt(const void *a, const void *b) { return *(int *)a - *(int *)b; }
int cmpM(const void *a, const void *b) { const M *p = a, *q = b; return p->s != q->s ? p->s - q->s : p->e - q->e; }
typedef struct { int end, room; } R; R h[100005]; int hs = 0;               /* min-heap on end time */
void push(R x) { int i = hs++; h[i] = x; while (i && h[i].end < h[(i - 1) / 2].end) { int p = (i - 1) / 2; R t = h[i]; h[i] = h[p]; h[p] = t; i = p; } }
R pop() { R r = h[0]; h[0] = h[--hs]; int i = 0;
    for (;;) { int l = 2 * i + 1, rr = l + 1, m = i;
        if (l < hs && h[l].end < h[m].end) m = l; if (rr < hs && h[rr].end < h[m].end) m = rr;
        if (m == i) break; R t = h[i]; h[i] = h[m]; h[m] = t; i = m; }
    return r; }
int main() {
    int n; printf("Enter number of meetings: "); scanf("%d", &n);
    M *m = malloc(n * sizeof(M)); int *S = malloc(n * sizeof(int)), *E = malloc(n * sizeof(int));
    printf("Enter start and end of each meeting:\n");
    for (int i = 0; i < n; i++) { scanf("%d %d", &m[i].s, &m[i].e); m[i].id = i + 1; S[i] = m[i].s; E[i] = m[i].e; }
    /* Method 1: sort starts and ends separately, sweep with two pointers */
    qsort(S, n, sizeof(int), cmpInt); qsort(E, n, sizeof(int), cmpInt);
    int cur = 0, mx = 0;
    for (int i = 0, j = 0; i < n; ) { if (S[i] < E[j]) { cur++; i++; if (cur > mx) mx = cur; } else { cur--; j++; } }
    /* Method 2: min-heap of room end times, also gives an actual assignment */
    qsort(m, n, sizeof(M), cmpM); int rooms = 0;
    for (int i = 0; i < n; i++) {
        if (hs && h[0].end <= m[i].s) { R r = pop(); m[i].room = r.room; push((R){m[i].e, r.room}); }
        else { m[i].room = ++rooms; push((R){m[i].e, rooms}); }
    }
    printf("Minimum rooms (two-pointer sweep) = %d\nMinimum rooms (heap assignment)   = %d -> %s\n", mx, rooms, mx == rooms ? "MATCH" : "MISMATCH");
    printf("Assignment (meeting: [s,e) -> room):\n");
    for (int i = 0; i < n; i++) printf("  M%d: [%d,%d) -> Room %d\n", m[i].id, m[i].s, m[i].e, m[i].room);
    return 0;
}
