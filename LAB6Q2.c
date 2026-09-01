/* 2D square matrix operations - see complexity table in accompanying writeup */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

double **allocMatrix(int n) {
    double **m = malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) m[i] = calloc(n, sizeof(double));
    return m;
}
void freeMatrix(double **m, int n) { for (int i = 0; i < n; i++) free(m[i]); free(m); }
void printMatrix(const char *label, double **m, int n) {
    printf("%s\n", label);
    for (int i = 0; i < n; i++) {
        printf("  [");
        for (int j = 0; j < n; j++) printf("%8.3f%s", m[i][j], j == n - 1 ? "" : ", ");
        printf("]\n");
    }
}
double **copyMatrix(double **src, int n) {
    double **d = allocMatrix(n);
    for (int i = 0; i < n; i++) memcpy(d[i], src[i], n * sizeof(double));
    return d;
}

double **addMatrices(double **A, double **B, int n) {   /* O(n^2) */
    double **C = allocMatrix(n);
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) C[i][j] = A[i][j] + B[i][j];
    return C;
}

double **multiplyMatrices(double **A, double **B, int n) { /* O(n^3) */
    double **C = allocMatrix(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            double s = 0;
            for (int k = 0; k < n; k++) s += A[i][k] * B[k][j];
            C[i][j] = s;
        }
    return C;
}

int isZeroMatrix(double **A, int n) {                    /* O(n^2) */
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) if (fabs(A[i][j]) > 1e-9) return 0;
    return 1;
}

int isSymmetric(double **A, int n) {                     /* O(n^2) */
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) if (fabs(A[i][j] - A[j][i]) > 1e-9) return 0;
    return 1;
}

double determinant(double **Ain, int n) {                /* O(n^3): Gaussian elimination */
    double **A = copyMatrix(Ain, n);
    double det = 1.0;
    for (int col = 0; col < n; col++) {
        int piv = col; double mx = fabs(A[col][col]);
        for (int r = col + 1; r < n; r++) if (fabs(A[r][col]) > mx) { mx = fabs(A[r][col]); piv = r; }
        if (mx < 1e-12) { det = 0; break; }
        if (piv != col) { double *t = A[col]; A[col] = A[piv]; A[piv] = t; det = -det; }
        det *= A[col][col];
        for (int r = col + 1; r < n; r++) {
            double f = A[r][col] / A[col][col];
            for (int c = col; c < n; c++) A[r][c] -= f * A[col][c];
        }
    }
    freeMatrix(A, n);
    return det;
}

void transposeInPlace(double **A, int n) {                /* O(n^2) */
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) { double t = A[i][j]; A[i][j] = A[j][i]; A[j][i] = t; }
}

void dominantEigen(double **A, int n, double *lambdaOut, double *vOut, int maxIter, double tol) { /* O(k*n^2): power iteration */
    double *v = malloc(n * sizeof(double)), *Av = malloc(n * sizeof(double));
    for (int i = 0; i < n; i++) v[i] = 1.0;
    double lambda = 0;
    for (int it = 0; it < maxIter; it++) {
        for (int i = 0; i < n; i++) { double s = 0; for (int j = 0; j < n; j++) s += A[i][j] * v[j]; Av[i] = s; }
        double norm = 0; for (int i = 0; i < n; i++) norm += Av[i] * Av[i]; norm = sqrt(norm);
        for (int i = 0; i < n; i++) v[i] = Av[i] / norm;
        double num = 0;
        for (int i = 0; i < n; i++) { double rs = 0; for (int j = 0; j < n; j++) rs += A[i][j] * v[j]; num += v[i] * rs; }
        if (fabs(num - lambda) < tol) { lambda = num; break; }
        lambda = num;
    }
    *lambdaOut = lambda;
    for (int i = 0; i < n; i++) vOut[i] = v[i];
    free(v); free(Av);
}

int main(void) {
    int n = 3;
    double a_vals[3][3] = {{4,1,2},{1,3,0},{2,0,5}};
    double b_vals[3][3] = {{1,1,1},{1,1,1},{1,1,1}};
    double **A = allocMatrix(n), **B = allocMatrix(n);
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) { A[i][j] = a_vals[i][j]; B[i][j] = b_vals[i][j]; }

    printMatrix("A:", A, n); printMatrix("B:", B, n);

    double **sum = addMatrices(A, B, n); printMatrix("(i) A+B:", sum, n); freeMatrix(sum, n);
    double **prod = multiplyMatrices(A, B, n); printMatrix("(ii) A*B:", prod, n); freeMatrix(prod, n);
    printf("(iii) Zero matrix? %s\n", isZeroMatrix(A, n) ? "YES" : "NO");
    printf("(iv) Symmetric? %s\n", isSymmetric(A, n) ? "YES" : "NO");
    printf("(v) det(A) = %.4f\n", determinant(A, n));

    double **T = copyMatrix(A, n); transposeInPlace(T, n);
    printMatrix("(vi) transpose(A):", T, n); freeMatrix(T, n);

    double lambda, *vec = malloc(n * sizeof(double));
    dominantEigen(A, n, &lambda, vec, 1000, 1e-10);
    printf("(vii) Dominant eigenvalue = %.6f, eigenvector = [%.4f, %.4f, %.4f]\n", lambda, vec[0], vec[1], vec[2]);
    free(vec);

    freeMatrix(A, n); freeMatrix(B, n);
    return 0;
}