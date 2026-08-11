#include <stdio.h>
#include <stdlib.h>
#include <math.h>

long weigh_count = 0;

/* Simulates one balance-scale weighing of two equal-sized groups.
   Returns -1 if left pan lighter, 0 if balanced, 1 if right pan lighter. */
int weigh(int *w, int loL, int hiL, int loR, int hiR) {
    weigh_count++;
    long sumL = 0, sumR = 0;
    for (int i = loL; i <= hiL; i++) sumL += w[i];
    for (int i = loR; i <= hiR; i++) sumR += w[i];
    if (sumL < sumR) return -1;
    if (sumL > sumR) return 1;
    return 0;
}

/* Returns index of the defective (lighter) coin in w[lo..hi], or -1 if none. */
int findDefective(int *w, int lo, int hi) {
    int n = hi - lo + 1;
    if (n == 1) return -2; /* ambiguous: single coin, no reference to compare against */

    int k = n / 2;
    int loL = lo, hiL = lo + k - 1;
    int loR = lo + k, hiR = lo + 2 * k - 1;
    int spare = (2 * k < n) ? hi : -1;

    int cmp = weigh(w, loL, hiL, loR, hiR);

    if (cmp == 0) {
        if (spare != -1) {
            int cmp2 = weigh(w, spare, spare, loL, loL);
            return (cmp2 < 0) ? spare : -1;
        }
        return -1;
    }
    if (cmp < 0) return (k == 1) ? loL : findDefective(w, loL, hiL);
    return (k == 1) ? loR : findDefective(w, loR, hiR);
}

int main() {
    int n;
    printf("Enter number of coins: ");
    scanf("%d", &n);

    int *w = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) w[i] = 100;

    int defIdx;
    printf("Inject a defective coin? (index, or -1 for none): ");
    scanf("%d", &defIdx);
    if (defIdx >= 0 && defIdx < n) w[defIdx] = 99;

    weigh_count = 0;
    int found = findDefective(w, 0, n - 1);

    if (found == -1) printf("\nResult: no defective coin (all perfect)\n");
    else printf("\nResult: defective coin at index %d\n", found);
    printf("Weighings used = %ld, log2(n)+c reference = %.2f\n\n", weigh_count, log2(n));
    free(w);

    srand(1);
    printf("%-10s%-15s%-15s%-15s\n", "n", "Case", "Weighings", "log2(n)");
    for (int size = 2; size <= 100000; size *= 2) {
        for (int mode = 0; mode <= 1; mode++) {
            int *a = malloc(size * sizeof(int));
            for (int i = 0; i < size; i++) a[i] = 100;
            if (mode == 0) a[rand() % size] = 99;

            weigh_count = 0;
            findDefective(a, 0, size - 1);
            printf("%-10d%-15s%-15ld%-15.2f\n", size,
                   mode == 0 ? "defective" : "none", weigh_count, log2(size));
            free(a);
        }
    }
    return 0;
}
