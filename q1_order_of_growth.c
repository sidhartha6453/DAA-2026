/*
 * DAA Lab-01, Q1: Put the functions in order of growth
 * ---------------------------------------------------------------
 * We have 12 functions of n. We want to know: as n gets bigger and
 * bigger, which function grows the SLOWEST, and which grows the
 * FASTEST?
 *
 * How this program works (in plain steps):
 *   1. Pick a fairly large n (n = 30 here -- big enough to see the
 *      trend, small enough that none of the numbers overflow).
 *   2. Calculate the value of every function at that n.
 *   3. Sort the 12 values from smallest to largest using a simple
 *      bubble sort (same idea you'll use in Q3!).
 *   4. Print the sorted list.
 *
 * A NOTE ABOUT 2^32 * n:
 *   This looks scary because 2^32 is a huge number (about 4.3
 *   billion). But it is just a CONSTANT multiplied by n, so this
 *   function is still only "linear" in n -- it grows at the same
 *   rate as n itself, just with a big multiplier.
 */

#include <stdio.h>
#include <math.h>

#define NUM_FUNCTIONS 12

int main(void) {

    /* The names of our functions, just for printing nicely */
    const char *names[NUM_FUNCTIONS] = {
        "1/n",
        "log2(n)",
        "n^0.51",
        "12*sqrt(n)",
        "n*log2(n)",
        "50*n^0.5",
        "n^2 - 324",
        "2*n^3",
        "100*n^2 + 6*n",
        "n^(log2 n)",
        "2^32 * n",
        "3^n"
    };

    double n = 30.0;

    /* Step 1: calculate every function's value at n = 30 */
    double values[NUM_FUNCTIONS];
    values[0]  = 1.0 / n;
    values[1]  = log2(n);
    values[2]  = pow(n, 0.51);
    values[3]  = 12.0 * sqrt(n);
    values[4]  = n * log2(n);
    values[5]  = 50.0 * sqrt(n);
    values[6]  = n * n - 324.0;
    values[7]  = 2.0 * n * n * n;
    values[8]  = 100.0 * n * n + 6.0 * n;
    values[9]  = pow(n, log2(n));
    values[10] = pow(2.0, 32.0) * n;
    values[11] = pow(3.0, n);

    printf("Values of each function at n = %.0f:\n\n", n);
    for (int i = 0; i < NUM_FUNCTIONS; i++) {
        printf("  %-15s = %.4e\n", names[i], values[i]);
    }

    /* Step 2: sort the functions by their value, smallest first.
       This is a plain bubble sort -- swap names/values together
       whenever the one on the left is bigger than the one on the right. */
    for (int i = 0; i < NUM_FUNCTIONS - 1; i++) {
        for (int j = 0; j < NUM_FUNCTIONS - 1 - i; j++) {
            if (values[j] > values[j + 1]) {
                /* swap the values */
                double temp_val = values[j];
                values[j] = values[j + 1];
                values[j + 1] = temp_val;

                /* swap the matching names too, so they stay lined up */
                const char *temp_name = names[j];
                names[j] = names[j + 1];
                names[j + 1] = temp_name;
            }
        }
    }

    printf("\nSorted from slowest-growing to fastest-growing (at n = %.0f):\n\n", n);
    for (int i = 0; i < NUM_FUNCTIONS; i++) {
        printf("  %2d. %-15s = %.4e\n", i + 1, names[i], values[i]);
    }

    printf("\n--------------------------------------------------------------\n");
    printf("Important: at a SMALL n like 30, constants can still confuse the\n");
    printf("order a little (for example 12*sqrt(n) and n*log2(n) might swap\n");
    printf("places compared to their TRUE long-term order). Try changing\n");
    printf("n = 30 to a much bigger number, like n = 500, and re-run this\n");
    printf("program to see the order settle into its final, true pattern.\n\n");

    printf("The TRUE order of growth, for sufficiently large n, is:\n");
    printf("  1/n  <  log2(n)  <  12*sqrt(n) ~ 50*n^0.5  <  n^0.51  <  2^32*n\n");
    printf("    <  n*log2(n)  <  100n^2+6n ~ n^2-324  <  2*n^3\n");
    printf("    <  n^(log2 n)  <  3^n\n");
    printf("(functions joined by '~' grow at the same rate -- they only\n");
    printf("differ by a constant multiplier)\n");

    return 0;
}
