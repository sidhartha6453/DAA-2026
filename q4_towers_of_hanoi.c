/*
 * DAA Lab-01, Q4: Towers of Hanoi
 * ---------------------------------------------------------------
 * The rule for solving Towers of Hanoi with n discs:
 *   1. Move the top (n-1) discs from the "from" peg to the "helper"
 *      peg (using the "to" peg as a spare).
 *   2. Move the last, biggest disc straight from "from" to "to".
 *   3. Move the (n-1) discs from "helper" to "to" (using "from" as
 *      the spare this time).
 *
 * That's it! It's a classic example of recursion: solving the big
 * problem by solving two smaller versions of the same problem.
 *
 * We count every single move, then look at how that count grows as
 * we add more discs.
 */

#include <stdio.h>
#include <stdlib.h>

long total_moves = 0;   /* keeps count across recursive calls */

/* Solve Towers of Hanoi for "num_discs" discs.
   from_peg / helper_peg / to_peg are just labels like 'A', 'B', 'C'.
   If show_each_move is 1, we print every move as it happens. */
void solve_hanoi(int num_discs, char from_peg, char helper_peg, char to_peg, int show_each_move) {
    if (num_discs == 0) {
        return;   /* nothing to move -- we're done */
    }

    /* Step 1: move the top (num_discs - 1) discs out of the way */
    solve_hanoi(num_discs - 1, from_peg, to_peg, helper_peg, show_each_move);

    /* Step 2: move the last big disc */
    total_moves++;
    if (show_each_move) {
        printf("Move disc %d from peg %c to peg %c\n", num_discs, from_peg, to_peg);
    }

    /* Step 3: move the (num_discs - 1) discs on top of the big one */
    solve_hanoi(num_discs - 1, helper_peg, from_peg, to_peg, show_each_move);
}

int main(void) {
    printf("=== Example: solving Towers of Hanoi for 4 discs ===\n");
    total_moves = 0;
    solve_hanoi(4, 'A', 'B', 'C', 1);   /* 1 = print every move */
    printf("Total moves used: %ld (the formula 2^n - 1 predicts 2^4 - 1 = 15)\n\n", total_moves);

    FILE *csv_file = fopen("hanoi_moves.csv", "w");
    if (csv_file == NULL) {
        printf("Could not create the CSV file.\n");
        return 1;
    }
    fprintf(csv_file, "n,moves\n");

    printf("=== How many moves are needed for n = 1 up to n = 20 discs? ===\n");
    printf("%-6s %-10s\n", "n", "moves");
    for (int n = 1; n <= 20; n++) {
        total_moves = 0;
        solve_hanoi(n, 'A', 'B', 'C', 0);   /* 0 = don't print, just count */
        printf("%-6d %-10ld\n", n, total_moves);
        fprintf(csv_file, "%d,%ld\n", n, total_moves);
    }
    fclose(csv_file);

    printf("\nSaved results to hanoi_moves.csv -- try plotting 'moves' against\n");
    printf("'n'. You'll see the line stays low for a while, then shoots\n");
    printf("upward very steeply -- that's what EXPONENTIAL growth looks like.\n\n");

    printf("What to conclude:\n");
    printf(" - Solving n discs always takes exactly 2^n - 1 moves.\n");
    printf(" - Each extra disc DOUBLES the work (plus one extra move), so\n");
    printf("   the number of moves grows exponentially, not just quickly --\n");
    printf("   which is why n=64 (the famous 'legend' version of the puzzle)\n");
    printf("   would take billions of years to finish, even moving one disc\n");
    printf("   per second!\n");

    return 0;
}
