#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include <math.h>
#include <string.h>

double eps = 0.001;  // заданный эпсилон для оценки приближения корня в функции root
double eps2 = 0.00001;  // заданный эпсилон для приближенного вычисления интеграла
const int max_iter = 1000;  // максимальное количество итераций для оцен

// подынтегральные функции
double F1(double x) {
    return f1(x) - f3(x);
}

double F_1_5(double x) {
    return f2(x) - f3(x);
}

double F2(double x) {
    return f1(x) - f2(x);
}

// функция для подсчета интеграла в тестовом режиме с 2-мя функциями
double F(double (*f)(double), double (*g)(double), double x) {
    if (x == 0 || x == -1) return NAN;

    return f(x) - g(x);
}

// подбираем достаточно маленький интервал на котором будет точка пересечения двух функций
void find_interval_positive(double (*f)(double), double (*g)(double), double* a, double* b) {
    double x1 = 0.5, x2 = 1.0;

    while (F(f, g, x1) * F(f, g, x2) > 0) {
        x1 += 0.5;
        x2 += 0.5;
    }

    *a = x1;
    *b = x2;
}

void find_interval_negative(double (*f)(double), double (*g)(double), double* a, double* b) {
    double x1 = -1 - eps, x2 = -1 - 2 * eps;

    while (F(f, g, x1) * F(f, g, x2) > 0) {
        x1 -= eps;
        x2 -= eps;
    }

    *a = x1;
    *b = x2;
}

int check_functions_on_interval(double (*f)(double), double (*g)(double), double* a, double* b) {
    if (F(f, g, *a) * F(f, g, *b) > 0) {
        printf("There is no intersection point on the interval [%f, %f]\n", *a, *b);
        printf("Program will try to search it automaticly.\n");

        if (*b < 0) {
            find_interval_negative(f, g, a, b);
        }
        else {
            find_interval_positive(f, g, a, b);
        }

        return -1;
    }

    printf("Have an intercestion point on the interval [%f, %f]\n", *a, *b);

    return 1;
}

// функция для проверки интервалов
void check_interval(double a, double b) {
    if (a > b) {
        printf("Interval is wrong!!!! You need to enter numbers in proper order\n");

        exit(-1);
    }

    if (a > -1 + eps && a <= 0.2 || a <= -1 && b >= -1) {
        printf("Given interval include points 0 and -1 and their small areas where functions are not defined\n");
        printf("Change input interval\n");

        exit(-1);
    }

    printf("Interval is valid! Continue counting with interval from %f to %f\n", a, b);
}


// будем искать корень методом касательных(секущих)
double root(double f(double x), double g(double x), double* a, double* b, double eps) {
    check_interval(*a, *b);

    check_functions_on_interval(f, g, a, b);

    double xn_prev = *a, xn = *b;
    long max_iter = 1000, count_iterations = 0;

    while (count_iterations < max_iter) {
        double f_prev = F(f, g, xn_prev);
        double f_current = F(f, g, xn);

        if (isnan(f_prev) || isnan(f_current)) {
            printf("Error: x is out of scope!\n");
            return NAN;
        }

        if (fabs(f_current) < eps) break;

        if (fabs(f_current - f_prev) < 1e-10) {
            printf("Division by zero!\n");
            return NAN;
        }

        double xn_next = xn - f_current * (xn - xn_prev) / (f_current - f_prev);
        xn_prev = xn;
        xn = xn_next;
        count_iterations++;
    }

    if (count_iterations >= max_iter) {
        printf("It didn't add up in %ld iterations!\n", max_iter);
        return NAN;
    }

    printf("Found x = %.10f on the interval [%f, %f], iterations: %ld\n", xn, *a, *b, count_iterations);
    printf("\n");

    return xn;
}

// метод левых прямоугольников
double integral(double f(double x), double a, double b, double eps2) {
    check_interval(a, b);

    double s = 0.0;
    int i = 0;
    double step = eps2;

    while (a + step < b) {
        s += f(a + step);

        step += eps2;
    }

    return s * eps2;
}

double count_integral(double eps2) {
    double a, b;

    find_interval_positive(f1, f3, &a, &b);
    double first_intersection = root(f1, f3, &a, &b, eps);

    find_interval_positive(f2, f3, &a, &b);
    double second_intersection = root(f2, f3, &a, &b, eps);

    find_interval_positive(f1, f2, &a, &b);
    double third_intersection = root(f1, f2, &a, &b, eps);

    double first_area = integral(F1, first_intersection, second_intersection, eps2);
    double second_area = integral(F2, second_intersection, third_intersection, eps2);

    return first_area + second_area;
}


void test_root_func(int f_num1, int f_num2, double* a, double* b, double eps) {
    double intersection_point;

    if (f_num1 == 1 && f_num2 == 2 || f_num1 == 2 && f_num2 == 1) {
        intersection_point = root(f1, f2, a, b, eps);
    }
    else if (f_num1 == 2 && f_num2 == 3 || f_num1 == 3 && f_num2 == 2) {
        intersection_point = root(f2, f3, a, b, eps);
    }
    else if (f_num1 == 1 && f_num2 == 3 || f_num1 == 3 && f_num2 == 1) {
        intersection_point = root(f1, f3, a, b, eps);
    }

    printf("Intersection point on interval [%f, %f] is %f\n", *a, *b, intersection_point);
}

void test_integral_func(int f_num1, int f_num2, double* a, double* b, double eps2) {
    check_interval(*a, *b);

    double integral_result;

    if (f_num1 == 1 && f_num2 == 2 || f_num1 == 2 && f_num2 == 1) {
        integral_result = fabs(integral(F2, *a, *b, eps2));
    }
    else if (f_num1 == 2 && f_num2 == 3 || f_num1 == 3 && f_num2 == 2) {
        integral_result = fabs(integral(F_1_5, *a, *b, eps2));
    }
    else if (f_num1 == 1 && f_num2 == 3 || f_num1 == 3 && f_num2 == 1) {
        integral_result = fabs(integral(F1, *a, *b, eps2));
    }

    printf("Integral result on interval [%f, %f] is %f\n", *a, *b, integral_result);
}



int main(int argc, char* argv[]) {
    double a, b;  // start and end of the interval

    int test_mode = 0;
    int need_help = 0;
    int f_num1 = 0, f_num2 = 0;
    int test_root = 0;
    int test_integral = 0;


    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-help") == 0) {
            printf("You can use this folowing flags to work with program:\n");
            printf("-help -> give you info about flag and vars providing to this program\n");
            printf("-t    -> test mode, take this parameters ((r/i), f_num1, f_num2, inter_start, inter_end, eps)\n");

            exit(1);
        }

        if (strcmp(argv[i], "-t") == 0) {
            test_mode = 1;
            if (argc - i < 6) {
                printf("You need to provide at least 4 parameters: f1_num, f2_num, inter_start, inter_end\n");
                printf("Also will be cool if there will be eps\n");
                exit(-1);
            }

            if (strcmp(argv[i + 1], "r") == 0) {
                test_root = 1;
            }
            if (strcmp(argv[i + 1], "i") == 0) {
                test_integral = 1;
            }

            f_num1 = atoi(argv[i + 2]);
            f_num2 = atoi(argv[i + 3]);

            if(f_num1 > 3 || f_num1 < 1 || f_num2 > 3 || f_num2 < 1) {
                printf("Numbers of functions are not exists. Please enter correct numbers.\n");

                exit(-1);
            }

            a = atof(argv[i + 4]);
            b = atof(argv[i + 5]);

            if (test_integral) {
                eps2 = atof(argv[i + 6]);
            }

            if (test_root) {
                eps = atof(argv[i + 6]);
            }
        }
    }

    if (test_mode && test_root) {
        test_root_func(f_num1, f_num2, &a, &b, eps);
    }

    if (test_mode && test_integral) {
        test_integral_func(f_num1, f_num2, &a, &b, eps2);
    }


    if (argc == 1) {
        printf("Programm is running in standart mode.\n");
        printf("Will be print every intersection point, and interval will be searching automaticly.\n");
        printf("\n");

        double integral_value = count_integral(eps2);

        printf("Value of integral is %f\n", integral_value);
    }

    return 0;
}