#include "complex.h"
#include <stdio.h>

Complex complex_create(double re, double im) {
    Complex z;
    z.re = re;
    z.im = im;
    return z;
}

void complex_add(const void* a, const void* b, void* result) {
    const Complex* x = (const Complex*)a;
    const Complex* y = (const Complex*)b;
    Complex* r = (Complex*)result;

    r->re = x->re + y->re;
    r->im = x->im + y->im;
}

void complex_mul(const void* a, const void* b, void* result) {
    const Complex* x = (const Complex*)a;
    const Complex* y = (const Complex*)b;
    Complex* r = (Complex*)result;

    r->re = x->re * y->re - x->im * y->im;
    r->im = x->re * y->im + x->im * y->re;
}

void complex_scalar_mul(const void* value, const void* scalar, void* result) {
    const Complex* x = (const Complex*)value;
    const Complex* k = (const Complex*)scalar;
    Complex* r = (Complex*)result;

    r->re = x->re * k->re - x->im * k->im;
    r->im = x->re * k->im + x->im * k->re;
}

void complex_copy(const void* src, void* dst) {
    *(Complex*)dst = *(const Complex*)src;
}

void complex_print(const void* value) {
    const Complex* z = (const Complex*)value;
    if (z->im >= 0) {
        printf("%.2lf+%.2lfi", z->re, z->im);
    } else {
        printf("%.2lf%.2lfi", z->re, z->im);

    }
}

void complex_destroy(void* value) {
    (void)value;
}