#ifndef DV_C_H
#define DV_C_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef struct dv_var dv_var; // opaque

// Error handling: call after a function returns failure to get a message (UTF-8). The pointer is valid until next API call.
const char* dv_last_error_message(void);

// Util
size_t dv_base_units_size(void);
void   dv_free_cstring(char* s);  // Free a string returned by dv_var_to_string

// Lifecycle
dv_var* dv_var_new(double value, const char* unit_str);
void    dv_var_free(dv_var* v);

// Accessors
double  dv_var_value(const dv_var* v);
int     dv_var_is_unitless(const dv_var* v);
int     dv_var_value_in(const dv_var* v, const char* unit_str, double* out_value);
char*   dv_var_to_string(const dv_var* v); // Returns allocated string, must free with dv_free_cstring

// Arithmetic
dv_var* dv_var_add(const dv_var* a, const dv_var* b);
dv_var* dv_var_sub(const dv_var* a, const dv_var* b);
dv_var* dv_var_mul(const dv_var* a, const dv_var* b);
dv_var* dv_var_div(const dv_var* a, const dv_var* b);

// Scalar ops
dv_var* dv_var_mul_scalar(const dv_var* a, double s);
dv_var* dv_var_div_scalar(const dv_var* a, double s);

// Powers
dv_var* dv_var_powi(const dv_var* a, int exp);
dv_var* dv_var_powf(const dv_var* a, double exp);
dv_var* dv_var_sqrt(const dv_var* a);
dv_var* dv_var_ln(const dv_var* a);
dv_var* dv_var_log2(const dv_var* a);
dv_var* dv_var_log10(const dv_var* a);
dv_var* dv_var_sin(const dv_var* a);
dv_var* dv_var_cos(const dv_var* a);
dv_var* dv_var_tan(const dv_var* a);
dv_var* dv_var_neg(const dv_var* a);
dv_var* dv_var_abs(const dv_var* a);

// Inverse trigonometric functions (return angle in radians)
dv_var* dv_var_asin(const dv_var* a);
dv_var* dv_var_acos(const dv_var* a);
dv_var* dv_var_atan(const dv_var* a);

// Free-standing inverse trigonometric functions (take raw double, return angle in radians)
dv_var* dv_asin(double x);
dv_var* dv_acos(double x);
dv_var* dv_atan(double x);

#ifdef __cplusplus
}
#endif

#endif // DV_C_H
