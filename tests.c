#include "tests.h"
#include "matrix.h"
#include "field_info.h"
#include "complex.h"

#include <stdio.h>

static int passed = 0;
static int failed = 0;

static void print_result(const char* name, int ok) {
    if (ok) {
        printf("[OK] %s\n", name);
        passed++;
    } else {
        printf("[FAIL] %s\n", name);
        failed++;
    }
}

static int test_matrix_init_invalid_size(void) {
    Matrix m;
    return matrix_init(&m, 0, get_int_field_info()) == 0;
}

static int test_matrix_add_int(void) {
    Matrix a;
    Matrix b;
    Matrix result;
    int x;
    int ok = 1;

    if (!matrix_init(&a, 2, get_int_field_info())) return 0;
    if (!matrix_init(&b, 2, get_int_field_info())) {
        matrix_destroy_content(&a);
        return 0;
    }
    if (!matrix_init(&result, 2, get_int_field_info())) {
        matrix_destroy_content(&a);
        matrix_destroy_content(&b);
        return 0;
    }

    x = 1; matrix_set(&a, 0, 0, &x);
    x = 2; matrix_set(&a, 0, 1, &x);
    x = 3; matrix_set(&a, 1, 0, &x);
    x = 4; matrix_set(&a, 1, 1, &x);

    x = 5; matrix_set(&b, 0, 0, &x);
    x = 6; matrix_set(&b, 0, 1, &x);
    x = 7; matrix_set(&b, 1, 0, &x);
    x = 8; matrix_set(&b, 1, 1, &x);

    if (!matrix_add(&a, &b, &result)) {
        ok = 0;
    } else {
        if (*(int*)matrix_at(&result, 0, 0) != 6) ok = 0;
        if (*(int*)matrix_at(&result, 0, 1) != 8) ok = 0;
        if (*(int*)matrix_at(&result, 1, 0) != 10) ok = 0;
        if (*(int*)matrix_at(&result, 1, 1) != 12) ok = 0;
    }

    matrix_destroy_content(&a);
    matrix_destroy_content(&b);
    matrix_destroy_content(&result);

    return ok;
}

static int test_matrix_mul_int(void) {
    Matrix a;
    Matrix b;
    Matrix result;
    int x;
    int ok = 1;

    if (!matrix_init(&a, 2, get_int_field_info())) return 0;
    if (!matrix_init(&b, 2, get_int_field_info())) {
        matrix_destroy_content(&a);
        return 0;
    }
    if (!matrix_init(&result, 2, get_int_field_info())) {
        matrix_destroy_content(&a);
        matrix_destroy_content(&b);
        return 0;
    }

    x = 1; matrix_set(&a, 0, 0, &x);
    x = 2; matrix_set(&a, 0, 1, &x);
    x = 3; matrix_set(&a, 1, 0, &x);
    x = 4; matrix_set(&a, 1, 1, &x);

    x = 5; matrix_set(&b, 0, 0, &x);
    x = 6; matrix_set(&b, 0, 1, &x);
    x = 7; matrix_set(&b, 1, 0, &x);
    x = 8; matrix_set(&b, 1, 1, &x);

    if (!matrix_mul(&a, &b, &result)) {
        ok = 0;
    } 
    else {
        if (*(int*)matrix_at(&result, 0, 0) != 19) ok = 0;
        if (*(int*)matrix_at(&result, 0, 1) != 22) ok = 0;
        if (*(int*)matrix_at(&result, 1, 0) != 43) ok = 0;
        if (*(int*)matrix_at(&result, 1, 1) != 50) ok = 0;
    }

    matrix_destroy_content(&a);
    matrix_destroy_content(&b);
    matrix_destroy_content(&result);

    return ok;
}

static int test_matrix_scalar_mul_int(void) {
    Matrix a;
    Matrix result;
    int x;
    int scalar = 3;
    int ok = 1;

    if (!matrix_init(&a, 2, get_int_field_info())) return 0;
    if (!matrix_init(&result, 2, get_int_field_info())) {
        matrix_destroy_content(&a);
        return 0;
    }

    x = 1; matrix_set(&a, 0, 0, &x);
    x = 2; matrix_set(&a, 0, 1, &x);
    x = 3; matrix_set(&a, 1, 0, &x);
    x = 4; matrix_set(&a, 1, 1, &x);

    if (!matrix_scalar_mul(&a, &scalar, get_int_field_info(), &result)) {
        ok = 0;
    } 
    else {
        if (*(int*)matrix_at(&result, 0, 0) != 3) ok = 0;
        if (*(int*)matrix_at(&result, 0, 1) != 6) ok = 0;
        if (*(int*)matrix_at(&result, 1, 0) != 9) ok = 0;
        if (*(int*)matrix_at(&result, 1, 1) != 12) ok = 0;
    }

    matrix_destroy_content(&a);
    matrix_destroy_content(&result);

    return ok;
}

static int test_matrix_add_complex(void) {
    Matrix a;
    Matrix b;
    Matrix result;
    Complex z;
    Complex* r;
    int ok = 1;

    if (!matrix_init(&a, 1, get_complex_field_info())) return 0;
    if (!matrix_init(&b, 1, get_complex_field_info())) {
        matrix_destroy_content(&a);
        return 0;
    }
    if (!matrix_init(&result, 1, get_complex_field_info())) {
        matrix_destroy_content(&a);
        matrix_destroy_content(&b);
        return 0;
    }

    z = complex_create(1, 2);
    matrix_set(&a, 0, 0, &z);

    z = complex_create(3, 4);
    matrix_set(&b, 0, 0, &z);

    if (!matrix_add(&a, &b, &result)) {
        ok = 0;
    } 
    else {
        r = (Complex*)matrix_at(&result, 0, 0);
        if (r == NULL) ok = 0;
        else if (r->re != 4 || r->im != 6) ok = 0;
    }

    matrix_destroy_content(&a);
    matrix_destroy_content(&b);
    matrix_destroy_content(&result);

    return ok;
}

static int test_matrix_mul_complex(void) {
    Matrix a;
    Matrix b;
    Matrix result;
    Complex z;
    Complex* r;
    int ok = 1;

    if (!matrix_init(&a, 1, get_complex_field_info())) return 0;
    if (!matrix_init(&b, 1, get_complex_field_info())) {
        matrix_destroy_content(&a);
        return 0;
    }
    if (!matrix_init(&result, 1, get_complex_field_info())) {
        matrix_destroy_content(&a);
        matrix_destroy_content(&b);
        return 0;
    }

    z = complex_create(1, 2);
    matrix_set(&a, 0, 0, &z);

    z = complex_create(3, 4);
    matrix_set(&b, 0, 0, &z);

    if (!matrix_mul(&a, &b, &result)) {
        ok = 0;
    } 
    else {
        r = (Complex*)matrix_at(&result, 0, 0);
        if (r == NULL) ok = 0;
        else if (r->re != -5 || r->im != 10) ok = 0;
    }

    matrix_destroy_content(&a);
    matrix_destroy_content(&b);
    matrix_destroy_content(&result);

    return ok;
}

static int test_matrix_scalar_wrong_type(void) {
    Matrix a;
    Matrix result;
    Complex scalar;
    int ok;

    if (!matrix_init(&a, 2, get_int_field_info())) return 0;
    if (!matrix_init(&result, 2, get_int_field_info())) {
        matrix_destroy_content(&a);
        return 0;
    }

    scalar = complex_create(1, 1);
    ok = matrix_scalar_mul(&a, &scalar, get_complex_field_info(), &result);

    matrix_destroy_content(&a);
    matrix_destroy_content(&result);

    return ok == 0;
}

static int test_algebra_rule_mul(void) {
    Matrix a;
    Matrix b;
    Matrix result;
    int x;
    int ok = 1;

    if (!matrix_init(&a, 1, get_int_field_info())) return 0;
    if (!matrix_init(&b, 1, get_int_field_info())) {
        matrix_destroy_content(&a);
        return 0;
    }
    if (!matrix_init(&result, 1, get_int_field_info())) {
        matrix_destroy_content(&a);
        matrix_destroy_content(&b);
        return 0;
    }

    x = 2;
    matrix_set(&a, 0, 0, &x);

    x = 3;
    matrix_set(&b, 0, 0, &x);

    if (!matrix_mul(&a, &b, &result)) {
        ok = 0;
    } 
    else {
        if (*(int*)matrix_at(&result, 0, 0) != 6) ok = 0;
    }

    matrix_destroy_content(&a);
    matrix_destroy_content(&b);
    matrix_destroy_content(&result);

    return ok;
}

static int test_matrix_add_different_sizes(void) {
    Matrix a;
    Matrix b;
    Matrix result;
    int ok;

    if (!matrix_init(&a, 2, get_int_field_info())) return 0;
    if (!matrix_init(&b, 3, get_int_field_info())) {
        matrix_destroy_content(&a);
        return 0;
    }
    if (!matrix_init(&result, 2, get_int_field_info())) {
        matrix_destroy_content(&a);
        matrix_destroy_content(&b);
        return 0;
    }

    ok = matrix_add(&a, &b, &result);

    matrix_destroy_content(&a);
    matrix_destroy_content(&b);
    matrix_destroy_content(&result);

    return ok == 0;
}

static int test_matrix_add_different_types(void) {
    Matrix a;
    Matrix b;
    Matrix result;
    int ok;

    if (!matrix_init(&a, 2, get_int_field_info())) return 0;
    if (!matrix_init(&b, 2, get_complex_field_info())) {
        matrix_destroy_content(&a);
        return 0;
    }
    if (!matrix_init(&result, 2, get_int_field_info())) {
        matrix_destroy_content(&a);
        matrix_destroy_content(&b);
        return 0;
    }

    ok = matrix_add(&a, &b, &result);

    matrix_destroy_content(&a);
    matrix_destroy_content(&b);
    matrix_destroy_content(&result);

    return ok == 0;
}

static int test_matrix_add_null(void) {
    Matrix a;
    Matrix result;
    int ok;

    if (!matrix_init(&a, 2, get_int_field_info())) return 0;
    if (!matrix_init(&result, 2, get_int_field_info())) {
        matrix_destroy_content(&a);
        return 0;
    }

    ok = matrix_add(NULL, &a, &result);

    matrix_destroy_content(&a);
    matrix_destroy_content(&result);

    return ok == 0;
}

void run_all_tests(void) {
    print_result("matrix_init_invalid_size", test_matrix_init_invalid_size());

    print_result("matrix_add_int", test_matrix_add_int());
    print_result("matrix_mul_int", test_matrix_mul_int());
    print_result("matrix_scalar_mul_int", test_matrix_scalar_mul_int());

    print_result("matrix_add_complex", test_matrix_add_complex());
    print_result("matrix_mul_complex", test_matrix_mul_complex());

    print_result("matrix_scalar_wrong_type", test_matrix_scalar_wrong_type());
    print_result("matrix_add_different_sizes", test_matrix_add_different_sizes());
    print_result("matrix_add_different_types", test_matrix_add_different_types());
    print_result("matrix_add_null", test_matrix_add_null());

    print_result("test_algebra_rule_mul", test_algebra_rule_mul());

    printf("\nТестов пройдено: %d\n", passed);
    printf("Тестов провалено: %d\n", failed);
}