/* Convolution via divide-and-conquer FFT - O(n log n). See writeup for derivation. */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.14159265358979323846

typedef struct { double re, im; } complex_t;
complex_t c_add(complex_t a, complex_t b) { return (complex_t){a.re+b.re, a.im+b.im}; }
complex_t c_sub(complex_t a, complex_t b) { return (complex_t){a.re-b.re, a.im-b.im}; }
complex_t c_mul(complex_t a, complex_t b) { return (complex_t){a.re*b.re-a.im*b.im, a.re*b.im+a.im*b.re}; }

void fft(complex_t *a, int n, int invert) {              /* T(n) = 2T(n/2) + O(n) = O(n log n) */
    if (n == 1) return;
    complex_t *even = malloc(n/2 * sizeof(complex_t)), *odd = malloc(n/2 * sizeof(complex_t));
    for (int i = 0; i < n/2; i++) { even[i] = a[2*i]; odd[i] = a[2*i+1]; }
    fft(even, n/2, invert); fft(odd, n/2, invert);
    double sign = invert ? 1.0 : -1.0;
    for (int k = 0; k < n/2; k++) {
        double ang = sign * 2.0 * PI * k / n;
        complex_t w = {cos(ang), sin(ang)}, t = c_mul(w, odd[k]);
        a[k] = c_add(even[k], t);
        a[k+n/2] = c_sub(even[k], t);
    }
    free(even); free(odd);
}

int nextPow2(int x) { int p = 1; while (p < x) p <<= 1; return p; }

double *convolveFFT(double *A, int m, double *B, int n, int *outLen) { /* O(n log n) since n>=m */
    int resLen = m + n - 1, N = nextPow2(resLen);
    complex_t *fa = calloc(N, sizeof(complex_t)), *fb = calloc(N, sizeof(complex_t));
    for (int i = 0; i < m; i++) fa[i].re = A[i];
    for (int i = 0; i < n; i++) fb[i].re = B[i];
    fft(fa, N, 0); fft(fb, N, 0);
    for (int i = 0; i < N; i++) fa[i] = c_mul(fa[i], fb[i]);
    fft(fa, N, 1);
    double *C = malloc(resLen * sizeof(double));
    for (int i = 0; i < resLen; i++) C[i] = fa[i].re / N;
    free(fa); free(fb);
    *outLen = resLen;
    return C;
}

double *convolveNaive(double *A, int m, double *B, int n, int *outLen) { /* O(m*n), for validation */
    int resLen = m + n - 1;
    double *C = calloc(resLen, sizeof(double));
    for (int k = 0; k < resLen; k++) {
        double s = 0;
        for (int j = 0; j < m; j++) { int bi = k - j; if (bi >= 0 && bi < n) s += A[j] * B[bi]; }
        C[k] = s;
    }
    *outLen = resLen;
    return C;
}

void printVector(const char *label, double *v, int len) {
    printf("%s [", label);
    for (int i = 0; i < len; i++) printf("%.4f%s", v[i], i == len - 1 ? "" : ", ");
    printf("]\n");
}

int main(void) {
    double A[] = {1, 2, 3, 4};
    double B[] = {1, 0, -1, 2, 1, 3, 0, 2};
    int m = sizeof(A)/sizeof(A[0]), n = sizeof(B)/sizeof(B[0]);
    printf("m=%d, n=%d (n>=m)\n", m, n);
    printVector("A =", A, m);
    printVector("B =", B, n);

    int lenN, lenF;
    double *cN = convolveNaive(A, m, B, n, &lenN);
    double *cF = convolveFFT(A, m, B, n, &lenF);
    printVector("C (naive)  =", cN, lenN);
    printVector("C (FFT)    =", cF, lenF);

    double maxDiff = 0;
    for (int i = 0; i < lenN; i++) { double d = fabs(cN[i]-cF[i]); if (d > maxDiff) maxDiff = d; }
    printf("Max diff = %.10f -> %s\n", maxDiff, maxDiff < 1e-6 ? "PASSED" : "FAILED");

    free(cN); free(cF);
    return 0;
}