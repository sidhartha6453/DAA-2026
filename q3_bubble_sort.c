/*
 * DAA Lab-01, Q3: Performance analysis of bubble sort
 * ---------------------------------------------------------------
 * We write TWO versions of bubble sort:
 *
 *   Version A ("smart"): stops early if a full pass makes no swaps
 *                        at all, because that means the array is
 *                        already sorted.
 *
 *   Version B ("plain"): always runs every single pass, even if the
 *                        array became sorted early.
 *
 * For a few different array sizes, we count how many COMPARISONS
 * each version makes, print them, and save them to a CSV file so
 * they can be graphed later (in Excel, Google Sheets, Python, etc).
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Version A: stops early once the array is sorted.
   Returns the number of comparisons it made. */
int bubble_sort_smart(int arr[], int size) {
    int comparisons = 0;

    for (int pass = 0; pass < size - 1; pass++) {
        int did_swap = 0;   /* nothing swapped yet this pass */

        for (int j = 0; j < size - 1 - pass; j++) {
            comparisons++;
            if (arr[j] > arr[j + 1]) {
                /* swap arr[j] and arr[j+1] */
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                did_swap = 1;
            }
        }

        if (did_swap == 0) {
            break;   /* no swaps happened -> array is already sorted, stop! */
        }
    }

    return comparisons;
}

/* Version B: always does every pass, no early stopping.
   Returns the number of comparisons it made. */
int bubble_sort_plain(int arr[], int size) {
    int comparisons = 0;

    for (int pass = 0; pass < size - 1; pass++) {
        for (int j = 0; j < size - 1 - pass; j++) {
            comparisons++;
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
        /* no early-exit check -- keep going no matter what */
    }

    return comparisons;
}

/* Fill an array with random numbers */
void fill_with_random_numbers(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 100000;
    }
}

int main(void) {
    srand((unsigned) time(NULL));

    int sizes_to_test[] = {100, 500, 1000, 2000, 3000, 5000};
    int num_tests = sizeof(sizes_to_test) / sizeof(sizes_to_test[0]);

    FILE *csv_file = fopen("bubble_sort_comparisons.csv", "w");
    if (csv_file == NULL) {
        printf("Could not create the CSV file.\n");
        return 1;
    }
    fprintf(csv_file, "n,smart_comparisons,plain_comparisons\n");

    printf("%-8s %-22s %-22s\n", "n", "smart (early-stop)", "plain (full passes)");

    for (int t = 0; t < num_tests; t++) {
        int size = sizes_to_test[t];

        /* make one random array, then copy it so BOTH sorts start
           from the exact same data -- that's a fair comparison */
        int *original = malloc(size * sizeof(int));
        int *array_for_smart = malloc(size * sizeof(int));
        int *array_for_plain = malloc(size * sizeof(int));

        fill_with_random_numbers(original, size);
        for (int i = 0; i < size; i++) {
            array_for_smart[i] = original[i];
            array_for_plain[i] = original[i];
        }

        int smart_comparisons = bubble_sort_smart(array_for_smart, size);
        int plain_comparisons = bubble_sort_plain(array_for_plain, size);

        printf("%-8d %-22d %-22d\n", size, smart_comparisons, plain_comparisons);
        fprintf(csv_file, "%d,%d,%d\n", size, smart_comparisons, plain_comparisons);

        free(original);
        free(array_for_smart);
        free(array_for_plain);
    }

    fclose(csv_file);

    printf("\nSaved results to bubble_sort_comparisons.csv -- open it in Excel or\n");
    printf("plot it in Python to see the two curves.\n\n");

    printf("What to notice:\n");
    printf(" - 'Plain' always makes the same number of comparisons no matter\n");
    printf("   what the data looks like: n*(n-1)/2, which is Theta(n^2).\n");
    printf(" - 'Smart' also makes about the same number of comparisons on\n");
    printf("   random data (there's rarely a reason to stop early), but if\n");
    printf("   you feed it an ALREADY SORTED array, it finishes after just\n");
    printf("   one pass -- Theta(n) instead of Theta(n^2). Try it!\n");

    return 0;
}
