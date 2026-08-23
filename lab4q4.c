#include <stdio.h>
#include <stdlib.h>

typedef struct {
    double time;
    int type;   /* type: +1 = entry, -1 = exit */
} Event;

int cmp(const void *a, const void *b) {
    double d = ((Event *)a)->time - ((Event *)b)->time;
    return (d > 0) - (d < 0);
}

/* O(n log n): build 2n events, sort by time,
   sweep tracking running count */
int maxPresent(double entry[], double exit[], int n, double *bestTime) {
    Event *ev = malloc(2 * n * sizeof(Event));

    for (int i = 0; i < n; i++) {
        ev[2 * i] = (Event){entry[i], +1};
        ev[2 * i + 1] = (Event){exit[i], -1};
    }

    qsort(ev, 2 * n, sizeof(Event), cmp);

    int count = 0, best = 0;
    *bestTime = ev[0].time;

    for (int i = 0; i < 2 * n; i++) {
        count += ev[i].type;

        if (count > best) {
            best = count;
            *bestTime = ev[i].time;
        }
    }

    free(ev);
    return best;
}

/* O(n^2) brute force: check crowd size at every entry time,
   used only to validate */
int bruteForce(double entry[], double exit[], int n, double *bestTime) {
    int best = 0;

    for (int i = 0; i < n; i++) {
        double t = entry[i];
        int count = 0;

        for (int j = 0; j < n; j++)
            if (entry[j] <= t && t < exit[j])
                count++;

        if (count > best) {
            best = count;
            *bestTime = t;
        }
    }

    return best;
}

int main() {
    int n;

    printf("Enter number of persons: ");
    scanf("%d", &n);

    double *entry = malloc(n * sizeof(double));
    double *exit_ = malloc(n * sizeof(double));

    printf("Enter %d (entry exit) pairs:\n", n);

    for (int i = 0; i < n; i++)
        scanf("%lf %lf", &entry[i], &exit_[i]);

    double t1, t2;

    int m1 = maxPresent(entry, exit_, n, &t1);
    int m2 = bruteForce(entry, exit_, n, &t2);

    printf("\nMax simultaneous people = %d, achieved at time = %.4g\n",
           m1, t1);

    printf("Brute-force check: max = %d at time = %.4g -> %s\n\n",
           m2, t2, m1 == m2 ? "MATCH" : "MISMATCH");

    free(entry);
    free(exit_);

    srand(1);

    printf("Randomised validation:\n");

    int mismatches = 0;

    for (int size = 5; size <= 2000; size *= 4) {

        for (int trial = 0; trial < 5; trial++) {

            double *e = malloc(size * sizeof(double));
            double *x = malloc(size * sizeof(double));

            for (int i = 0; i < size; i++) {
                double a = rand() % 100000
                         + (rand() % 1000) / 1000.0;

                double b = a + 1 + rand() % 5000
                         + (rand() % 1000) / 1000.0;

                e[i] = a;
                x[i] = b;
            }

            double bt1, bt2;

            int r1 = maxPresent(e, x, size, &bt1);
            int r2 = bruteForce(e, x, size, &bt2);

            if (r1 != r2)
                mismatches++;

            free(e);
            free(x);
        }

        printf("n = %-6d 5 trials done\n", size);
    }

    printf("Mismatches with brute force = %d\n", mismatches);

    return 0;
}