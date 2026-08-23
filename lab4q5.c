#include <stdio.h>
#include <stdlib.h>

typedef struct { double x, y; } Interval;

int cmp(const void *a, const void *b) {
    double d = ((Interval *)a)->x - ((Interval *)b)->x;
    return (d > 0) - (d < 0);
}

/* O(n log n): sort by start, then merge in one pass */
int mergeIntervals(Interval in[], int n, Interval out[]) {
    if (n == 0) return 0;

    Interval *tmp = malloc(n * sizeof(Interval));

    for (int i = 0; i < n; i++)
        tmp[i] = in[i];

    qsort(tmp, n, sizeof(Interval), cmp);

    int m = 0;
    out[0] = tmp[0];

    for (int i = 1; i < n; i++) {
        if (tmp[i].x <= out[m].y) /* overlaps (or touches) current merged interval */
            out[m].y = out[m].y > tmp[i].y ? out[m].y : tmp[i].y;
        else
            out[++m] = tmp[i];
    }

    free(tmp);
    return m + 1;
}

int covered(Interval list[], int n, double t) {
    for (int i = 0; i < n; i++)
        if (list[i].x <= t && t <= list[i].y)
            return 1;

    return 0;
}

/* Validate: (a) output intervals are sorted & non-overlapping,
   (b) union of output == union of input, checked at many sample points */
int validate(Interval in[], int n, Interval out[], int m) {
    for (int i = 1; i < m; i++)
        if (out[i].x <= out[i - 1].y)
            return 0;

    double lo = in[0].x, hi = in[0].y;

    for (int i = 1; i < n; i++) {
        if (in[i].x < lo) lo = in[i].x;
        if (in[i].y > hi) hi = in[i].y;
    }

    for (double t = lo - 1; t <= hi + 1; t += 0.5)
        if (covered(in, n, t) != covered(out, m, t))
            return 0;

    return 1;
}

int main() {
    int n;

    printf("Enter number of intervals: ");
    scanf("%d", &n);

    Interval *in = malloc(n * sizeof(Interval));
    Interval *out = malloc(n * sizeof(Interval));

    printf("Enter %d (x y) pairs:\n", n);

    for (int i = 0; i < n; i++)
        scanf("%lf %lf", &in[i].x, &in[i].y);

    int m = mergeIntervals(in, n, out);

    printf("\nMerged intervals: ");

    for (int i = 0; i < m; i++)
        printf("(%g,%g) ", out[i].x, out[i].y);

    printf("\nValid: %s\n\n",
           validate(in, n, out, m) ? "YES" : "NO");

    free(in);
    free(out);

    srand(1);

    printf("Randomised validation:\n");

    int mismatches = 0;

    for (int size = 5; size <= 5000; size *= 4) {

        for (int trial = 0; trial < 10; trial++) {

            Interval *a = malloc(size * sizeof(Interval));
            Interval *b = malloc(size * sizeof(Interval));

            for (int i = 0; i < size; i++) {
                double s = rand() % 1000;
                a[i].x = s;
                a[i].y = s + 1 + rand() % 20;
            }

            int mm = mergeIntervals(a, size, b);

            if (!validate(a, size, b, mm))
                mismatches++;

            free(a);
            free(b);
        }

        printf("n = %-6d 10 trials done\n", size);
    }

    printf("Failures = %d\n", mismatches);

    return 0;
}