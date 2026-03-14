#include "field_info.h"
#include "complex.h"
#include <stdio.h>

static void int_add(const void* a, const void* b, void* result) {
    *(int*)result = *(const int*)a + *(const int*)b;
}

static void int_mul(const void* a, const void* b, void* result) {
    *(int*)result = (*(const int*)a) * (*(const int*)b);
}

static void int_scalar_mul(const void* value, const void* scalar, void* result) {
    *(int*)result = (*(const int*)value) * (*(const int*)scalar);
}

static void int_copy(const void* src, void* dst) {
    *(int*)dst = *(const int*)src;
}

static void int_print(const void* value) {
    printf("%d", *(const int*)value);
}

static void int_destroy(void* value) {
    (void)value;
}

static int int_same_scalar_type(const FieldInfo* other) {
    return other == get_int_field_info();
}

static int complex_same_scalar_type(const FieldInfo* other) {
    return other == get_complex_field_info();
}

const FieldInfo* get_int_field_info(void) {
    static FieldInfo info = {
        sizeof(int),
        int_add,
        int_mul,
        int_scalar_mul,
        int_copy,
        int_print, 
        int_destroy, 
        int_same_scalar_type
    };
    return &info;
}

const FieldInfo* get_complex_field_info(void) {
    static FieldInfo info = {
        sizeof(Complex),
        complex_add,
        complex_mul,
        complex_scalar_mul,
        complex_copy,
        complex_print,
        complex_destroy,
        complex_same_scalar_type
    };
    return &info;
}