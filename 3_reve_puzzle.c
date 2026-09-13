/* Problem 3: Reve's Puzzle (4-peg Tower of Hanoi, Frame-Stewart algorithm)
 * FS(n) = min over 1<=k<n of [ 2*FS(k) + (2^(n-k) - 1) ], FS(0)=0
 * For n=8 disks, FS(8) = 33.
 */
#include <stdio.h>
#include <stdlib.h>

#define MAXN 30

long long FS[MAXN+1];
int OptK[MAXN+1];
long long moveCount = 0;
int printMoves = 1;

/* forward declaration */
void hanoi3Labeled(int n, char src, char dst, char spare, int baseOffset);

/* ---------- Step 0: DP to find Frame-Stewart optimal move counts ---------- */
void buildDP(int n) {
    FS[0] = 0;
    for (int i = 1; i <= n; i++) {
        long long best = -1;
        int bestK = 1;
        for (int k = 1; k < i; k++) {
            long long cost = 2*FS[k] + ((1LL << (i-k)) - 1);
            if (best == -1 || cost < best) { best = cost; bestK = k; }
        }
        if (i == 1) { best = 1; bestK = 0; }
        FS[i] = best;
        OptK[i] = bestK;
    }
}

/* ---------- Frame-Stewart recursive 4-peg algorithm ---------- */
void frameStewart(int n, char src, char dst, char spare1, char spare2, int baseOffset) {
    if (n == 0) return;
    if (n == 1) {
        moveCount++;
        if (printMoves) printf("Move disk %d: %c -> %c\n", baseOffset+1, src, dst);
        return;
    }
    int k = OptK[n];
    frameStewart(k, src, spare2, dst, spare1, baseOffset);
    hanoi3Labeled(n-k, src, dst, spare1, baseOffset+k);
    frameStewart(k, spare2, dst, src, spare1, baseOffset);
}

/* labeled 3-peg hanoi, used for the middle chunk */
void hanoi3Labeled(int n, char src, char dst, char spare, int baseOffset) {
    if (n == 0) return;
    hanoi3Labeled(n-1, src, spare, dst, baseOffset);
    moveCount++;
    if (printMoves) printf("Move disk %d: %c -> %c\n", baseOffset+n, src, dst);
    hanoi3Labeled(n-1, spare, dst, src, baseOffset);
}

int main(void) {
    int n;
    printf("Enter number of disks n: ");
    scanf("%d", &n);
    if (n > MAXN) { printf("n too large for this demo.\n"); return 1; }

    buildDP(n);

    printf("\nFrame-Stewart predicted minimum moves for n=%d: %lld (optimal k=%d)\n\n",
           n, FS[n], OptK[n]);

    printf("Move sequence (A=source, D=destination, B,C=spares):\n");
    moveCount = 0;
    frameStewart(n, 'A', 'D', 'B', 'C', 0);

    printf("\nActual moves executed: %lld\n", moveCount);
    if (moveCount == FS[n])
        printf("VALIDATED: matches Frame-Stewart formula.\n");
    else
        printf("MISMATCH! check implementation.\n");

    return 0;
}
