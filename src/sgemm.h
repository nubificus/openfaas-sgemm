#pragma once

int sgemmCPU(
	int m, int n, int k,
	float alpha, float beta,
	float *a, float *b, float *c
);

