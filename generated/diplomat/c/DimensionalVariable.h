#ifndef DimensionalVariable_H
#define DimensionalVariable_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "diplomat_runtime.h"

#include "BaseUnits.d.h"

#include "DimensionalVariable.d.h"






typedef struct DimensionalVariable_new_result {union {DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_new_result;
DimensionalVariable_new_result DimensionalVariable_new(double value, DiplomatStringView unit);

typedef struct DimensionalVariable_asin_scalar_result {union {DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_asin_scalar_result;
DimensionalVariable_asin_scalar_result DimensionalVariable_asin_scalar(double x);

typedef struct DimensionalVariable_acos_scalar_result {union {DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_acos_scalar_result;
DimensionalVariable_acos_scalar_result DimensionalVariable_acos_scalar(double x);

typedef struct DimensionalVariable_atan_scalar_result {union {DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_atan_scalar_result;
DimensionalVariable_atan_scalar_result DimensionalVariable_atan_scalar(double x);

size_t DimensionalVariable_base_units_size(void);

double DimensionalVariable_value(const DimensionalVariable* self);

typedef struct DimensionalVariable_value_in_result {union {double ok; }; bool is_ok;} DimensionalVariable_value_in_result;
DimensionalVariable_value_in_result DimensionalVariable_value_in(const DimensionalVariable* self, DiplomatStringView unit);

bool DimensionalVariable_is_unitless(const DimensionalVariable* self);

BaseUnits DimensionalVariable_base_units(const DimensionalVariable* self);

typedef struct DimensionalVariable_add_result {union {DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_add_result;
DimensionalVariable_add_result DimensionalVariable_add(const DimensionalVariable* self, const DimensionalVariable* other);

typedef struct DimensionalVariable_sub_result {union {DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_sub_result;
DimensionalVariable_sub_result DimensionalVariable_sub(const DimensionalVariable* self, const DimensionalVariable* other);

DimensionalVariable* DimensionalVariable_mul(const DimensionalVariable* self, const DimensionalVariable* other);

DimensionalVariable* DimensionalVariable_div(const DimensionalVariable* self, const DimensionalVariable* other);

DimensionalVariable* DimensionalVariable_mul_scalar(const DimensionalVariable* self, double scalar);

DimensionalVariable* DimensionalVariable_div_scalar(const DimensionalVariable* self, double scalar);

DimensionalVariable* DimensionalVariable_rdiv_scalar(const DimensionalVariable* self, double scalar);

DimensionalVariable* DimensionalVariable_powi(const DimensionalVariable* self, int32_t exp);

typedef struct DimensionalVariable_powf_result {union {DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_powf_result;
DimensionalVariable_powf_result DimensionalVariable_powf(const DimensionalVariable* self, double exp);

typedef struct DimensionalVariable_sqrt_result {union {DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_sqrt_result;
DimensionalVariable_sqrt_result DimensionalVariable_sqrt(const DimensionalVariable* self);

typedef struct DimensionalVariable_ln_result {union {DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_ln_result;
DimensionalVariable_ln_result DimensionalVariable_ln(const DimensionalVariable* self);

typedef struct DimensionalVariable_log2_result {union {DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_log2_result;
DimensionalVariable_log2_result DimensionalVariable_log2(const DimensionalVariable* self);

typedef struct DimensionalVariable_log10_result {union {DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_log10_result;
DimensionalVariable_log10_result DimensionalVariable_log10(const DimensionalVariable* self);

typedef struct DimensionalVariable_sin_result {union {DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_sin_result;
DimensionalVariable_sin_result DimensionalVariable_sin(const DimensionalVariable* self);

typedef struct DimensionalVariable_cos_result {union {DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_cos_result;
DimensionalVariable_cos_result DimensionalVariable_cos(const DimensionalVariable* self);

typedef struct DimensionalVariable_tan_result {union {DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_tan_result;
DimensionalVariable_tan_result DimensionalVariable_tan(const DimensionalVariable* self);

typedef struct DimensionalVariable_asin_result {union {DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_asin_result;
DimensionalVariable_asin_result DimensionalVariable_asin(const DimensionalVariable* self);

typedef struct DimensionalVariable_acos_result {union {DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_acos_result;
DimensionalVariable_acos_result DimensionalVariable_acos(const DimensionalVariable* self);

typedef struct DimensionalVariable_atan_result {union {DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_atan_result;
DimensionalVariable_atan_result DimensionalVariable_atan(const DimensionalVariable* self);

DimensionalVariable* DimensionalVariable_neg(const DimensionalVariable* self);

DimensionalVariable* DimensionalVariable_abs(const DimensionalVariable* self);

bool DimensionalVariable_equals(const DimensionalVariable* self, const DimensionalVariable* other);

bool DimensionalVariable_not_equals(const DimensionalVariable* self, const DimensionalVariable* other);

typedef struct DimensionalVariable_less_than_result {union {bool ok; }; bool is_ok;} DimensionalVariable_less_than_result;
DimensionalVariable_less_than_result DimensionalVariable_less_than(const DimensionalVariable* self, const DimensionalVariable* other);

typedef struct DimensionalVariable_less_equal_result {union {bool ok; }; bool is_ok;} DimensionalVariable_less_equal_result;
DimensionalVariable_less_equal_result DimensionalVariable_less_equal(const DimensionalVariable* self, const DimensionalVariable* other);

typedef struct DimensionalVariable_greater_than_result {union {bool ok; }; bool is_ok;} DimensionalVariable_greater_than_result;
DimensionalVariable_greater_than_result DimensionalVariable_greater_than(const DimensionalVariable* self, const DimensionalVariable* other);

typedef struct DimensionalVariable_greater_equal_result {union {bool ok; }; bool is_ok;} DimensionalVariable_greater_equal_result;
DimensionalVariable_greater_equal_result DimensionalVariable_greater_equal(const DimensionalVariable* self, const DimensionalVariable* other);

DimensionalVariable* DimensionalVariable_clone_var(const DimensionalVariable* self);

typedef struct DimensionalVariable_to_string_result { bool is_ok;} DimensionalVariable_to_string_result;
DimensionalVariable_to_string_result DimensionalVariable_to_string(const DimensionalVariable* self, DiplomatWrite* write);

void DimensionalVariable_destroy(DimensionalVariable* self);





#endif // DimensionalVariable_H
