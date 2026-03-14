#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "field_info.h"
#include "complex.h"
#include "tests.h"

static void clear_input_buffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
}

static int read_int(const char* prompt, int* value) {
    printf("%s", prompt);
    if (scanf("%d", value) != 1) {
        clear_input_buffer();
        return 0;
    }
    return 1;
}

static int read_size(const char* prompt, int* value) {
    printf("%s", prompt);
    if (scanf("%d", value) != 1) {
        clear_input_buffer();
        return 0;
    }
    if (*value <= 0) {
        return 0;
    }
    return 1;
}

static void print_matrix(const Matrix* matrix) {
    int i;
    int j;

    if (matrix == NULL || matrix->data == NULL || matrix->field_info == NULL) {
        printf("Матрица не создана\n");
        return;
    }

    for (i = 0; i < matrix->size; i++) {
        for (j = 0; j < matrix->size; j++) {
            matrix->field_info->print(matrix_at_const(matrix, i, j));
            printf("\t");
        }
        printf("\n");
    }
}

static int input_matrix(Matrix* matrix) {
    int i;
    int j;

    if (matrix->field_info == get_int_field_info()) {
        int value;

        for (i = 0; i < matrix->size; i++) {
            for (j = 0; j < matrix->size; j++) {
                printf("Введите элемент [%d][%d]: ", i, j);

                if (scanf("%d", &value) != 1) {
                    clear_input_buffer();
                    return 0;
                }

                if (!matrix_set(matrix, i, j, &value)) {
                    return 0;
                }
            }
        }
    }
    else if (matrix->field_info == get_complex_field_info()) {
        Complex value;

        for (i = 0; i < matrix->size; i++) {
            for (j = 0; j < matrix->size; j++) {
                printf("Введите элемент [%d][%d] (re im): ", i, j);

                if (scanf("%lf %lf", &value.re, &value.im) != 2) {
                    clear_input_buffer();
                    return 0;
                }

                if (!matrix_set(matrix, i, j, &value)) {
                    return 0;
                }
            }
        }
    }
    else {
        return 0;
    }
    return 1;
}

int main(void) {

    run_all_tests();

    Matrix A;
    Matrix B;
    Matrix Res;

    int has_A = 0;
    int has_B = 0;
    int has_Res = 0;

    int command;
    int off_prog = 0;

    printf("\n=== Полиморфная квадратная матрица (вариант 21) ===\n");
    printf("1. Создать матрицу A\n");
    printf("2. Создать матрицу B\n");
    printf("3. Сложить матрицы (A + B)\n");
    printf("4. Умножить матрицы (A * B)\n");
    printf("5. Умножить матрицу A на скаляр\n");
    printf("6. Показать матрицу\n");
    printf("0. Выход\n");

    while (!off_prog) {

        if (!read_int("\nВведите команду: ", &command)) {
            printf("Ошибка ввода команды\n");
            continue;
        }

        switch (command) {

            case 1:
            case 2: {

                int n;
                int type_number;
                const FieldInfo* info;
                Matrix* target;
                int* target_flag;

                if (!read_int("Введите тип (1=int, 2=complex): ", &type_number)) {
                    printf("Ошибка ввода типа\n");
                    break;
                }

                if (type_number != 1 && type_number != 2) {
                    printf("Такого типа нет\n");
                    break;
                }

                if (!read_size("Введите размер N: ", &n)) {
                    printf("Размер должен быть положительным\n");
                    break;
                }

                info = (type_number == 1)
                       ? get_int_field_info()
                       : get_complex_field_info();

                if (command == 1) {
                    target = &A;
                    target_flag = &has_A;
                } else {
                    target = &B;
                    target_flag = &has_B;
                }

                if (*target_flag) {
                    matrix_destroy_content(target);
                    *target_flag = 0;
                }

                if (!matrix_init(target, n, info)) {
                    printf("Не удалось создать матрицу\n");
                    break;
                }

                if (!input_matrix(target)) {
                    printf("Ошибка ввода элементов\n");
                    matrix_destroy_content(target);
                    *target_flag = 0;
                    break;
                }

                *target_flag = 1;

                printf("Матрица %c создана\n",
                       (command == 1) ? 'A' : 'B');

                break;
            }

            case 3: {

                if (!has_A || !has_B) {
                    printf("Сначала создайте A и B\n");
                    break;
                }

                if (A.size != B.size || A.field_info != B.field_info) {
                    printf("Матрицы несовместимы\n");
                    break;
                }

                if (has_Res) {
                    matrix_destroy_content(&Res);
                    has_Res = 0;
                }

                if (!matrix_init(&Res, A.size, A.field_info)) {
                    printf("Не удалось создать результат\n");
                    break;
                }

                if (!matrix_add(&A, &B, &Res)) {
                    printf("Ошибка сложения\n");
                    matrix_destroy_content(&Res);
                    break;
                }

                has_Res = 1;

                printf("Сложение выполнено\n");

                break;
            }

            case 4: {

                if (!has_A || !has_B) {
                    printf("Сначала создайте A и B\n");
                    break;
                }

                if (A.size != B.size || A.field_info != B.field_info) {
                    printf("Матрицы несовместимы\n");
                    break;
                }

                if (has_Res) {
                    matrix_destroy_content(&Res);
                    has_Res = 0;
                }

                if (!matrix_init(&Res, A.size, A.field_info)) {
                    printf("Не удалось создать результат\n");
                    break;
                }

                if (!matrix_mul(&A, &B, &Res)) {
                    printf("Ошибка умножения\n");
                    matrix_destroy_content(&Res);
                    break;
                }

                has_Res = 1;

                printf("Умножение выполнено\n");

                break;
            }

            case 5: {

                if (!has_A) {
                    printf("Сначала создайте A\n");
                    break;
                }

                if (has_Res) {
                    matrix_destroy_content(&Res);
                    has_Res = 0;
                }

                if (!matrix_init(&Res, A.size, A.field_info)) {
                    printf("Не удалось создать результат\n");
                    break;
                }

                if (A.field_info == get_int_field_info()) {

                    int scalar;

                    if (!read_int("Введите скаляр: ", &scalar)) {
                        printf("Ошибка ввода\n");
                        matrix_destroy_content(&Res);
                        break;
                    }

                    if (!matrix_scalar_mul(&A, &scalar,
                        get_int_field_info(), &Res)) {

                        printf("Ошибка умножения\n");
                        matrix_destroy_content(&Res);
                        break;
                    }
                }
                else {

                    Complex scalar;

                    printf("Введите скаляр (re im): ");

                    if (scanf("%lf %lf",
                              &scalar.re,
                              &scalar.im) != 2) {

                        clear_input_buffer();
                        printf("Ошибка ввода\n");
                        matrix_destroy_content(&Res);
                        break;
                    }

                    if (!matrix_scalar_mul(&A, &scalar,
                        get_complex_field_info(), &Res)) {

                        printf("Ошибка умножения\n");
                        matrix_destroy_content(&Res);
                        break;
                    }
                }

                has_Res = 1;

                printf("Умножение на скаляр выполнено\n");

                break;
            }

            case 6: {

                int show_which;
                const Matrix* matrix_to_show = NULL;

                if (!read_int("Показать (1=A,2=B,3=Result): ",
                              &show_which)) {
                    printf("Ошибка ввода\n");
                    break;
                }

                if (show_which == 1 && has_A)
                    matrix_to_show = &A;

                else if (show_which == 2 && has_B)
                    matrix_to_show = &B;

                else if (show_which == 3 && has_Res)
                    matrix_to_show = &Res;

                if (matrix_to_show)
                    print_matrix(matrix_to_show);
                else
                    printf("Матрица не существует\n");

                break;
            }

            case 0:
                off_prog = 1;
                break;

            default:
                printf("Такой команды нет\n");
        }
    }

    if (has_A) matrix_destroy_content(&A);
    if (has_B) matrix_destroy_content(&B);
    if (has_Res) matrix_destroy_content(&Res);

    return 0;
}