#ifndef S21_HELPER_H
#define S21_HELPER_H
#include <math.h>
#include <string.h>

#include "s21_decimal.h"

typedef struct {
  unsigned int bits[12];
} s21_decimal28;

#define MAX_DECIMAL 79228162514264337593543950335.L

/**
 * @brief get sign of struct s21_decimal
 *
 * @param value variable which sign needs to be known
 * @retval 0 - positive
 * @retval 1 - negative
 */
int s21_getSign(s21_decimal value);
/**
 * @brief set sign in struct s21_decimal
 *
 * @param value struct in which nedd to set sign
 * @param sign 0-positive, 1-negative
 */
void s21_setSign(s21_decimal *value, int sign);

/**
 * @brief get scale factor of struct s21_decimal
 *
 * @param value variable which scale factor needs to be known
 * @return int scale factor
 */
int s21_getScale(s21_decimal value);
/**
 * @brief set scale in struct s21_decimal
 *
 * @param value struct in which need to set scale factor
 * @param scale scale factor
 */
void s21_setScale(s21_decimal *value, int scale);

/**
 * @brief convertation s21_decimal to s21_decimal28
 *
 * @param value struct need to convertate
 * @param value28 struct for print result
 */
void s21_toDecimal28(s21_decimal value, s21_decimal28 *value28);
/**
 * @brief convertation s21_decimal28 to s21_decimal
 *
 * @param value struct need to convertate
 * @param value28 struct for print result
 */
int s21_toDecimal(s21_decimal28 value28, s21_decimal *value);

/**
 * @brief round value by bank
 *
 * @param value28 value need to round
 * @param mod the last discarded digit
 * @param newScale scale factor
 * @retval 0 - OK
 * @retval 1 - calculation error
 */
int s21_bankRound(s21_decimal28 *value28, int mod, int *newScale);

/**
 * @brief copy count-elements from array1 to array2
 *
 * @param array1 source need to copy
 * @param array2 destination for coping
 * @param count count of elements
 */
void s21_copyArray(const unsigned *array1, unsigned *array2, int count);
/**
 * @brief zero array
 *
 * @param array array need to zero
 * @param count count of element need to zero
 */
void s21_zeroArray(unsigned *array, int count);
/**
 * @brief check all elemts of array is 0
 *
 * @param arr checking array
 * @param count count of elements array
 * @retval 1 - true
 * @retval 0 - false
 */
int s21_is_zero(const unsigned *arr, int count);

/**
 * @brief adding each elements by element two arrays
 *
 * @param value_1 first array
 * @param value_2 second array
 * @param result result of sum
 */
void s21_add28(s21_decimal28 value_1, s21_decimal28 value_2,
               s21_decimal28 *result);
/**
 * @brief difference between arrays
 *
 * @param value_1 first array
 * @param value_2 second array
 * @param result result of difference
 */
void s21_sub28(s21_decimal28 value_1, s21_decimal28 value_2,
               s21_decimal28 *result);
/**
 * @brief multiply two decimal values
 *
 * @param value_1 first array
 * @param value_2 second array
 * @param result result of multiply
 */
void s21_mul28(s21_decimal28 value_1, s21_decimal28 value_2,
               s21_decimal28 *result);
/**
 * @brief divide two decimal values
 *
 * @param value_1 first array
 * @param value_2 second array
 * @param result result of divide
 */
void s21_div28(s21_decimal28 value_1, s21_decimal28 value_2,
               s21_decimal28 *result);
/**
 * @brief average two decimal values
 *
 * @param value_1 first array
 * @param value_2 second array
 * @param result result of average
 */
void s21_middle28(s21_decimal28 value_1, s21_decimal28 value_2,
                  s21_decimal28 *result);

/**
 * @brief multiple by 10
 *
 * @param arr multiple
 * @param count count of elements in array
 */
void s21_mul10(unsigned *arr, int count);
/**
 * @brief divide by 10
 *
 * @param arr divident
 * @param count count of elements in array
 */
void s21_div10(unsigned *arr, int count);
/**
 * @brief found mod from devision by 10
 *
 * @param arr array need to devide
 * @param count count of elements in array
 * @return int mod from devision
 */
int s21_mod10(const unsigned *arr, int count);
/**
 * @brief compare two variables
 *
 * @param value variables for comparison
 * @retval  1 - arr_1 is greater than arr_2
 * @retval  0 - arr_1 is equal   than arr_2
 * @retval -1 - arr_1 is less    than arr_2
 */
int s21_bit_compare(const unsigned *arr_1, const unsigned *arr_2, int count);
void nullify(s21_decimal *d);

#endif