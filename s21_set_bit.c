#include "s21_decimal.h"

void s21_set_bit(s21_decimal* num, int index, int value) {
    if (value == 1) {
    num->bits[index / 32] = num->bits[index / 32] | (1 << index % 32); // (1 << bit % 32) создает число, у которого только один бит равен 1 на позиции bit % 32
  } else if (value == 0) {
    num->bits[index / 32] = num->bits[index / 32] & ~(1 << index % 32); // инвертированная маска
  }
}