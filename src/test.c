#include <stdio.h>
#include <stdlib.h>

#include <sgemm.h>
#include <jansson.h>

int fileread(char **ptr, ssize_t * len);

#define ELEM_2D(array, i, j, ld) (*((array) + i*ld + j))

//#define M 128
//#define N 128
//#define K 128

/*
 * Parse text into a JSON object. If text is valid JSON, returns a
 * json_t structure, otherwise prints and error and returns null.
 */
json_t *load_json(const char *text) {
    json_t *root;
    json_error_t error;

    root = json_loads(text, 0, &error);

    if (root) {
        return root;
    } else {
        fprintf(stderr, "json error on line %d: %s\n", error.line, error.text);
        return (json_t *)0;
    }
}


int load_input(char *ptr, int *M, int *N, int *K, float **inA, float **inB, float **inC, float *alpha, float *beta) {

        json_t *root = load_json(ptr);

	*alpha = json_real_value(json_object_get(root, "alpha"));
	*beta = json_real_value(json_object_get(root, "beta"));
	*M = json_integer_value(json_object_get(root, "M"));
	*N = json_integer_value(json_object_get(root, "N"));
	*K = json_integer_value(json_object_get(root, "K"));
	int m = *M, n = *N, k = *K;
	//fprintf(stderr,"m: %d, %d\n", m, *M);
	//fprintf(stderr,"n: %d, %d\n", n, *N);
	//fprintf(stderr,"k: %d, %d\n", k, *K);
	float *A = malloc(m * k * sizeof(float) * sizeof(float));
	float *B = malloc(k * n * sizeof(float) * sizeof(float));
	float *C = malloc(m * n * sizeof(float) * sizeof(float));

	json_t *arrayA = json_object_get(root, "A");
	json_t *element = arrayA;
        for (int i = 0; i < m; ++i)
		for (int j = 0; j < k; ++j)
			ELEM_2D(A, i, j, k) =  json_real_value(json_array_get(element, i * k + j));

	json_t *arrayB = json_object_get(root, "B");
	element = arrayB;

        for (int i = 0; i < k; ++i)
		for (int j = 0; j < n; ++j)
			ELEM_2D(B, i, j, n) =  json_real_value(json_array_get(element, i * n + j));

	json_t *arrayC = json_object_get(root, "C");
	element = arrayC;

        for (int i = 0; i < m; ++i)
		for (int j = 0; j < n; ++j)
			ELEM_2D(C, i, j, n) =  json_real_value(json_array_get(element, i * n + j));

	*inA = A;
	*inB = B;
	*inC = C;
        if (root) {
            json_decref(root);
        }

    return 0;

}

void print_output(int m, int n, int k, float C[])
{
	printf("{ \"C\" : [ ");
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; ++j) {
			printf (" %lf", ELEM_2D(C, i, j, n));
			if ((i == k - 1) && (j == n - 1)) printf(" "); else printf (", ");
		}
	}
	printf(" ] }\n");

}
int main(int argc, char **argv)
{
	char *ptr;
	ssize_t len;
	int ret = 0;

	float *A;
	float *B;
	float *C;
 	float alpha, beta;
	int M, N, K;

	/* Read Input */
	fileread(&ptr, &len);
	load_input(ptr, &M, &N, &K, &A, &B, &C, &alpha, &beta);
	//fprintf(stderr, "alpha:%lf\n", alpha);
	//fprintf(stderr, "beta:%lf\n", beta);

	ret = sgemmCPU(M, N, K, alpha, beta, A, B, C);

	print_output(M, N, K, C);

	free(A);
	free(B);
	free(C);

	return ret;
}
