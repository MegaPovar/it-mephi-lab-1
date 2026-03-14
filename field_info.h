#ifndef FIELD_INFO_H
#define FIELD_INFO_H

#include <stddef.h>

typedef struct FieldInfo {
    size_t element_size;

    void (*add)(const void* a, const void* b, void* result);
    void (*mul)(const void* a, const void* b, void* result);
    void (*scalar_mul)(const void* value, const void* scalar, void* result);

    void (*copy)(const void* src, void* dst);
    void (*print)(const void* value);
    void (*destroy)(void* value);

    int (*is_same_scalar_type)(const struct FieldInfo* other);
} FieldInfo;

const FieldInfo* get_int_field_info(void);
const FieldInfo* get_complex_field_info(void);

#endif