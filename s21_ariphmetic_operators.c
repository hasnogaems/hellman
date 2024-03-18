#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int ret = 0;
  if (s21_getSign(value_1) == s21_getSign(value_2)) {
    s21_decimal28 arr1, arr2, res;

    s21_toDecimal28(value_1, &arr1);
    s21_toDecimal28(value_2, &arr2);
    s21_add28(arr1, arr2, &res);
    ret = s21_toDecimal(res, result);
    s21_setSign(result, s21_getSign(value_1));
    if (ret) ret += s21_getSign(value_1);
  } else {
    if (s21_getSign(value_2)) {
      s21_negate(value_2, &value_2);
      ret = s21_sub(value_1, value_2, result);
    } else {
      s21_negate(value_1, &value_1);
      ret = s21_sub(value_2, value_1, result);
    }
  }

  return ret;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int ret = 0;
  if (s21_getSign(value_1) == s21_getSign(value_2)) {
    s21_decimal28 arr1, arr2, res;
    s21_toDecimal28(value_1, &arr1);
    s21_toDecimal28(value_2, &arr2);
    int sign = s21_getSign(value_1);
    s21_setSign(&value_1, 0);
    s21_setSign(&value_2, 0);

    if (s21_is_greater(value_1, value_2)) {
      s21_sub28(arr1, arr2, &res);
    } else {
      s21_sub28(arr2, arr1, &res);
      sign = !sign;
    }
    ret = s21_toDecimal(res, result);
    if (ret)
      ret += s21_getSign(value_1);
    else
      s21_setSign(result, sign);
  } else {
    s21_negate(value_2, &value_2);
    ret = s21_add(value_1, value_2, result);
  }
  return ret;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int ret = 0;
  s21_decimal28 arr1, arr2, res;
  s21_toDecimal28(value_1, &arr1);
  s21_toDecimal28(value_2, &arr2);
  s21_mul28(arr1, arr2, &res);
  ret = s21_toDecimal(res, result);
  if (ret)
    ret += s21_getSign(value_1) != s21_getSign(value_2);
  else
    s21_setSign(result, s21_getSign(value_1) != s21_getSign(value_2));

  return ret;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int ret = s21_is_zero((unsigned *)value_2.bits, 3) ? 3 : 0;
  if (!ret) {
    s21_decimal max_num = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
    s21_decimal28 arr1, arr2, arr_max, arr_res;

    s21_toDecimal28(value_1, &arr1);
    s21_toDecimal28(value_2, &arr2);
    s21_toDecimal28(max_num, &arr_max);
    s21_mul28(arr2, arr_max, &arr_res);
    if (s21_bit_compare(arr_res.bits, arr1.bits, 12) == -1) {
      ret = 1 + (s21_getSign(value_1) != s21_getSign(value_2));
    } else {
      s21_div28(arr1, arr2, &arr_res);
      s21_toDecimal(arr_res, result);
      s21_setSign(result, s21_getSign(value_1) != s21_getSign(value_2));
    }
  }
  return ret;
}

void s21_add28(s21_decimal28 value_1, s21_decimal28 value_2,
               s21_decimal28 *result) {
  if (result) {
    s21_zeroArray(result->bits, 12);
    unsigned overflow = 0;

    for (int i = 0; i < 12; ++i) {
      unsigned long long int tmp = overflow;
      tmp += (unsigned)value_1.bits[i];
      tmp += (unsigned)value_2.bits[i];
      result->bits[i] = tmp & 0xFFFFFFFF;
      overflow = tmp >> 32;
    }
  }
}

void s21_sub28(s21_decimal28 value_1, s21_decimal28 value_2,
               s21_decimal28 *result) {
  if (result) {
    s21_zeroArray(result->bits, 12);
    unsigned overflow = 1;

    for (int i = 0; i < 12; ++i) {
      unsigned long long int tmp = overflow;
      tmp += (unsigned)value_1.bits[i];
      tmp += (unsigned)~value_2.bits[i];
      result->bits[i] = tmp & 0xFFFFFFFF;
      overflow = tmp >> 32;
    }
  }
}

void s21_mul28(s21_decimal28 value_1, s21_decimal28 value_2,
               s21_decimal28 *result) {
  s21_zeroArray(result->bits, 12);
  for (int i = 0; i < 6; ++i) {
    for (int k = 0; k < 6; ++k) {
      unsigned long long int m =
          (unsigned long long)value_1.bits[k] * value_2.bits[i] +
          result->bits[i + k];
      result->bits[i + k] = m & 0xFFFFFFFF;
      unsigned overflow = m >> 32;
      for (int p = i + k + 1; p < 12; p++) {
        unsigned long long int tmp = overflow;
        tmp += result->bits[p];
        result->bits[p] = tmp & 0xFFFFFFFF;
        overflow = tmp >> 32;
      }
    }
  }
  for (int d = 0; d < 28; ++d) {
    s21_div10(result->bits, 12);
  }
}

void s21_div28(s21_decimal28 value_1, s21_decimal28 value_2,
               s21_decimal28 *result) {
  s21_decimal min = {{0, 0, 0, 0}},
              max = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal28 left, left_pred, right, right_pred, middle, ans_tmp;

  s21_toDecimal28(min, &left);
  s21_toDecimal28(max, &right);

  s21_zeroArray(left_pred.bits, 12);
  s21_zeroArray(right_pred.bits, 12);

  while (s21_bit_compare(left_pred.bits, left.bits, 12) ||
         s21_bit_compare(right_pred.bits, right.bits, 12)) {
    s21_copyArray(left.bits, left_pred.bits, 12);
    s21_copyArray(right.bits, right_pred.bits, 12);
    s21_middle28(left, right, &middle);
    s21_mul28(middle, value_2, &ans_tmp);
    if (s21_bit_compare(ans_tmp.bits, value_1.bits, 12) == -1) {
      s21_copyArray(middle.bits, left.bits, 12);
    } else {
      s21_copyArray(middle.bits, right.bits, 12);
    }
  }

  s21_mul28(middle, value_2, &ans_tmp);
  if (s21_bit_compare(ans_tmp.bits, value_1.bits, 12))
    s21_copyArray(right.bits, result->bits, 12);
  else
    s21_copyArray(left.bits, result->bits, 12);
}

void s21_middle28(s21_decimal28 value_1, s21_decimal28 value_2,
                  s21_decimal28 *result) {
  s21_add28(value_1, value_2, result);
  unsigned mod = 0;
  for (int i = 11; i >= 0; --i) {
    unsigned tmp = mod;
    mod = result->bits[i] & 1;
    result->bits[i] = (result->bits[i] >> 1) | (tmp << 31);
  }
}

void s21_mul10(unsigned *arr, int count) {
  unsigned long long int overflow = 0;
  for (int k = 0; k < count; ++k) {
    unsigned long long int tmp = arr[k];
    tmp *= 10;
    tmp += overflow;
    arr[k] = tmp & 0xFFFFFFFF;
    overflow = tmp >> 32;
  }
}

void s21_div10(unsigned *arr, int count) {
  unsigned long long int mod = 0;
  for (int i = count - 1; i >= 0; i--) {
    // printf("%08X ", arr[i]);
    unsigned long long int tmp = mod << 32;
    tmp += arr[i];
    arr[i] = tmp / 10;
    mod = tmp % 10;
  }
  // printf("\n");
}

int s21_mod10(const unsigned *arr, int count) {
  unsigned arrTmp[12];
  s21_copyArray(arr, arrTmp, 12);
  unsigned long long int mod = 0;
  for (int i = count - 1; i >= 0; i--) {
    unsigned long long int tmp = mod << 32;
    tmp += arrTmp[i];
    arrTmp[i] = tmp;
    mod = tmp % 10;
  }
  // for (int i = count - 1; i >= 0; i--)
  //   printf("%08X ", arr[i]);
  // printf("\n");
  return mod;
}
