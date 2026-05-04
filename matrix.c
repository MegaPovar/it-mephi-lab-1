#include "matrix.h"

#include <stdlib.h>

static int field_info_is_valid(const FieldInfo* field_info) { // проверка описания типа
    return field_info != NULL
        && field_info->element_size > 0
        && field_info->add != NULL
        && field_info->mul != NULL
        && field_info->scalar_mul != NULL
        && field_info->zero != NULL
        && field_info->copy != NULL
        && field_info->print != NULL
        && field_info->destroy != NULL
        && field_info->is_same_scalar_type != NULL;
}

int matrix_init(Matrix* matrix, int size, const FieldInfo* field_info) { // создаю матрицу
    size_t i;
    size_t total;

    if (matrix == NULL || size <= 0 || !field_info_is_valid(field_info)) {
        return 0;
    }

    if ((size_t)size > ((size_t)-1) / (size_t)size) {
        return 0;
    }

    total = (size_t)size * (size_t)size;
    if (total > ((size_t)-1) / field_info->element_size) {
        return 0;
    }

    matrix->size = size;
    matrix->field_info = field_info;
    matrix->data = calloc(total, field_info->element_size);

    if (matrix->data == NULL) {
        matrix->size = 0;
        matrix->field_info = NULL;
        return 0;
    }

    for (i = 0; i < total; i++) {
        void* elem = (char*)matrix->data + i * field_info->element_size;
        field_info->zero(elem);
    }

    return 1;
}

void matrix_destroy_content(Matrix* matrix) { // чистка памяти матрицы
    size_t i;
    size_t total;

    if (matrix == NULL || matrix->data == NULL) {
        return;
    }

    if (matrix->field_info != NULL && matrix->field_info->destroy != NULL) {
        total = (size_t)matrix->size * (size_t)matrix->size;
        for (i = 0; i < total; i++) {
            void* elem = (char*)matrix->data + i * matrix->field_info->element_size;
            matrix->field_info->destroy(elem);
        }
    }

    free(matrix->data);
    matrix->data = NULL;
    matrix->field_info = NULL;
    matrix->size = 0;
}

void* matrix_at(Matrix* matrix, int row, int col) { // доступ к элементу
    if (matrix == NULL || matrix->data == NULL) {
        return NULL;
    }

    if (row < 0 || row >= matrix->size || col < 0 || col >= matrix->size) {
        return NULL;
    }

    return (char*)matrix->data +
           (row * matrix->size + col) * matrix->field_info->element_size;
}

const void* matrix_at_const(const Matrix* matrix, int row, int col) { // то же самое, но без изменения
    if (matrix == NULL || matrix->data == NULL) {
        return NULL;
    }

    if (row < 0 || row >= matrix->size || col < 0 || col >= matrix->size) {
        return NULL;
    }

    return (const char*)matrix->data +
           (row * matrix->size + col) * matrix->field_info->element_size;
}

int matrix_set(Matrix* matrix, int row, int col, const void* value) { // записать элемент
    void* cell;

    if (matrix == NULL || value == NULL || matrix->field_info == NULL) {
        return 0;
    }

    cell = matrix_at(matrix, row, col);
    if (cell == NULL) {
        return 0;
    }

    if (cell == value) {
        return 1;
    }

    matrix->field_info->destroy(cell);
    matrix->field_info->copy(value, cell);
    return 1;
}

int matrix_add(const Matrix* a, const Matrix* b, Matrix* result) { // сложение по ячейкам
    int i;
    int j;

    if (a == NULL || b == NULL || result == NULL) {
        return 0;
    }

    if (a->data == NULL || b->data == NULL || result->data == NULL) {
        return 0;
    }

    if (a->size != b->size || a->size != result->size) {
        return 0;
    }

    if (a->field_info != b->field_info || a->field_info != result->field_info) {
        return 0;
    }

    for (i = 0; i < a->size; i++) {
        for (j = 0; j < a->size; j++) {
            a->field_info->add(
                matrix_at_const(a, i, j),
                matrix_at_const(b, i, j),
                matrix_at(result, i, j)
            );
        }
    }
    return 1;
}

int matrix_scalar_mul(const Matrix* matrix, const void* scalar,
                      const FieldInfo* scalar_type, Matrix* result) { // умножение на скаляр
    int i;
    int j;

    if (matrix == NULL || scalar == NULL || result == NULL) {
        return 0;
    }

    if (matrix->data == NULL || result->data == NULL) {
        return 0;
    }

    if (matrix->size != result->size) {
        return 0;
    }

    if (matrix->field_info != result->field_info) {
        return 0;
    }

    if (matrix->field_info == NULL || scalar_type == NULL) {
        return 0;
    }

    if (!matrix->field_info->is_same_scalar_type(scalar_type)) {
        return 0;
    }

    for (i = 0; i < matrix->size; i++) {
        for (j = 0; j < matrix->size; j++) {
            matrix->field_info->scalar_mul(
                matrix_at_const(matrix, i, j),
                scalar,
                matrix_at(result, i, j)
            );
        }
    }
    return 1;
}

int matrix_mul(const Matrix* a, const Matrix* b, Matrix* result) { // обычное умножение матриц
    int i;
    int j;
    int k;
    void* tmp1;
    void* tmp2;
    size_t size;

    if (a == NULL || b == NULL || result == NULL) {
        return 0;
    }

    if (a->data == NULL || b->data == NULL || result->data == NULL) {
        return 0;
    }

    if (a->size != b->size || a->size != result->size) {
        return 0;
    }

    if (a->field_info != b->field_info || a->field_info != result->field_info) {
        return 0;
    }

    size = a->field_info->element_size;

    tmp1 = malloc(size); // временно храню произведение
    tmp2 = malloc(size);

    if (tmp1 == NULL || tmp2 == NULL) {
        free(tmp1);
        free(tmp2);
        return 0;
    }

    for (i = 0; i < a->size; i++) {
        for (j = 0; j < a->size; j++) {
            a->field_info->zero(matrix_at(result, i, j)); // сначала ставлю ноль

            for (k = 0; k < a->size; k++) {
                a->field_info->mul(
                    matrix_at_const(a, i, k),
                    matrix_at_const(b, k, j),
                    tmp1
                );

                a->field_info->add(
                    matrix_at(result, i, j),
                    tmp1,
                    tmp2
                );

                a->field_info->copy(tmp2, matrix_at(result, i, j));
            }
        }
    }

    free(tmp1);
    free(tmp2);
    
    return 1;
}
