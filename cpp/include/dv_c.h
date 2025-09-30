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

// Lifecycle
dv_var* dv_var_new(double value, const char* unit_str);
void    dv_var_free(dv_var* v);

// Accessors
double  dv_var_value(const dv_var* v);
int     dv_var_is_unitless(const dv_var* v);
int     dv_var_value_in(const dv_var* v, const char* unit_str, double* out_value);

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

#ifdef __cplusplus
}
#endif

#endif // DV_C_H
