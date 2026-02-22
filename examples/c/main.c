#include "DimensionalVariable.h"
#include <stdio.h>
#include <string.h>

static DiplomatStringView str_view(const char* s) {
    DiplomatStringView v;
    v.data = s;
    v.len = strlen(s);
    return v;
}

int main(void) {
    DimensionalVariable_new_result d_res = DimensionalVariable_new(42.0, str_view("m"));
    if (!d_res.is_ok) { fprintf(stderr, "DimensionalVariable_new(d) failed\n"); return 1; }
    DimensionalVariable* d = d_res.ok;

    DimensionalVariable_new_result t_res = DimensionalVariable_new(3.0, str_view("s"));
    if (!t_res.is_ok) { fprintf(stderr, "DimensionalVariable_new(t) failed\n"); DimensionalVariable_destroy(d); return 1; }
    DimensionalVariable* t = t_res.ok;

    DimensionalVariable* v = DimensionalVariable_div(d, t); // 14 m/s
    if (!v) { fprintf(stderr, "DimensionalVariable_div(d,t) failed\n"); DimensionalVariable_destroy(d); DimensionalVariable_destroy(t); return 1; }

    DimensionalVariable_value_in_result mph_res = DimensionalVariable_value_in(v, str_view("mi/hr"));
    if (!mph_res.is_ok) {
        fprintf(stderr, "DimensionalVariable_value_in(v, mi/hr) failed\n");
        DimensionalVariable_destroy(v); DimensionalVariable_destroy(d); DimensionalVariable_destroy(t);
        return 1;
    }

    printf("speed = %.4f mi/hr\n", mph_res.ok);

    DimensionalVariable_destroy(v);
    DimensionalVariable_destroy(d);
    DimensionalVariable_destroy(t);
    return 0;
}
