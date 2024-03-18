/**
 * @file s21_another.c
 * @author yuetteyo (yuetteyo@student.21-school.ru)
 * @brief
 * @version 0.1
 * @date 2024-02-19
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int scale = s21_getScale(value);
  s21_setScale(&value, 0);
  for (int i = 0; i < scale; i++) {
    s21_div10((unsigned *)value.bits, 3);
  }
  if (result) {
    s21_copyArray((unsigned *)value.bits, (unsigned *)result->bits, 4);
  }

  return 0;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  if (s21_getScale(value)) {
    if (s21_getSign(value) && !s21_is_zero((unsigned *)value.bits, 3)) {
      s21_decimal one = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}};
      s21_sub(value, one, &value);
      s21_truncate(value, result);
    } else {
      // if(s21_is_zero((unsigned *)value.bits, 4)) s21_setSign(&value, 0);
      s21_truncate(value, result);
    }
  } else {
    s21_copyArray((unsigned *)value.bits, (unsigned *)result->bits, 4);
  }
  return 0;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  s21_decimal half_one = {{0x00000005, 0x00000000, 0x00000000, 0x00010000}};
  s21_decimal minus_half_one = {
      {0x00000005, 0x00000000, 0x00000000, 0x80010000}};
  if (s21_getScale(value)) {
    if (s21_is_less(value, half_one) && s21_is_greater(value, minus_half_one)) {
      if (result) s21_zeroArray((unsigned *)result->bits, 4);
    } else {
      if (s21_getSign(value) && !s21_is_zero((unsigned *)value.bits, 3)) {
        s21_sub(value, half_one, &value);
        s21_truncate(value, result);
      } else {
        s21_add(value, half_one, &value);
        s21_truncate(value, result);
      }
    }
  } else {
    s21_copyArray((unsigned *)value.bits, (unsigned *)result->bits, 4);
  }
  return 0;
}