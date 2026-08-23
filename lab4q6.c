#include <stdio.h>
#include <stdlib.h>

typedef struct { double l, r; } Interval;
typedef struct { double val; int type; } Event; /* type: 0 = start(+1), 1 = end(-1) */

int cmp(const void *a, const void *b) {
    Event *e1 = (Event *)a, *e2 = (Event *)b;
    double d = e1->val - e2->val;
    if (d != 0) return (d > 0) - (d < 0);
    return e1->type - e2->type; /* tie: process starts before ends (closed intervals) */
}

/* O(n log n): sort 2n endpoint events, sweep tracking running coverage count */
int maxCoveragePoint(Interval S[], int n, double *bestPoint) {
    Event *ev = malloc(2 * n * sizeof(Event));

    for (int i = 0; i < n; i++) {
        ev[2 * i] = (Event){S[i].l, 0};
        ev[2 * i + 1] = (Event){S[i].r, 1};
    }

    qsort(ev, 2 * n, sizeof(Event), cmp);

    int count = 0, best = 0;
    *bestPoint = ev[0].val;

    for (int i = 0; i < 2 * n; i++) {
        if (ev[i].type == 0) {
            count++;

            if (count > best) {
                best = count;
                *bestPoint = ev[i].val;
            }
        }
        else {
            count--;
        }
    }

    free(ev);
    return best;
}

int coverageAt(Interval S[], int n, double p) {
    int c = 0;

    for (int i = 0; i < n; i++)
        if (S[i].l <= p && p <= S[i].r)
            c++;

    return c;
}

/* O(n^2) brute force: max coverage is always attained at some endpoint, so it
   suffices to test all 2n candidate points. Used only to validate correctness. */
int bruteForce(Interval S[], int n, double *bestPoint) {
    int best = 0;

    for (int i = 0; i < n; i++) {
        int c1 = coverageAt(S, n, S[i].l);

        if (c1 > best) {
            best = c1;
            *bestPoint = S[i].l;
        }

        int c2 = coverageAt(S, n, S[i].r);

        if (c2 > best) {
            best = c2;
            *bestPoint = S[i].r;
        }
    }

    return best;
}

int main() {
    int n;

    printf("Enter number of intervals: ");
    scanf("%d", &n);

    Interval *S = malloc(n * sizeof(Interval));

    printf("Enter %d (l r) pairs:\n", n);

    for (int i = 0; i < n; i++)
        scanf("%lf %lf", &S[i].l, &S[i].r);

    double p1, p2;

    int m1 = maxCoveragePoint(S, n, &p1);
    int m2 = bruteForce(S, n, &p2);

    printf("\nMax coverage = %d, at point p = %g\n", m1, p1);

    printf("Brute-force check: max = %d at p = %g -> %s\n\n",
           m2, p2, m1 == m2 ? "MATCH" : "MISMATCH");

    free(S);

    srand(1);

    printf("Randomised validation:\n");

    int mismatches = 0;

    for (int size = 5; size <= 5000; size *= 4) {

        for (int trial = 0; trial < 10; trial++) {

            Interval *a = malloc(size * sizeof(Interval));

            for (int i = 0; i < size; i++) {
                double s = rand() % 1000;

                a[i].l = s;
                a[i].r = s + 1 + rand() % 50;
            }

            double bp1, bp2;

            int r1 = maxCoveragePoint(a, size, &bp1);
            int r2 = bruteForce(a, size, &bp2);

            if (r1 != r2)
                mismatches++;

            free(a);
        }

        printf("n = %-6d 10 trials done\n", size);
    }

    printf("Mismatches with brute force = %d\n", mismatches);

    return 0;
}