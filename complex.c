#include "complex.h"
#include <stdio.h>

Complex complex_create(double re, double im) { // собрать complex
    Complex z;
    z.re = re;
    z.im = im;
    return z;
}

void complex_add(const void* a, const void* b, void* result) { // сложение complex
    const Complex* x = (const Complex*)a;
    const Complex* y = (const Complex*)b;
    Complex* r = (Complex*)result;

    r->re = x->re + y->re;
    r->im = x->im + y->im;
}

void complex_mul(const void* a, const void* b, void* result) { // умножение complex
    const Complex* x = (const Complex*)a;
    const Complex* y = (const Complex*)b;
    Complex* r = (Complex*)result;

    r->re = x->re * y->re - x->im * y->im;
    r->im = x->re * y->im + x->im * y->re;
}

void complex_scalar_mul(const void* value, const void* scalar, void* result) { // complex на complex
    const Complex* x = (const Complex*)value;
    const Complex* k = (const Complex*)scalar;
    Complex* r = (Complex*)result;

    r->re = x->re * k->re - x->im * k->im;
    r->im = x->re * k->im + x->im * k->re;
}

void complex_zero(void* result) { // нулевой complex
    Complex* r = (Complex*)result;

    r->re = 0.0;
    r->im = 0.0;
}

void complex_copy(const void* src, void* dst) { // копирование complex
    *(Complex*)dst = *(const Complex*)src;
}

void complex_print(const void* value) { // вывод complex
    const Complex* z = (const Complex*)value;
    if (z->im >= 0) {
        printf("%.2lf+%.2lfi", z->re, z->im);
    } else {
        printf("%.2lf%.2lfi", z->re, z->im);

    }
}

void complex_destroy(void* value) { // тут освобождать нечего
    (void)value;
}
