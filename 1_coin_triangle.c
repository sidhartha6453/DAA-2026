/* Problem 1: Invert the coin-triangle
 * Minimum moves = floor(n(n+1)/6) = floor(N/3)
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct { int x, y; } Point;

int cmpPoint(const void *a, const void *b) {
    Point *p = (Point*)a, *q = (Point*)b;
    if (p->y != q->y) return p->y - q->y;
    return p->x - q->x;
}

int genUp(int n, Point *pts) {
    int idx = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j <= i; j++) { pts[idx].x = 2*j - i; pts[idx].y = i; idx++; }
    return idx;
}

int genDown(int n, int m, int DX, Point *pts) {
    int idx = 0;
    for (int i = 0; i < n; i++) {
        int rowlen = n - i;
        for (int k = 0; k < rowlen; k++) {
            pts[idx].x = 2*k - (rowlen - 1) + DX;
            pts[idx].y = i + m;
            idx++;
        }
    }
    return idx;
}

int countOverlap(Point *A, int na, Point *B, int nb) {
    qsort(A, na, sizeof(Point), cmpPoint);
    qsort(B, nb, sizeof(Point), cmpPoint);
    int i=0,j=0,cnt=0;
    while (i<na && j<nb) {
        if (A[i].y==B[j].y && A[i].x==B[j].x) { cnt++; i++; j++; }
        else if (A[i].y<B[j].y || (A[i].y==B[j].y && A[i].x<B[j].x)) i++;
        else j++;
    }
    return cnt;
}

int main(void) {
    int n;
    printf("Enter n (coins per side): ");
    scanf("%d", &n);
    int N = n*(n+1)/2;

    Point *up = malloc(N*sizeof(Point));
    Point *down = malloc(N*sizeof(Point));
    Point *tmp = malloc(N*sizeof(Point));
    genUp(n, up);

    int bestOverlap = -1, bestM = 0, bestDX = 0;
    for (int m = -(n-1); m <= n-1; m++)
        for (int DX = 0; DX <= 1; DX++) {
            int nd = genDown(n, m, DX, down);
            for (int t=0;t<N;t++) tmp[t]=up[t];
            int ov = countOverlap(tmp, N, down, nd);
            if (ov > bestOverlap) { bestOverlap = ov; bestM = m; bestDX = DX; }
        }

    int moves = N - bestOverlap;
    int formula = (n*(n+1))/6; /* floor(N/3) */

    printf("N=%d  best shift m=%d DX=%d\n", N, bestM, bestDX);
    printf("Simulated minimum moves = %d\n", moves);
    printf("Formula floor(n(n+1)/6) = %d\n", formula);

    /* explicit move list */
    genDown(n, bestM, bestDX, down);
    for (int t=0;t<N;t++) tmp[t]=up[t];
    qsort(tmp, N, sizeof(Point), cmpPoint);
    qsort(down, N, sizeof(Point), cmpPoint);

    Point *src = malloc(N*sizeof(Point)), *dst = malloc(N*sizeof(Point));
    int ns=0, nd2=0, i=0, j=0;
    while (i<N && j<N) {
        if (tmp[i].y==down[j].y && tmp[i].x==down[j].x) { i++; j++; }
        else if (tmp[i].y<down[j].y || (tmp[i].y==down[j].y && tmp[i].x<down[j].x)) src[ns++]=tmp[i++];
        else dst[nd2++]=down[j++];
    }
    while (i<N) src[ns++]=tmp[i++];
    while (j<N) dst[nd2++]=down[j++];

    printf("\nMove list (%d moves):\n", ns);
    for (int k=0;k<ns;k++)
        printf("  (%d,%d) -> (%d,%d)\n", src[k].x, src[k].y, dst[k].x, dst[k].y);

    free(up); free(down); free(tmp); free(src); free(dst);
    return 0;
}
