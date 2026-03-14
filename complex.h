#ifndef COMPLEX_H
#define COMPLEX_H

typedef struct Complex {
    double re;
    double im;
} Complex;

Complex complex_create(double re, double im);
void complex_add(const void* a, const void* b, void* result);
void complex_mul(const void* a, const void* b, void* result);
void complex_scalar_mul(const void* value, const void* scalar, void* result);
void complex_copy(const void* src, void* dst);
void complex_print(const void* value);
void complex_destroy(void* value);

#endif