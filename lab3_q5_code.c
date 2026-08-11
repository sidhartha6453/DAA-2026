#include <stdio.h>
#include <stdlib.h>

long mul_count = 0, add_count = 0;

/* Any array v of length n = 2^k, expanded as M[i][j] = v[i^j],
   automatically satisfies the recursive block pattern M = [[M1,M2],[M2,M1]]. */
void expandFull(int *v, int **M, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            M[i][j] = v[i ^ j];
}

void extractCompact(int **M, int *v, int n) {
    for (int j = 0; j < n; j++) v[j] = M[0][j];
}

/* C = A * B where A, B are compact vectors of special matrices.
   C1 = A1*B1 + A2*B2 ,  C2 = A1*B2 + A2*B1  (recursion on halves) */
void specialMultiply(int *A, int *B, int *C, int n) {
    if (n == 1) { C[0] = A[0] * B[0]; mul_count++; return; }

    int k = n / 2;
    int *A1 = A, *A2 = A + k, *B1 = B, *B2 = B + k;
    int *P1 = malloc(k * sizeof(int)), *P2 = malloc(k * sizeof(int));
    int *P3 = malloc(k * sizeof(int)), *P4 = malloc(k * sizeof(int));

    specialMultiply(A1, B1, P1, k);
    specialMultiply(A2, B2, P2, k);
    specialMultiply(A1, B2, P3, k);
    specialMultiply(A2, B1, P4, k);

    for (int i = 0; i < k; i++) { C[i] = P1[i] + P2[i]; add_count++; }
    for (int i = 0; i < k; i++) { C[k + i] = P3[i] + P4[i]; add_count++; }

    free(P1); free(P2); free(P3); free(P4);
}

int **alloc(int n) {
    int **m = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) m[i] = malloc(n * sizeof(int));
    return m;
}

void freeMat(int **m, int n) { for (int i = 0; i < n; i++) free(m[i]); free(m); }

void naiveMultiply(int **A, int **B, int **C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            C[i][j] = 0;
            for (int p = 0; p < n; p++) C[i][j] += A[i][p] * B[p][j];
        }
}

int main() {
    int n;
    printf("Enter n (power of 2): ");
    scanf("%d", &n);

    srand(1);
    int *a = malloc(n * sizeof(int)), *b = malloc(n * sizeof(int)), *c = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) { a[i] = rand() % 10; b[i] = rand() % 10; }

    int **A = alloc(n), **B = alloc(n), **Cs = alloc(n), **Cn = alloc(n);
    expandFull(a, A, n);
    expandFull(b, B, n);

    mul_count = add_count = 0;
    specialMultiply(a, b, c, n);
    expandFull(c, Cs, n);
    naiveMultiply(A, B, Cn, n);

    int match = 1;
    for (int i = 0; i < n && match; i++)
        for (int j = 0; j < n && match; j++)
            if (Cs[i][j] != Cn[i][j]) match = 0;

    printf("Special D&C result %s naive result\n", match ? "MATCHES" : "DIFFERS FROM");
    printf("Multiplications = %ld, Additions = %ld, Total = %ld, n^2 = %d\n\n",
           mul_count, add_count, mul_count + add_count, n * n);

    if (n <= 8) {
        printf("Result matrix:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) printf("%d ", Cs[i][j]);
            printf("\n");
        }
        printf("\n");
    }

    printf("%-10s%-15s%-10s%-15s\n", "n", "SpecialOps", "n^2", "Naive(n^3)");
    for (int size = 2; size <= 2048; size *= 2) {
        int *va = malloc(size * sizeof(int)), *vb = malloc(size * sizeof(int)), *vc = malloc(size * sizeof(int));
        for (int i = 0; i < size; i++) { va[i] = rand() % 10; vb[i] = rand() % 10; }
        mul_count = add_count = 0;
        specialMultiply(va, vb, vc, size);
        printf("%-10d%-15ld%-10d%-15lld\n", size, mul_count + add_count, size * size, (long long)size * size * size);
        free(va); free(vb); free(vc);
    }

    free(a); free(b); free(c);
    freeMat(A, n); freeMat(B, n); freeMat(Cs, n); freeMat(Cn, n);
    return 0;
}
