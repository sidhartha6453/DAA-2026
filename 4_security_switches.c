/* Problem 4: Security switches
 * The legal-move structure matches the reflected binary Gray code.
 * Minimum moves T(n) = floor(2^(n+1)/3).
 */
#include <stdio.h>
#include <stdlib.h>

long long formulaT(int n) {
    return (1LL << (n+1)) / 3;   /* floor(2^(n+1)/3) */
}

int ctz(long long x) {
    int c = 0;
    while ((x & 1) == 0) { x >>= 1; c++; }
    return c;
}

/* rule-based legality check, used only for independent validation */
int canToggle(int *s, int n, int idx /*1-indexed*/) {
    if (idx == n) return 1;
    if (s[idx+1] != 1) return 0;
    for (int j = idx+2; j <= n; j++)
        if (s[j] != 0) return 0;
    return 1;
}

int main(void) {
    int n;
    printf("Enter number of switches n: ");
    scanf("%d", &n);

    long long T = formulaT(n);
    printf("Formula minimum moves T(%d) = %lld\n\n", n, T);

    int *s = malloc((n+1) * sizeof(int));
    for (int i = 1; i <= n; i++) s[i] = 1;

    long long executed = 0;
    int ok = 1;

    for (long long m = 1; m <= T; m++) {
        long long k = T - m + 1;
        int bitpos = ctz(k);
        int sw = n - bitpos;

        if (!canToggle(s, n, sw)) {
            printf("ILLEGAL MOVE at step %lld: switch %d not togglable!\n", m, sw);
            ok = 0;
            break;
        }
        s[sw] ^= 1;
        executed++;

        /* uncomment for full trace:
        printf("Move %lld: toggle switch %d\n", m, sw); */
    }

    int allOff = 1;
    for (int i = 1; i <= n; i++) if (s[i] != 0) allOff = 0;

    printf("Moves executed: %lld\n", executed);
    printf("All switches off: %s\n", allOff ? "YES" : "NO");
    printf("Validation: %s\n", (ok && allOff && executed==T) ? "PASSED" : "FAILED");

    free(s);
    return 0;
}
