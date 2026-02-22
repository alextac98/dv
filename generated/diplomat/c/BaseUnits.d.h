#ifndef BaseUnits_D_H
#define BaseUnits_D_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "diplomat_runtime.h"





typedef struct BaseUnits {
  double m;
  double kg;
  double s;
  double k;
  double a;
  double mol;
  double cd;
  double rad;
} BaseUnits;

typedef struct BaseUnits_option {union { BaseUnits ok; }; bool is_ok; } BaseUnits_option;



#endif // BaseUnits_D_H
