#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>

#define N 1024

// Unoptimized Cloud AI Model (32-bit Floats) - 4MB per matrix
float A_fp32[N][N];
float B_fp32[N][N];
float C_fp32[N][N];

// Optimized Edge AI Model (8-bit Integers) - 1MB per matrix
int8_t A_int8[N][N];
int8_t B_int8[N][N];
int32_t C_int32[N][N]; // 32-bit accumulation to prevent overflow

double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}

int main() {
    printf("Initializing Neural Network Tensors...\n");
    // Populate tensors with dummy data
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A_fp32[i][j] = 1.0f; B_fp32[i][j] = 1.0f; C_fp32[i][j] = 0.0f;
            A_int8[i][j] = 1;    B_int8[i][j] = 1;    C_int32[i][j] = 0;
        }
    }

    printf("\n--- Executing FP32 (Cloud) Inference Layer ---\n");
    double start_f = get_time();
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < N; k++) {
            for (int j = 0; j < N; j++) {
                C_fp32[i][j] += A_fp32[i][k] * B_fp32[k][j];
            }
        }
    }
    double end_f = get_time();
    printf("FP32 Execution Time: %f seconds\n", end_f - start_f);

    printf("\n--- Executing INT8 (Edge) Inference Layer ---\n");
    double start_i = get_time();
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < N; k++) {
            for (int j = 0; j < N; j++) {
                C_int32[i][j] += A_int8[i][k] * B_int8[k][j];
            }
        }
    }
    double end_i = get_time();
    printf("INT8 Execution Time: %f seconds\n", end_i - start_i);

    // Calculate Speedup
    printf("\nQuantization Speedup: %.2fx faster\n", (end_f - start_f) / (end_i - start_i));

    return 0;
}