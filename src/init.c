#include <stdio.h>
#include <stdlib.h>

#define M 128
#define N 128
#define K 128

#define ELEM_2D(array, i, j, ld) (*((array) + i*ld + j))

void init(int m, int n, int k, float *A, float *B, float *C)
{
	for (int i = 0; i < m; ++i)
		for (int j = 0; j < k; ++j)
			ELEM_2D(A, i, j, k) = ((float) ((i+1) * (j+1) % n) / m);

	for (int i = 0; i < k; ++i)
		for (int j = 0; j < n; ++j)
			ELEM_2D(B, i, j, n) = ((float) ((i+1) * (j+1) % k) / m);

	for (int i = 0; i < m; ++i)
		for (int j = 0; j < n; ++j)
			ELEM_2D(C, i, j, n) = ((float) ((i) * (j) ) / m);
}

int main(int argc, char **argv)
{
	int ret = 0;

 	float alpha = 32412.0, beta = 2124.0;	
	float A[M*K];
	float B[K*N];
	float C[M*N];
	init(M, N, K, A, B, C);

	printf ("{ \n");
	printf ("\"A\": [ ");
	for (int i = 0; i < K; i++) {
		for (int j = 0; j < N; ++j) {
			printf (" %lf ", ELEM_2D(A, i, j, N));
			if ((i == K - 1) && (j == N - 1)) printf(" "); else printf (", ");
		}
	}
	printf ("], \n");

	printf ("\"B\": [ ");
	for (int i = 0; i < K; i++) {
		for (int j = 0; j < N; ++j) {
			printf (" %lf ", ELEM_2D(B, i, j, N));
			if ((i == K - 1) && (j == N - 1)) printf(" "); else printf (", ");
		}
	}
	printf ("], \n");

	printf ("\"C\": [ ");
	for (int i = 0; i < K; i++) {
		for (int j = 0; j < N; ++j) {
			printf (" %lf ", ELEM_2D(C, i, j, N));
			if ((i == K - 1) && (j == N - 1)) printf(" "); else printf (", ");
		}
	}
	printf ("], \n");
	printf("\"alpha\" : %lf,\n", alpha); 
	printf("\"beta\" : %lf,\n", beta); 
	printf("\"M\" : %d,\n", M); 
	printf("\"K\" : %d,\n", K); 
	printf("\"N\" : %d\n", N); 
	printf ("} \n");

	return ret;
}
