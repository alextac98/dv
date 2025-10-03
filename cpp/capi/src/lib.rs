//! C ABI for dv core.

use std::ffi::{CStr, CString};
use std::os::raw::{c_char, c_double, c_int};
use std::ptr;

use dv_rs::{units, DimensionalVariable};

// Opaque handle for C API
#[repr(C)]
pub struct dv_var {
    inner: DimensionalVariable,
}

// Thread-local last error string for C API callers.
thread_local! {
    static LAST_ERROR: std::cell::RefCell<Option<String>> = const { std::cell::RefCell::new(None) };
}

fn set_last_error(msg: String) {
    LAST_ERROR.with(|e| *e.borrow_mut() = Some(msg));
}

#[no_mangle]
pub extern "C" fn dv_last_error_message() -> *const c_char {
    LAST_ERROR.with(|e| {
        if let Some(ref s) = *e.borrow() {
            // Leak a CString per call; caller should copy immediately. We can reuse a TLS buffer if needed.
            let c = CString::new(s.as_str()).unwrap_or_else(|_| CString::new("invalid error").unwrap());
            let ptr = c.as_ptr();
            std::mem::forget(c);
            ptr
        } else {
            ptr::null()
        }
    })
}

#[no_mangle]
pub extern "C" fn dv_free_cstring(s: *mut c_char) {
    if s.is_null() { return; }
    unsafe { let _ = CString::from_raw(s); }
}

#[no_mangle]
pub extern "C" fn dv_base_units_size() -> usize {
    units::BASE_UNITS_SIZE
}

// Create a variable from value and unit string.
#[no_mangle]
pub extern "C" fn dv_var_new(value: c_double, unit_str: *const c_char) -> *mut dv_var {
    if unit_str.is_null() {
        set_last_error("unit_str was null".to_string());
        return ptr::null_mut();
    }
    let cstr = unsafe { CStr::from_ptr(unit_str) };
    let unit = match cstr.to_str() {
        Ok(s) => s,
        Err(_) => {
            set_last_error("unit_str not valid UTF-8".to_string());
            return ptr::null_mut();
        }
    };
    match DimensionalVariable::new(value, unit) {
        Ok(inner) => Box::into_raw(Box::new(dv_var { inner })),
        Err(e) => {
            set_last_error(e);
            ptr::null_mut()
        }
    }
}

#[no_mangle]
pub extern "C" fn dv_var_free(ptr_: *mut dv_var) {
    if ptr_.is_null() { return; }
    unsafe { let _ = Box::from_raw(ptr_); }
}

#[no_mangle]
pub extern "C" fn dv_var_value(ptr_: *const dv_var) -> c_double {
    if ptr_.is_null() { return f64::NAN; }
    let v = unsafe { &(*ptr_) };
    v.inner.value()
}

// Returns 1 if unitless, 0 otherwise.
#[no_mangle]
pub extern "C" fn dv_var_is_unitless(ptr_: *const dv_var) -> c_int {
    if ptr_.is_null() { return 0; }
    let v = unsafe { &(*ptr_) };
    if v.inner.is_unitless() { 1 } else { 0 }
}

// Attempt to convert to a target unit; on success stores into out_value and returns 1. On failure, returns 0 and sets last error.
#[no_mangle]
pub extern "C" fn dv_var_value_in(ptr_: *const dv_var, unit_str: *const c_char, out_value: *mut c_double) -> c_int {
    if ptr_.is_null() { set_last_error("null dv_var".to_string()); return 0; }
    if unit_str.is_null() { set_last_error("unit_str was null".to_string()); return 0; }
    if out_value.is_null() { set_last_error("out_value was null".to_string()); return 0; }
    let v = unsafe { &(*ptr_) };
    let cstr = unsafe { CStr::from_ptr(unit_str) };
    let unit = match cstr.to_str() { Ok(s) => s, Err(_) => { set_last_error("unit_str not valid UTF-8".to_string()); return 0; } };
    match v.inner.value_in(unit) {
        Ok(val) => { unsafe { *out_value = val; } 1 },
        Err(e) => { set_last_error(e); 0 }
    }
}

// Arithmetic operations. All return a newly allocated dv_var* or null on error.
#[no_mangle]
pub extern "C" fn dv_var_add(a: *const dv_var, b: *const dv_var) -> *mut dv_var {
    if a.is_null() || b.is_null() { set_last_error("null operand".to_string()); return ptr::null_mut(); }
    let a = unsafe { &(*a) };
    let b = unsafe { &(*b) };
    match a.inner.try_add(&b.inner) {
        Ok(v) => Box::into_raw(Box::new(dv_var { inner: v })),
        Err(e) => { set_last_error(e); ptr::null_mut() }
    }
}

#[no_mangle]
pub extern "C" fn dv_var_sub(a: *const dv_var, b: *const dv_var) -> *mut dv_var {
    if a.is_null() || b.is_null() { set_last_error("null operand".to_string()); return ptr::null_mut(); }
    let a = unsafe { &(*a) };
    let b = unsafe { &(*b) };
    match a.inner.try_sub(&b.inner) {
        Ok(v) => Box::into_raw(Box::new(dv_var { inner: v })),
        Err(e) => { set_last_error(e); ptr::null_mut() }
    }
}

#[no_mangle]
pub extern "C" fn dv_var_mul(a: *const dv_var, b: *const dv_var) -> *mut dv_var {
    if a.is_null() || b.is_null() { set_last_error("null operand".to_string()); return ptr::null_mut(); }
    let a = unsafe { &(*a) };
    let b = unsafe { &(*b) };
    let v = &a.inner * &b.inner;
    Box::into_raw(Box::new(dv_var { inner: v }))
}

#[no_mangle]
pub extern "C" fn dv_var_div(a: *const dv_var, b: *const dv_var) -> *mut dv_var {
    if a.is_null() || b.is_null() { set_last_error("null operand".to_string()); return ptr::null_mut(); }
    let a = unsafe { &(*a) };
    let b = unsafe { &(*b) };
    let v = &a.inner / &b.inner;
    Box::into_raw(Box::new(dv_var { inner: v }))
}

#[no_mangle]
pub extern "C" fn dv_var_mul_scalar(a: *const dv_var, s: c_double) -> *mut dv_var {
    if a.is_null() { set_last_error("null operand".to_string()); return ptr::null_mut(); }
    let a = unsafe { &(*a) };
    let v = &a.inner * s;
    Box::into_raw(Box::new(dv_var { inner: v }))
}

#[no_mangle]
pub extern "C" fn dv_var_div_scalar(a: *const dv_var, s: c_double) -> *mut dv_var {
    if a.is_null() { set_last_error("null operand".to_string()); return ptr::null_mut(); }
    let a = unsafe { &(*a) };
    let v = &a.inner / s;
    Box::into_raw(Box::new(dv_var { inner: v }))
}

#[no_mangle]
pub extern "C" fn dv_var_powi(a: *const dv_var, exp: c_int) -> *mut dv_var {
    if a.is_null() { set_last_error("null operand".to_string()); return ptr::null_mut(); }
    let a = unsafe { &(*a) };
    let v = a.inner.powi(exp as i32);
    Box::into_raw(Box::new(dv_var { inner: v }))
}

#[no_mangle]
pub extern "C" fn dv_var_powf(a: *const dv_var, exp: c_double) -> *mut dv_var {
    if a.is_null() { set_last_error("null operand".to_string()); return ptr::null_mut(); }
    let a = unsafe { &(*a) };
    match a.inner.powf(exp) {
        Ok(v) => Box::into_raw(Box::new(dv_var { inner: v })),
        Err(e) => { set_last_error(e); ptr::null_mut() }
    }
}

#[no_mangle]
pub extern "C" fn dv_var_sqrt(a: *const dv_var) -> *mut dv_var {
    if a.is_null() { set_last_error("null operand".to_string()); return ptr::null_mut(); }
    let a = unsafe { &(*a) };
    match a.inner.sqrt() {
        Ok(v) => Box::into_raw(Box::new(dv_var { inner: v })),
        Err(e) => { set_last_error(e); ptr::null_mut() }
    }
}
