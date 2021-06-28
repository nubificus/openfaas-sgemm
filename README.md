# openfaas-sgemm

This repo hosts an example sgemm CPU operation for OpenFaaS.

### Input

The input is provided via a json file (needs libjansson) through STDIN. 

```
A[M*K],
B[K*N],
C[M*N],
M,
N,
K,
alpha,
beta
```

An example input file is produced by running:

```
./src/init > input.txt
```

### Output

The output is `C[i][j] *= beta;` + `C[i][j] += alpha * A[i][k] * B[k][j];`,
also provided as a json file.

### Build as a standalone app

To build enter the src directory and type `make`. Two libraries and an
executable should be built:

```
libfileread.so
libsgemm.so
test
```

To build as an OpenFaaS function from the root directory type:

```
docker build -t user/sgemmcpu:latest -f Dockerfile .
```

then push to the dockerhub:

```
docker push user/sgemmcpu:latest
```

and use the stack-sgemm-cpu.yml to deploy to your openfaas installation:

```
faas-cli deploy -f stack-sgemm-cpu.yml
```

Make sure to use your own gateway param, as well as your own openfaas profile
annotation.
