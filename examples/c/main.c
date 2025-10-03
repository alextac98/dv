#include "dv_c.h"
#include <stdio.h>

static void print_last_error(const char* prefix) {
    const char* msg = dv_last_error_message();
    if (msg && *msg) {
        fprintf(stderr, "%s: %s\n", prefix, msg);
    } else {
        fprintf(stderr, "%s: (unknown error)\n", prefix);
    }
}

int main(void) {
    dv_var* d = dv_var_new(42.0, "m");
    if (!d) { print_last_error("dv_var_new(d)"); return 1; }

    dv_var* t = dv_var_new(3.0, "s");
    if (!t) { print_last_error("dv_var_new(t)"); dv_var_free(d); return 1; }

    dv_var* v = dv_var_div(d, t); // 14 m/s
    if (!v) { print_last_error("dv_var_div(d,t)"); dv_var_free(d); dv_var_free(t); return 1; }

    double mph = 0.0;
    if (!dv_var_value_in(v, "mi/hr", &mph)) {
        print_last_error("dv_var_value_in(v, mi/hr)");
        dv_var_free(v); dv_var_free(d); dv_var_free(t);
        return 1;
    }

    printf("speed = %.4f mi/hr\n", mph);

    dv_var_free(v);
    dv_var_free(d);
    dv_var_free(t);
    return 0;
}
