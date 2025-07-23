#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void sieveOfEratosthenes(int n, int* primes, int* primeCount) {
    int* isPrime = (int*)malloc((n + 1) * sizeof(int));
    for (int i = 2; i <= n; i++) isPrime[i] = 1;

    for (int p = 2; p * p <= n; p++) {
        if (isPrime[p]) {
            for (int multiple = p * p; multiple <= n; multiple += p) {
                isPrime[multiple] = 0;
            }
        }
    }

    *primeCount = 0;
    for (int i = 2; i <= n; i++) {
        if (isPrime[i]) primes[(*primeCount)++] = i;
    }
    free(isPrime);
}

int gcd(int a, int b, int* iter) {
    int max = (a > b) ? a : b;
    int* primes = (int*)malloc((max + 1) * sizeof(int));
    int primeCount;
    sieveOfEratosthenes(max, primes, &primeCount);

    int gcd = 1;
    for (int i = 0; i < primeCount; i++) {
    	(*iter)++;
        while (a % primes[i] == 0 && b % primes[i] == 0) {
        	if ((a/primes[i]) % primes[i] == 0 && (b/primes[i]) % primes[i] == 0) 
            	(*iter)++;
            gcd *= primes[i];
            a /= primes[i];
            b /= primes[i];
        }
    }

    free(primes);
    return gcd;
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