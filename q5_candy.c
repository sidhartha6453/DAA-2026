/* Q5: Candy distribution - two-pass (left/right) greedy, cross-checked with one-pass slope method */
#include <stdio.h>
#include <stdlib.h>
int main() {
    int n; printf("Enter n: "); scanf("%d", &n);
    int *r = malloc(n * sizeof(int)), *c = malloc(n * sizeof(int));
    printf("Enter %d ratings: ", n);
    for (int i = 0; i < n; i++) { scanf("%d", &r[i]); c[i] = 1; }
    for (int i = 1; i < n; i++) if (r[i] > r[i - 1]) c[i] = c[i - 1] + 1;                 /* left -> right */
    for (int i = n - 2; i >= 0; i--) if (r[i] > r[i + 1] && c[i] < c[i + 1] + 1) c[i] = c[i + 1] + 1; /* right -> left */
    long long total = 0; for (int i = 0; i < n; i++) total += c[i];
    /* O(1) extra space: walk up-slopes and down-slopes */
    long long t2 = 1; int up = 0, down = 0, peak = 0;
    for (int i = 1; i < n; i++) {
        if (r[i] > r[i - 1]) { up++; peak = up; down = 0; t2 += 1 + up; }
        else if (r[i] == r[i - 1]) { up = down = peak = 0; t2 += 1; }
        else { up = 0; down++; t2 += 1 + down; if (peak >= down) t2--; }
    }
    printf("Candies: "); for (int i = 0; i < n; i++) printf("%d ", c[i]);
    printf("\nMinimum total candies = %lld\nSlope method (O(1) space) = %lld -> %s\n", total, t2, total == t2 ? "MATCH" : "MISMATCH");
    free(r); free(c); return 0;
}
