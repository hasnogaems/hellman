/**
 * @file s21_sign.c
 * @author yuetteyo (yuetteyo@student.21-school.ru)
 * @brief
 * @version 0.1
 * @date 2024-02-18
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <stdio.h>

#include "s21_decimal.h"

int s21_getSign(s21_decimal value) {
  int mask = 1;
  int sign = (value.bits[3] >> 31) & mask;
  return sign;
}

void s21_setSign(s21_decimal *value, int sign) {
  if (value) {
    value->bits[3] = value->bits[3] & 0xff0000;
    value->bits[3] = value->bits[3] | (sign << 31);
  }
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  int ret = 0;
  if (result) {
    s21_copyArray((unsigned *)value.bits, (unsigned *)result->bits, 4);
    s21_setSign(result, !s21_getSign(*result));
  } else {
    ret = 1;
  }
  return ret;
}
