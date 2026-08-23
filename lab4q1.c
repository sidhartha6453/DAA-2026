#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int num;
    char color;   // R, B, or Y
} Item;

int colorIndex(char c) {
    if (c == 'R') return 0;
    if (c == 'B') return 1;
    return 2;   // Y
}

void sortByColor(Item in[], Item out[], int n) {
    int count[3] = {0, 0, 0};

    // Count each color
    for (int i = 0; i < n; i++) {
        count[colorIndex(in[i].color)]++;
    }

    // Starting positions
    int start[3];
    start[0] = 0;
    start[1] = count[0];
    start[2] = count[0] + count[1];

    int pos[3] = {start[0], start[1], start[2]};

    // Place elements in output array
    // Original order is maintained within each color
    for (int i = 0; i < n; i++) {
        int c = colorIndex(in[i].color);
        out[pos[c]++] = in[i];
    }
}

int validate(Item out[], int n) {
    // Check colors are in R -> B -> Y order
    for (int i = 1; i < n; i++) {
        if (colorIndex(out[i - 1].color) > colorIndex(out[i].color))
            return 0;
    }

    // Check numbers remain sorted within each color
    int last[3] = {-2147483647, -2147483647, -2147483647};

    for (int i = 0; i < n; i++) {
        int c = colorIndex(out[i].color);

        if (out[i].num < last[c])
            return 0;

        last[c] = out[i].num;
    }

    return 1;
}

int main() {
    int n;

    printf("Enter number of items: ");
    scanf("%d", &n);

    Item *in = malloc(n * sizeof(Item));
    Item *out = malloc(n * sizeof(Item));

    printf("Enter %d pairs (number color[R/B/Y]), sorted by number:\n", n);

    for (int i = 0; i < n; i++) {
        char c;
        scanf("%d %c", &in[i].num, &c);
        in[i].color = c;
    }

    sortByColor(in, out, n);

    printf("\nSorted by colour:\n");

    for (int i = 0; i < n; i++) {
        printf("(%d,%c) ", out[i].num, out[i].color);
    }

    printf("\nValid: %s\n", validate(out, n) ? "YES" : "NO");

    free(in);
    free(out);

    return 0;
}