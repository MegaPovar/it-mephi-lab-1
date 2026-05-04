#ifndef MATRIX_H
#define MATRIX_H

#include "field_info.h"

typedef struct Matrix {
    int size;
    void* data;
    const FieldInfo* field_info;
} Matrix;

int matrix_init(Matrix* matrix, int size, const FieldInfo* field_info);
void matrix_destroy_content(Matrix* matrix);

void* matrix_at(Matrix* matrix, int row, int col);
const void* matrix_at_const(const Matrix* matrix, int row, int col);

int matrix_set(Matrix* matrix, int row, int col, const void* value);

int matrix_add(const Matrix* a, const Matrix* b, Matrix* result);
int matrix_mul(const Matrix* a, const Matrix* b, Matrix* result);
int matrix_scalar_mul(const Matrix* matrix, const void* scalar, const FieldInfo* scalar_type, Matrix* result);

#endif
