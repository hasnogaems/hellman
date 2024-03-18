/**
 * @file s21_scale.c
 * @author amidalah (amidalah@student.21-school.ru)
 * @brief
 * @version 0.1
 * @date 2024-02-18
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "s21_decimal.h"

void s21_setScale(s21_decimal *value, int scale) {
  if (value) {
    int mask = 0x80000000;
    value->bits[3] = value->bits[3] & mask;
    value->bits[3] = value->bits[3] | scale << 16;
  }
}

int s21_getScale(s21_decimal value) { return (value.bits[3] >> 16) & 0xFF; }