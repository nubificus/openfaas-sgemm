#include <stdio.h>
#include <sys/time.h>

#include "sgemm.h"

int sgemmCPU(
	int M, int N, int K,
	float alpha, float beta,
	float *A, float *B, float *C) 
{
	struct timeval t0, t1;
	double t10;

	gettimeofday(&t0, NULL);
	for (int i = 0; i < M; ++i) {
		for (int j = 0; j < N; ++j) {
			C[i * N + j] *= beta;
			for (int k = 0; k < K; ++k)
				C[i * N + j] += alpha * A[i * K + k] * B[k * N + j];
		}
	}
	gettimeofday(&t1, NULL);
	t10 = (t1.tv_sec*1000000.0 + t1.tv_usec) - (t0.tv_sec*1000000.0 + t0.tv_usec);
	fprintf(stderr, "gemm_kernel CPU time:\n%lf msecs\n", t10/1000.0f);
	fprintf(stderr, "\n--------------------------\n");

	return 0;
}

