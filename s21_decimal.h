#ifndef S21_DECIMAL
#define S21_DECIMAL
#include <math.h>
#include <stdio.h>

typedef struct {
  int bits[4];
} s21_decimal;

#include "s21_helper.h"

/**
 * @brief add decimal value1 to decimal value2 and print result to result
 *
 * @param value_1 term1
 * @param value_2 term2
 * @param result result of adding
 * @retval 0 - OK
 * @retval 1 - the number is too large or equal to infinity
 * @retval 2 - the number is too small or equal to negative infinity
 */
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
/**
 * @brief difference decimal value1 and decimal value2 and pront result to
 * result
 *
 * @param value_1 term1
 * @param value_2 term2
 * @param result result of difference
 * @retval 0 - OK
 * @retval 1 - the number is too large or equal to infinity
 * @retval 2 - the number is too small or equal to negative infinity
 */
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
/**
 * @brief multiply decimal value1 and decimal value2 and pront result to
 * result
 *
 * @param value_1 term1
 * @param value_2 term2
 * @param result result of multiply
 * @retval 0 - OK
 * @retval 1 - the number is too large or equal to infinity
 * @retval 2 - the number is too small or equal to negative infinity
 */
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
/**
 * @brief division decimal value1 and decimal value2 and pront result to
 * result
 *
 * @param value_1 term1
 * @param value_2 term2
 * @param result result of division
 * @retval 0 - OK
 * @retval 1 - the number is too large or equal to infinity
 * @retval 2 - the number is too small or equal to negative infinity
 * @retval 3 - division by 0
 */
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

/**
 * @brief determines first variable is less than second
 *
 * @param value variables for comparison
 * @return int 1-true 0-false
 */
int s21_is_less(s21_decimal, s21_decimal);
/**
 * @brief determines first variable is less than second or equal
 *
 * @param value variables for comparison
 * @return int 1-true 0-false
 */
int s21_is_less_or_equal(s21_decimal, s21_decimal);
/**
 * @brief determines first variable is greater than second
 *
 * @param value variables for comparison
 * @return int 1-true 0-false
 */
int s21_is_greater(s21_decimal, s21_decimal);
/**
 * @brief determines first variable is greater than second or equal
 *
 * @param value variables for comparison
 * @return int 1-true 0-false
 */
int s21_is_greater_or_equal(s21_decimal, s21_decimal);
/**
 * @brief determines first variable is equal second
 *
 * @param value variables for comparison
 * @return int 1-true 0-false
 */
int s21_is_equal(s21_decimal, s21_decimal);
/**
 * @brief determines first variable is not equal second
 *
 * @param value variables for comparison
 * @return int 1-true 0-false
 */
int s21_is_not_equal(s21_decimal, s21_decimal);

/**
 * @brief convert from int to decimal
 *
 * @param src int need to convert
 * @param dst decimal which need convert to
 * @retval 0 - OK
 * @retval 1 - convertation error
 */
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
/**
 * @brief convert from decimal to int
 *
 * @param src decimal need to convert
 * @param dst int which need convert to
 * @retval 0 - OK
 * @retval 1 - convertation error
 */
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
/**
 * @brief Rounds a specified Decimal number to the closest integer toward
 * negative infinity.
 *
 * @param value term
 * @param result the decimal where result is copied to
 * @retval 0 - OK
 * @retval 1 - convertation error
 */
int s21_floor(s21_decimal value, s21_decimal *result);

/**
 * @brief Rounds a decimal value to the nearest integer.
 *
 * @param value term
 * @param result the decimal where result is copied to
 * @return int 0 - OK, 1 - calculation error
 */
int s21_round(s21_decimal value, s21_decimal *result);
/**
 * @brief Returns the integral digits of the specified Decimal; any fractional
 * digits are discarded, including trailing zeroes.
 *
 * @param value term
 * @param result the decimal where result is copied to
 * @retval 0 - OK
 * @retval 1 - convertation error
 */
int s21_truncate(s21_decimal value, s21_decimal *result);
/**
 * @brief Returns the result of multiplying the specified Decimal value by
 * negative one.
 *
 * @param result the decimal where result is copied to
 * @retval 0 - OK
 * @retval 1 - convertation error
 */
int s21_negate(s21_decimal value, s21_decimal *result);
void printb(s21_decimal bits_array);
#endif
