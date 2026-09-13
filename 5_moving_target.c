/* Problem 5: Hitting a moving target
 * Shot sequence: 2,3,...,n-1, n-1,...,3,2  (n>=3);  1,1 for n=2.
 * Validated via survivor-set (frontier) simulation.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int buildShotSequence(int n, int *seq) {
    int t = 0;
    if (n == 2) {
        seq[t++] = 1;
        seq[t++] = 1;
        return t;
    }
    for (int i = 2; i <= n - 1; i++) seq[t++] = i;   /* sweep right */
    for (int i = n - 1; i >= 2; i--) seq[t++] = i;   /* sweep left  */
    return t;
}

/* Validate: track survivor set S (positions target could occupy,
   consistent with all trajectories not yet caught). */
int validate(int n, int *seq, int T, int verbose) {
    char *S = calloc(n + 1, sizeof(char));
    char *Snext = calloc(n + 1, sizeof(char));
    for (int i = 1; i <= n; i++) S[i] = 1;

    for (int t = 0; t < T; t++) {
        int shot = seq[t];
        S[shot] = 0;

        int anyAlive = 0;
        for (int i = 1; i <= n; i++) if (S[i]) anyAlive = 1;
        if (verbose) {
            printf("t=%2d  shoot %2d   survivors: ", t + 1, shot);
            for (int i = 1; i <= n; i++) if (S[i]) printf("%d ", i);
            if (!anyAlive) printf("(none)");
            printf("\n");
        }
        if (!anyAlive) { free(S); free(Snext); return 1; }

        memset(Snext, 0, (n + 1) * sizeof(char));
        for (int i = 1; i <= n; i++) {
            if (!S[i]) continue;
            if (i - 1 >= 1) Snext[i - 1] = 1;
            if (i + 1 <= n) Snext[i + 1] = 1;
        }
        memcpy(S, Snext, (n + 1) * sizeof(char));
    }

    int anyAlive = 0;
    for (int i = 1; i <= n; i++) if (S[i]) anyAlive = 1;
    free(S); free(Snext);
    return !anyAlive;
}

int main(void) {
    int n;
    printf("Enter number of hiding spots n (>1): ");
    scanf("%d", &n);
    if (n <= 1) { printf("n must be > 1\n"); return 1; }

    int *seq = malloc(2 * n * sizeof(int));
    int T = buildShotSequence(n, seq);

    printf("\nShot sequence (%d shots): ", T);
    for (int i = 0; i < T; i++) printf("%d ", seq[i]);
    printf("\n\n");

    int ok = validate(n, seq, T, 1);

    printf("\nResult: %s\n", ok ? "GUARANTEED CATCH (validated)" : "FAILED - counterexample survives");

    free(seq);
    return 0;
}
