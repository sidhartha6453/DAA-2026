/*
 * DAA Lab-01, Q2: Fair vs Biased coin
 * ---------------------------------------------------------------
 * Idea: "toss a coin" by generating a random number between 0 and 1.
 *   - If that number is less than 0.5   -> call it HEAD (fair coin)
 *   - If that number is less than 0.7   -> call it HEAD (biased coin)
 *   ...and so on. Whatever probability we pick, that's how often
 *   HEAD should come up if we toss enough times.
 *
 * We toss the coin many times, count how many HEADs we got, and
 * divide by the number of tosses. That gives us the "experimental"
 * probability of getting a HEAD -- and it should get closer and
 * closer to the TRUE probability as we toss more times.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Toss one coin. probability_of_head is a number between 0 and 1.
   Returns 1 for HEAD, 0 for TAIL. */
int toss_coin(double probability_of_head) {
    double random_number = (double) rand() / RAND_MAX;   /* between 0.0 and 1.0 */
    if (random_number < probability_of_head) {
        return 1;   /* HEAD */
    } else {
        return 0;   /* TAIL */
    }
}

/* Toss the coin "num_tosses" times and report the results */
void run_coin_experiment(const char *coin_name, double true_probability, int num_tosses) {
    int heads_count = 0;

    for (int i = 0; i < num_tosses; i++) {
        if (toss_coin(true_probability) == 1) {
            heads_count++;
        }
    }

    double experimental_probability = (double) heads_count / num_tosses;

    printf("%-10s | true P(head)=%.2f | tosses=%-8d | heads=%-8d | experimental P(head)=%.4f\n",
           coin_name, true_probability, num_tosses, heads_count, experimental_probability);
}

int main(void) {
    srand((unsigned) time(NULL));   /* different random numbers each run */

    printf("=== Part 1: Fair coin -- watch the probability settle near 0.5 ===\n");
    run_coin_experiment("Fair", 0.5, 100);
    run_coin_experiment("Fair", 0.5, 1000);
    run_coin_experiment("Fair", 0.5, 10000);
    run_coin_experiment("Fair", 0.5, 100000);
    run_coin_experiment("Fair", 0.5, 1000000);

    printf("\n=== Part 2: Fair coin vs several Biased coins (same number of tosses) ===\n");
    run_coin_experiment("Fair",     0.5, 1000000);
    run_coin_experiment("Biased-A", 0.7, 1000000);
    run_coin_experiment("Biased-B", 0.3, 1000000);
    run_coin_experiment("Biased-C", 0.9, 1000000);

    printf("\nWhat to notice:\n");
    printf(" - The fair coin's experimental probability gets closer to 0.5\n");
    printf("   the more times we toss it.\n");
    printf(" - Each biased coin settles near its OWN true probability instead\n");
    printf("   (0.7, 0.3, or 0.9) -- it never drifts back toward 0.5.\n");

    return 0;
}
