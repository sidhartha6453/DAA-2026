#include <stdio.h>
#include <stdlib.h>

int **mat(int n) {
    int **m = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) m[i] = calloc(n, sizeof(int));
    return m;
}

void free_mat(int **m, int n) { for (int i = 0; i < n; i++) free(m[i]); free(m); }

int **op(int **A, int **B, int n, int sign) {
    int **C = mat(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] + sign * B[i][j];
    return C;
}

void split(int **M, int **a, int **b, int **c, int **d, int n) {
    int h = n / 2;
    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++) {
            a[i][j] = M[i][j]; b[i][j] = M[i][j + h];
            c[i][j] = M[i + h][j]; d[i][j] = M[i + h][j + h];
        }
}

int **strassen(int **A, int **B, int n) {
    int **C = mat(n);
    if (n == 1) { C[0][0] = A[0][0] * B[0][0]; return C; }

    int h = n / 2;
    int **a11 = mat(h), **a12 = mat(h), **a21 = mat(h), **a22 = mat(h);
    int **b11 = mat(h), **b12 = mat(h), **b21 = mat(h), **b22 = mat(h);
    split(A, a11, a12, a21, a22, n);
    split(B, b11, b12, b21, b22, n);

    int **t1, **t2;
    t1 = op(a11, a22, h, 1); t2 = op(b11, b22, h, 1);
    int **p1 = strassen(t1, t2, h); free_mat(t1, h); free_mat(t2, h);

    t1 = op(a21, a22, h, 1);
    int **p2 = strassen(t1, b11, h); free_mat(t1, h);

    t2 = op(b12, b22, h, -1);
    int **p3 = strassen(a11, t2, h); free_mat(t2, h);

    t2 = op(b21, b11, h, -1);
    int **p4 = strassen(a22, t2, h); free_mat(t2, h);

    t1 = op(a11, a12, h, 1);
    int **p5 = strassen(t1, b22, h); free_mat(t1, h);

    t1 = op(a21, a11, h, -1); t2 = op(b11, b12, h, 1);
    int **p6 = strassen(t1, t2, h); free_mat(t1, h); free_mat(t2, h);

    t1 = op(a12, a22, h, -1); t2 = op(b21, b22, h, 1);
    int **p7 = strassen(t1, t2, h); free_mat(t1, h); free_mat(t2, h);

    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++) {
            C[i][j]         = p1[i][j] + p4[i][j] - p5[i][j] + p7[i][j];
            C[i][j + h]     = p3[i][j] + p5[i][j];
            C[i + h][j]     = p2[i][j] + p4[i][j];
            C[i + h][j + h] = p1[i][j] - p2[i][j] + p3[i][j] + p6[i][j];
        }

    int **tmp[] = {a11,a12,a21,a22,b11,b12,b21,b22,p1,p2,p3,p4,p5,p6,p7};
    for (int i = 0; i < 15; i++) free_mat(tmp[i], h);
    return C;
}

int **normal(int **A, int **B, int n) {
    int **C = mat(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                C[i][j] += A[i][k] * B[k][j];
    return C;
}

int main() {
    int n;
    printf("Enter size (power of 2): ");
    scanf("%d", &n);

    int **A = mat(n), **B = mat(n);
    printf("Enter matrix A (%d values):\n", n * n);
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) scanf("%d", &A[i][j]);
    printf("Enter matrix B (%d values):\n", n * n);
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) scanf("%d", &B[i][j]);

    int **C1 = strassen(A, B, n), **C2 = normal(A, B, n);

    printf("\nResult:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) printf("%d ", C1[i][j]);
        printf("\n");
    }

    int match = 1;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (C1[i][j] != C2[i][j]) match = 0;
    printf("\nVerification: %s\n", match ? "MATCH" : "MISMATCH");

    free_mat(A, n); free_mat(B, n); free_mat(C1, n); free_mat(C2, n);
    return 0;
}
