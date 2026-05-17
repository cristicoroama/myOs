#include "../include/math.h"

int abs(int n) {
    return n < 0 ? -n : n;
}

int min(int a, int b) {
    return a < b ? a : b;
}

int max(int a, int b) {
    return a > b ? a : b;
}

int clamp(int val, int lo, int hi) {
    if (val < lo) return lo;
    if (val > hi) return hi;
    return val;
}

long pow(long base, int exp) {
    long result = 1;
    while (exp-- > 0) result *= base;
    return result;
}
