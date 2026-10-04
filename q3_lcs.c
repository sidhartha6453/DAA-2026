// Input: strings X and Y.  Time O(m*n), Space O(m*n)
#include <stdio.h>
#include <string.h>
char X[1001], Y[1001], s[1001];
int L[1001][1001];
int main() {
    scanf("%1000s %1000s", X, Y);
    int m = strlen(X), n = strlen(Y);
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            L[i][j] = X[i-1] == Y[j-1] ? L[i-1][j-1] + 1
                    : L[i-1][j] > L[i][j-1] ? L[i-1][j] : L[i][j-1];
    int k = L[m][n];
    for (int i = m, j = n; i && j; )                   // trace back
        if (X[i-1] == Y[j-1]) s[--k] = X[i-1], i--, j--;
        else if (L[i-1][j] >= L[i][j-1]) i--;
        else j--;
    printf("LCS length = %d, LCS = %s\n", L[m][n], s);
}
