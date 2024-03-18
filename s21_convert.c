/**
 * @file s21_convert.c
 * @author amidalah (amidalah@student.21-school.ru)
 * @brief
 * @version 0.1
 * @date 2024-02-18
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int ret = 0;
  if (dst) {
    s21_zeroArray((unsigned *)dst->bits, 4);
    s21_setSign(dst, src < 0);
    if (src > 0 || src == (int)0x80000000)
      dst->bits[0] = src;
    else
      dst->bits[0] = -src;
  } else
    ret = 1;
  return ret;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int ret = 0;
  s21_truncate(src, &src);
  if (s21_is_zero((unsigned *)src.bits + 1, 2) &&
      ((unsigned)src.bits[0] < 0x80000000 ||
       ((unsigned)src.bits[0] == 0x80000000 && s21_getSign(src)))) {
    if (dst) {
      *dst = src.bits[0];
      if (s21_getSign(src)) *dst *= -1;
    }
  } else {
    if (dst) *dst = 0;
    ret = 1;
  }
  return ret;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int ret = 0;
  if (dst && fabs(src) >= 1e-28 && fabs(src) <= MAX_DECIMAL) {
    char str[100], *tmp;
    s21_decimal28 cipher, res;
    s21_zeroArray(cipher.bits, 12);
    s21_zeroArray(res.bits, 12);
    sprintf(str, "% .28f", src);
    // printf("%s\n", str);
    tmp = str + 1;
    while (*tmp) {
      if (*tmp != '.') {
        cipher.bits[0] = *tmp - '0';
        s21_mul10(res.bits, 12);
        s21_add28(res, cipher, &res);
      }
      tmp++;
    }
    s21_toDecimal(res, dst);
    s21_setSign(dst, src < 0);
  } else {
    if (dst) s21_zeroArray((unsigned *)dst->bits, 4);
    ret = 1;
  }
  return ret;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int ret = 0;
  if (dst) {
    char str[100];
    *str = '\0';
    int len = 0;
    while (!s21_is_zero((unsigned *)src.bits, 3) || len <= s21_getScale(src)) {
      if (len == s21_getScale(src)) {
        strcat(str, ".");
      }
      char cipher[2] = ".";
      sprintf(cipher, "%d", s21_mod10((unsigned *)src.bits, 3));
      strcat(str, cipher);
      s21_div10((unsigned *)src.bits, 3);
      len++;
    }
    if (s21_getSign(src)) {
      char cipher[2] = "-";
      strcat(str, cipher);
      len++;
    }
    for (int i = 0; i < len / 2; ++i) {
      char c = str[i];
      str[i] = str[len - i];
      str[len - i] = c;
    }
    sscanf(str, "%f", dst);
  } else {
    ret = 1;
  }
  return ret;
}

void s21_zeroArray(unsigned *array, int count) {
  for (int i = 0; i < count; ++i) {
    array[i] = 0;
  }
}

void s21_copyArray(const unsigned *array1, unsigned *array2, int count) {
  for (int i = 0; i < count; ++i) {
    array2[i] = array1[i];
  }
}

void s21_toDecimal28(s21_decimal value, s21_decimal28 *value28) {
  int degree = 28 - s21_getScale(value);
  s21_zeroArray(value28->bits, 12);
  s21_copyArray((unsigned *)value.bits, value28->bits, 3);

  for (int i = 0; i < degree; ++i) {
    s21_mul10(value28->bits, 12);
  }
}

int s21_toDecimal(s21_decimal28 value28, s21_decimal *value) {
  int newScale = 28, mod = 0, ret = 0;
  // обработка незначащих нулей
  while (!s21_mod10(value28.bits, 12) && newScale > 0) {
    s21_div10(value28.bits, 12);
    newScale--;
  }
  // отбрасывание дробной части, пока мантисса не помещается в int[3]
  while (!s21_is_zero(value28.bits + 3, 9) && newScale > 0) {
    mod = s21_mod10(value28.bits, 12);
    s21_div10(value28.bits, 12);
    newScale--;
  }
  // банковское округление, а также проверка того,
  // что мантисса помещается в int[3]

  ret = s21_bankRound(&value28, mod, &newScale);
  if (value) {
    if (!ret) {
      s21_copyArray(value28.bits, (unsigned *)value->bits, 3);
      s21_setScale(value, newScale);
    } else {
      s21_zeroArray((unsigned *)value, 4);
    }
  }
  return ret;
}

int s21_bankRound(s21_decimal28 *value28, int mod, int *newScale) {
  int ret = !s21_is_zero(value28->bits + 3, 9);
  if (!ret && (mod > 5 || (mod == 5 && (value28->bits[0] & 1)))) {
    // проверяем краевые значения (на переполнения)
    unsigned max_mantisa[] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
    if (!s21_bit_compare(value28->bits, max_mantisa, 3)) {
      if (*newScale > 0) {
        mod = s21_mod10(value28->bits, 12);
        s21_div10(value28->bits, 12);
        (*newScale)--;
      } else {
        ret = 1;
      }
    }
    s21_decimal28 one;
    s21_zeroArray(one.bits, 12);
    one.bits[0] = 1;
    s21_add28(*value28, one, value28);
  }
  return ret;
}
