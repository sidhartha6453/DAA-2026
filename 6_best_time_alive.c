/* Problem 6: The best time to be alive
 * Sweep-line over birth/death events; ties broken so deaths precede
 * births in the same year. O(n log n) time.
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int year;
    int delta;      /* -1 for death, +1 for birth */
    int isDeath;
} Event;

/* Sort by year ascending; at equal year, deaths (-1) before births (+1) */
int cmpEvent(const void *a, const void *b) {
    const Event *e1 = (const Event*)a;
    const Event *e2 = (const Event*)b;
    if (e1->year != e2->year) return e1->year - e2->year;
    return e2->isDeath - e1->isDeath;
}

int main(void) {
    int n;
    printf("Enter number of scientists: ");
    scanf("%d", &n);

    Event *ev = malloc(2 * n * sizeof(Event));
    int *birth = malloc(n * sizeof(int));
    int *death = malloc(n * sizeof(int));

    printf("Enter birth and death year for each scientist:\n");
    for (int i = 0; i < n; i++) {
        int b, d;
        scanf("%d %d", &b, &d);
        birth[i] = b; death[i] = d;
        ev[2*i]     = (Event){ b, +1, 0 };
        ev[2*i + 1] = (Event){ d, -1, 1 };
    }

    qsort(ev, 2*n, sizeof(Event), cmpEvent);

    int maxCount = 0;
    int count = 0;

    int capacity = 16, resultCnt = 0;
    int *bestStart = malloc(capacity * sizeof(int));
    int *bestEnd   = malloc(capacity * sizeof(int));

    int idx = 0;
    while (idx < 2*n) {
        int curYear = ev[idx].year;
        while (idx < 2*n && ev[idx].year == curYear) {
            count += ev[idx].delta;
            idx++;
        }
        int nextYear = (idx < 2*n) ? ev[idx].year : curYear;
        int intervalEnd = (idx < 2*n) ? nextYear - 1 : curYear;

        if (count > maxCount) {
            maxCount = count;
            resultCnt = 0;
            bestStart[resultCnt] = curYear;
            bestEnd[resultCnt] = intervalEnd;
            resultCnt++;
        } else if (count == maxCount && count > 0) {
            if (resultCnt == capacity) {
                capacity *= 2;
                bestStart = realloc(bestStart, capacity * sizeof(int));
                bestEnd   = realloc(bestEnd, capacity * sizeof(int));
            }
            bestStart[resultCnt] = curYear;
            bestEnd[resultCnt] = intervalEnd;
            resultCnt++;
        }
    }

    printf("\nMaximum number of scientists alive simultaneously: %d\n", maxCount);
    printf("Achieved during year interval(s):\n");
    for (int i = 0; i < resultCnt; i++) {
        if (bestStart[i] == bestEnd[i])
            printf("  Year %d\n", bestStart[i]);
        else
            printf("  Years %d - %d\n", bestStart[i], bestEnd[i]);
    }

    /* brute-force cross-check (ignores the tie-rule subtlety) */
    int minYear = birth[0], maxYear = death[0];
    for (int i = 1; i < n; i++) {
        if (birth[i] < minYear) minYear = birth[i];
        if (death[i] > maxYear) maxYear = death[i];
    }
    int bruteMax = 0, bruteYear = minYear;
    for (int y = minYear; y <= maxYear; y++) {
        int c = 0;
        for (int i = 0; i < n; i++)
            if (birth[i] <= y && y <= death[i]) c++;
        if (c > bruteMax) { bruteMax = c; bruteYear = y; }
    }
    printf("\n[Brute-force cross-check, ignoring tie-rule subtlety] max=%d (near year %d)\n",
           bruteMax, bruteYear);

    free(ev); free(birth); free(death); free(bestStart); free(bestEnd);
    return 0;
}
