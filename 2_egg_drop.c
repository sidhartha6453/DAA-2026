/* Problem 2: Super egg testing experiment
 * E eggs, F floors -- generalized DP solution.
 * Method 1: f(m,e) = max floors distinguishable with m drops, e eggs -> O(M*E)
 * Method 2: classic dp[e][f] with monotonic-k optimization -> O(E*F) amortized
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/* ---------- Method 1: f(m,e) = max floors distinguishable ---------- */
long long solveMaxFloors(int E, int F, int *outMoves, long long ***outTable, int *outM) {
    int m = 0;
    long long **f = NULL;
    int capacity = 1;
    f = malloc(sizeof(long long*) * (capacity+1));
    for (int i = 0; i <= capacity; i++) f[i] = calloc(E+1, sizeof(long long));

    while (1) {
        m++;
        if (m > capacity) {
            int newCap = capacity * 2;
            f = realloc(f, sizeof(long long*) * (newCap+1));
            for (int i = capacity+1; i <= newCap; i++) f[i] = calloc(E+1, sizeof(long long));
            capacity = newCap;
        }
        f[m][0] = 0;
        for (int e = 1; e <= E; e++)
            f[m][e] = f[m-1][e-1] + f[m-1][e] + 1;
        if (f[m][E] >= F) break;
    }

    *outMoves = m;
    *outM = m;
    *outTable = f;
    return f[m][E];
}

/* ---------- Method 2: classic dp[e][f], O(E*F) amortized via monotonic k ---------- */
int solveClassicDP(int E, int F) {
    int **dp = malloc((E+1) * sizeof(int*));
    for (int e = 0; e <= E; e++) dp[e] = malloc((F+1) * sizeof(int));

    for (int f = 0; f <= F; f++) dp[1][f] = f;
    for (int e = 0; e <= E; e++) dp[e][0] = 0;
    if (E >= 1) for (int e=0; e<=E; e++) dp[e][1] = (e>=1)?1:0;

    for (int e = 2; e <= E; e++) {
        int k_opt = 1;
        for (int f = 1; f <= F; f++) {
            while (k_opt <= f) {
                int breakCase  = dp[e-1][k_opt-1];
                int surviveCase = dp[e][f-k_opt];
                int worst = breakCase > surviveCase ? breakCase : surviveCase;
                int worstNext;
                if (k_opt+1 <= f) {
                    int b2 = dp[e-1][k_opt];
                    int s2 = dp[e][f-k_opt-1];
                    worstNext = b2 > s2 ? b2 : s2;
                } else worstNext = INT_MAX;

                if (worstNext < worst) k_opt++;
                else break;
            }
            int breakCase  = dp[e-1][k_opt-1];
            int surviveCase = dp[e][f-k_opt];
            int worst = breakCase > surviveCase ? breakCase : surviveCase;
            dp[e][f] = 1 + worst;
        }
    }

    int result = dp[E][F];
    for (int e = 0; e <= E; e++) free(dp[e]);
    free(dp);
    return result;
}

int main(void) {
    int E, F;
    printf("Enter number of eggs E and floors F: ");
    scanf("%d %d", &E, &F);

    int moves1, mTable;
    long long **table;
    long long covered = solveMaxFloors(E, F, &moves1, &table, &mTable);
    printf("\n[Method 1: f(m,e) max-floors DP]\n");
    printf("Minimum droppings = %d  (covers up to %lld floors, need %d)\n",
           moves1, covered, F);

    int moves2 = solveClassicDP(E, F);
    printf("\n[Method 2: classic dp[e][f] DP]\n");
    printf("Minimum droppings = %d\n", moves2);

    if (moves1 == moves2)
        printf("\nBoth methods agree: answer = %d\n", moves1);
    else
        printf("\nMismatch! %d vs %d (check implementation)\n", moves1, moves2);

    for (int i = 0; i <= mTable; i++) free(table[i]);
    free(table);
    return 0;
}
