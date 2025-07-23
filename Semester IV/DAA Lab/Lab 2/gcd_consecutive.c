#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int gcd(int m, int n, int* iter) {
	int i = m<n ? m : n;
	for (; i>1; i--) {
		(*iter)++;
		if (m%i == 0 && n%i == 0) break;
	}
	return i;
}

int main() {
    int a, b, iter = 0;
    printf("Enter two numbers: ");
    scanf("%d %d", &a, &b);

    clock_t start = clock();
    int result = gcd(a, b, &iter);
    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("GCD of %d and %d is: %d\n", a, b, result);
    printf("Iterations run: %d\n", iter);
    printf("Time taken: %f seconds\n", time_taken);

    return 0;
}