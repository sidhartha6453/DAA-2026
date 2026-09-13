/* Problem 7: Matrix Chain Multiplication (MCM)
 * O(n^3) DP: m[i][j] = min scalar mults to compute Ai..Aj
 * s[i][j] stores optimal split for parenthesization reconstruction.
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void matrixChainOrder(int *p, int n, long long **m, int **s) {
    for (int i = 1; i <= n; i++) m[i][i] = 0;

    for (int L = 2; L <= n; L++) {
        for (int i = 1; i <= n - L + 1; i++) {
            int j = i + L - 1;
            m[i][j] = LLONG_MAX;
            for (int k = i; k < j; k++) {
                long long cost = m[i][k] + m[k+1][j]
                                 + (long long)p[i-1] * p[k] * p[j];
                if (cost < m[i][j]) {
                    m[i][j] = cost;
                    s[i][j] = k;
                }
            }
        }
    }
}

void printOptimalParens(int **s, int i, int j) {
    if (i == j) {
        printf("A%d", i);
    } else {
        printf("(");
        printOptimalParens(s, i, s[i][j]);
        printOptimalParens(s, s[i][j] + 1, j);
        printf(")");
    }
}

int main(void) {
    int n;
    printf("Enter number of matrices: ");
    scanf("%d", &n);

    int *p = malloc((n + 1) * sizeof(int));
    printf("Enter %d dimensions p[0..%d] such that matrix Ai is p[i-1] x p[i]:\n", n + 1, n);
    for (int i = 0; i <= n; i++) scanf("%d", &p[i]);

    long long **m = malloc((n + 1) * sizeof(long long*));
    int **s        = malloc((n + 1) * sizeof(int*));
    for (int i = 0; i <= n; i++) {
        m[i] = calloc(n + 1, sizeof(long long));
        s[i] = calloc(n + 1, sizeof(int));
    }

    matrixChainOrder(p, n, m, s);

    printf("\nMinimum number of scalar multiplications: %lld\n", m[1][n]);

    printf("Optimal parenthesization: ");
    printOptimalParens(s, 1, n);
    printf("\n");

    printf("\nDP cost table m[i][j]:\n");
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (j < i) printf("%8s", "-");
            else printf("%8lld", m[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i <= n; i++) { free(m[i]); free(s[i]); }
    free(m); free(s); free(p);
    return 0;
}
