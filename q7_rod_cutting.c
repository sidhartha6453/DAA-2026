// Input: n, then prices p1..pn.  Time O(n^2), Space O(n)
#include <stdio.h>
int p[1001], r[1001], cut[1001];
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", &p[i]);
    for (int j = 1; j <= n; j++)
        for (int i = 1; i <= j; i++)
            if (p[i] + r[j-i] > r[j]) r[j] = p[i] + r[j-i], cut[j] = i;
    printf("Max revenue = %d, pieces:", r[n]);
    for (int j = n; j > 0; j -= cut[j]) printf(" %d", cut[j]);
    puts("");
}
