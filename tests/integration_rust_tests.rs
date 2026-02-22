use dv_rs::DimensionalVariable as Dv;
use std::collections::BTreeMap;
use std::env;
use std::fs;
use std::path::{Path, PathBuf};

#[derive(Clone, Debug)]
enum JValue {
    Obj(BTreeMap<String, JValue>),
    Arr(Vec<JValue>),
    Str(String),
    Num(f64),
    Bool(bool),
    Null,
}

struct Parser<'a> {
    s: &'a [u8],
    i: usize,
}

impl<'a> Parser<'a> {
    fn new(s: &'a str) -> Self {
        Self { s: s.as_bytes(), i: 0 }
    }

    fn parse(mut self) -> Result<JValue, String> {
        let v = self.parse_value()?;
        self.skip_ws();
        if self.i != self.s.len() {
            return Err(format!("unexpected trailing content at byte {}", self.i));
        }
        Ok(v)
    }

    fn parse_value(&mut self) -> Result<JValue, String> {
        self.skip_ws();
        let b = self.peek().ok_or("unexpected EOF while parsing value")?;
        match b {
            b'{' => self.parse_object(),
            b'[' => self.parse_array(),
            b'"' => self.parse_string().map(JValue::Str),
            b't' | b'f' => self.parse_bool().map(JValue::Bool),
            b'n' => {
                self.expect_bytes(b"null")?;
                Ok(JValue::Null)
            }
            b'-' | b'0'..=b'9' => self.parse_number().map(JValue::Num),
            _ => Err(format!("unexpected char '{}' at byte {}", b as char, self.i)),
        }
    }

    fn parse_object(&mut self) -> Result<JValue, String> {
        self.expect(b'{')?;
        self.skip_ws();
        let mut map = BTreeMap::new();
        if self.peek() == Some(b'}') {
            self.i += 1;
            return Ok(JValue::Obj(map));
        }
        loop {
            self.skip_ws();
            let key = self.parse_string()?;
            self.skip_ws();
            self.expect(b':')?;
            let value = self.parse_value()?;
            map.insert(key, value);
            self.skip_ws();
            match self.peek() {
                Some(b',') => self.i += 1,
                Some(b'}') => {
                    self.i += 1;
                    break;
                }
                _ => return Err(format!("expected ',' or '}}' at byte {}", self.i)),
            }
        }
        Ok(JValue::Obj(map))
    }

    fn parse_array(&mut self) -> Result<JValue, String> {
        self.expect(b'[')?;
        self.skip_ws();
        let mut items = Vec::new();
        if self.peek() == Some(b']') {
            self.i += 1;
            return Ok(JValue::Arr(items));
        }
        loop {
            items.push(self.parse_value()?);
            self.skip_ws();
            match self.peek() {
                Some(b',') => self.i += 1,
                Some(b']') => {
                    self.i += 1;
                    break;
                }
                _ => return Err(format!("expected ',' or ']' at byte {}", self.i)),
            }
        }
        Ok(JValue::Arr(items))
    }

    fn parse_string(&mut self) -> Result<String, String> {
        self.expect(b'"')?;
        let mut out = String::new();
        while let Some(b) = self.peek() {
            self.i += 1;
            match b {
                b'"' => return Ok(out),
                b'\\' => {
                    let esc = self.peek().ok_or("unterminated escape")?;
                    self.i += 1;
                    match esc {
                        b'"' => out.push('"'),
                        b'\\' => out.push('\\'),
                        b'/' => out.push('/'),
                        b'b' => out.push('\u{0008}'),
                        b'f' => out.push('\u{000C}'),
                        b'n' => out.push('\n'),
                        b'r' => out.push('\r'),
                        b't' => out.push('\t'),
                        _ => return Err(format!("unsupported escape \\{} at byte {}", esc as char, self.i - 1)),
                    }
                }
                _ => out.push(b as char),
            }
        }
        Err("unterminated string".to_string())
    }

    fn parse_bool(&mut self) -> Result<bool, String> {
        if self.starts_with(b"true") {
            self.i += 4;
            return Ok(true);
        }
        if self.starts_with(b"false") {
            self.i += 5;
            return Ok(false);
        }
        Err(format!("invalid boolean at byte {}", self.i))
    }

    fn parse_number(&mut self) -> Result<f64, String> {
        let start = self.i;
        if self.peek() == Some(b'-') {
            self.i += 1;
        }
        self.consume_digits();
        if self.peek() == Some(b'.') {
            self.i += 1;
            self.consume_digits();
        }
        if matches!(self.peek(), Some(b'e' | b'E')) {
            self.i += 1;
            if matches!(self.peek(), Some(b'+' | b'-')) {
                self.i += 1;
            }
            self.consume_digits();
        }
        let s = std::str::from_utf8(&self.s[start..self.i]).map_err(|_| "invalid number bytes")?;
        s.parse::<f64>()
            .map_err(|e| format!("failed to parse number '{}': {}", s, e))
    }

    fn consume_digits(&mut self) {
        while matches!(self.peek(), Some(b'0'..=b'9')) {
            self.i += 1;
        }
    }

    fn skip_ws(&mut self) {
        while matches!(self.peek(), Some(b' ' | b'\n' | b'\r' | b'\t')) {
            self.i += 1;
        }
    }

    fn expect(&mut self, ch: u8) -> Result<(), String> {
        if self.peek() == Some(ch) {
            self.i += 1;
            Ok(())
        } else {
            Err(format!("expected '{}' at byte {}", ch as char, self.i))
        }
    }

    fn expect_bytes(&mut self, bytes: &[u8]) -> Result<(), String> {
        if self.starts_with(bytes) {
            self.i += bytes.len();
            Ok(())
        } else {
            Err(format!("expected token at byte {}", self.i))
        }
    }

    fn starts_with(&self, prefix: &[u8]) -> bool {
        self.s.get(self.i..self.i + prefix.len()) == Some(prefix)
    }

    fn peek(&self) -> Option<u8> {
        self.s.get(self.i).copied()
    }
}

fn as_obj<'a>(v: &'a JValue, ctx: &str) -> &'a BTreeMap<String, JValue> {
    if let JValue::Obj(o) = v {
        o
    } else {
        panic!("{}: expected object", ctx)
    }
}

fn as_arr<'a>(v: &'a JValue, ctx: &str) -> &'a [JValue] {
    if let JValue::Arr(a) = v {
        a
    } else {
        panic!("{}: expected array", ctx)
    }
}

fn get_str(obj: &BTreeMap<String, JValue>, key: &str, ctx: &str) -> String {
    match obj.get(key) {
        Some(JValue::Str(s)) => s.clone(),
        _ => panic!("{}: missing or invalid string key '{}'", ctx, key),
    }
}

fn get_num(obj: &BTreeMap<String, JValue>, key: &str, ctx: &str) -> f64 {
    match obj.get(key) {
        Some(JValue::Num(n)) => *n,
        _ => panic!("{}: missing or invalid number key '{}'", ctx, key),
    }
}

fn get_bool(obj: &BTreeMap<String, JValue>, key: &str, ctx: &str) -> bool {
    match obj.get(key) {
        Some(JValue::Bool(b)) => *b,
        _ => panic!("{}: missing or invalid bool key '{}'", ctx, key),
    }
}

fn resolve_vector_path(file_name: &str) -> PathBuf {
    let workspace = env::var("TEST_WORKSPACE").unwrap_or_else(|_| "dv".to_string());
    let candidates = [
        env::var("TEST_SRCDIR")
            .ok()
            .map(|v| Path::new(&v).join(&workspace).join("tests").join(file_name)),
        Some(Path::new("tests").join(file_name)),
        Some(Path::new(file_name).to_path_buf()),
    ];
    for c in candidates.into_iter().flatten() {
        if c.exists() {
            return c;
        }
    }
    panic!("could not locate tests/{}", file_name)
}

fn nearly_eq(actual: f64, expected: f64, abs_tol: f64, rel_tol: f64) -> bool {
    let diff = (actual - expected).abs();
    diff <= abs_tol.max(rel_tol * expected.abs())
}

fn assert_expected_dv(case_id: &str, expect: &BTreeMap<String, JValue>, dv: &Dv, abs_tol: f64, rel_tol: f64) {
    let exp_value = get_num(expect, "value", case_id);
    let exp_unit = get_str(expect, "unit", case_id);
    let actual = dv
        .value_in(&exp_unit)
        .unwrap_or_else(|e| panic!("{}: value_in failed: {}", case_id, e));
    assert!(
        nearly_eq(actual, exp_value, abs_tol, rel_tol),
        "{}: expected {}, got {}",
        case_id,
        exp_value,
        actual
    );
}

fn assert_expected_scalar(case_id: &str, expect: &BTreeMap<String, JValue>, actual: f64, abs_tol: f64, rel_tol: f64) {
    let exp_value = get_num(expect, "value", case_id);
    let exp_unit = get_str(expect, "unit", case_id);
    assert!(exp_unit.is_empty(), "{}: scalar operations must expect unit ''", case_id);
    assert!(
        nearly_eq(actual, exp_value, abs_tol, rel_tol),
        "{}: expected {}, got {}",
        case_id,
        exp_value,
        actual
    );
}

fn build_input_dv(case_obj: &BTreeMap<String, JValue>, case_id: &str) -> Dv {
    let input = as_obj(case_obj.get("input").expect("missing input"), case_id);
    let value = get_num(input, "value", case_id);
    let unit = get_str(input, "unit", case_id);
    Dv::new(value, &unit).unwrap_or_else(|e| panic!("{}: failed to construct input: {}", case_id, e))
}

fn run_vectors(file_name: &str) {
    let path = resolve_vector_path(file_name);
    let raw = fs::read_to_string(path).expect("failed to read vector file");
    let root = Parser::new(&raw).parse().expect("invalid JSON in vector file");
    let root_obj = as_obj(&root, "root");

    let defaults = root_obj
        .get("defaults")
        .map(|v| as_obj(v, "defaults"));
    let abs_tol = defaults.map(|d| get_num(d, "abs_tol", "defaults")).unwrap_or(1e-10);
    let rel_tol = defaults.map(|d| get_num(d, "rel_tol", "defaults")).unwrap_or(1e-10);

    let cases = as_arr(root_obj.get("cases").expect("missing cases"), "cases");

    for case_v in cases {
        let case = as_obj(case_v, "case");
        let case_id = get_str(case, "id", "case");
        let op = get_str(case, "op", &case_id);
        let expect_obj = as_obj(case.get("expect").expect("missing expect"), &case_id);
        let ok = get_bool(expect_obj, "ok", &case_id);

        match op.as_str() {
            "construct" => {
                let input = as_obj(case.get("input").expect("missing input"), &case_id);
                let value = get_num(input, "value", &case_id);
                let unit = get_str(input, "unit", &case_id);
                if ok {
                    let dv = Dv::new(value, &unit)
                        .unwrap_or_else(|e| panic!("{}: expected success, got error {}", case_id, e));
                    assert_expected_dv(&case_id, expect_obj, &dv, abs_tol, rel_tol);
                } else {
                    assert!(Dv::new(value, &unit).is_err(), "{}: expected construct error", case_id);
                }
            }
            "convert" => {
                let input = as_obj(case.get("input").expect("missing input"), &case_id);
                let value = get_num(input, "value", &case_id);
                let from_unit = get_str(input, "from_unit", &case_id);
                let to_unit = get_str(input, "to_unit", &case_id);
                let dv = Dv::new(value, &from_unit)
                    .unwrap_or_else(|e| panic!("{}: input build failed: {}", case_id, e));
                if ok {
                    let actual = dv.value_in(&to_unit)
                        .unwrap_or_else(|e| panic!("{}: expected conversion success, got {}", case_id, e));
                    let exp_value = get_num(expect_obj, "value", &case_id);
                    assert!(
                        nearly_eq(actual, exp_value, abs_tol, rel_tol),
                        "{}: expected {}, got {}",
                        case_id,
                        exp_value,
                        actual
                    );
                } else {
                    assert!(dv.value_in(&to_unit).is_err(), "{}: expected conversion error", case_id);
                }
            }
            "add" | "sub" | "mul" | "div" => {
                let left_obj = as_obj(case.get("left").expect("missing left"), &case_id);
                let right_obj = as_obj(case.get("right").expect("missing right"), &case_id);
                let left = Dv::new(get_num(left_obj, "value", &case_id), &get_str(left_obj, "unit", &case_id))
                    .unwrap_or_else(|e| panic!("{}: left build failed: {}", case_id, e));
                let right = Dv::new(get_num(right_obj, "value", &case_id), &get_str(right_obj, "unit", &case_id))
                    .unwrap_or_else(|e| panic!("{}: right build failed: {}", case_id, e));

                if !ok {
                    match op.as_str() {
                        "add" => assert!(left.try_add(&right).is_err(), "{}: expected add error", case_id),
                        "sub" => assert!(left.try_sub(&right).is_err(), "{}: expected sub error", case_id),
                        _ => panic!("{}: ok=false unsupported for {}", case_id, op),
                    }
                    continue;
                }

                let result = match op.as_str() {
                    "add" => &left + &right,
                    "sub" => &left - &right,
                    "mul" => &left * &right,
                    "div" => &left / &right,
                    _ => unreachable!(),
                };
                assert_expected_dv(&case_id, expect_obj, &result, abs_tol, rel_tol);
            }
            "powi" => {
                let dv = build_input_dv(case, &case_id);
                let exponent = get_num(case, "exponent", &case_id) as i32;
                if ok {
                    let result = dv.powi(exponent);
                    assert_expected_dv(&case_id, expect_obj, &result, abs_tol, rel_tol);
                } else {
                    panic!("{}: powi with ok=false is not supported", case_id);
                }
            }
            "powf" => {
                let dv = build_input_dv(case, &case_id);
                let exponent = get_num(case, "exponent", &case_id);
                if ok {
                    let result = dv.powf(exponent)
                        .unwrap_or_else(|e| panic!("{}: powf failed: {}", case_id, e));
                    assert_expected_dv(&case_id, expect_obj, &result, abs_tol, rel_tol);
                } else {
                    assert!(dv.powf(exponent).is_err(), "{}: expected powf error", case_id);
                }
            }
            "sqrt" => {
                let dv = build_input_dv(case, &case_id);
                if ok {
                    let result = dv.sqrt()
                        .unwrap_or_else(|e| panic!("{}: sqrt failed: {}", case_id, e));
                    assert_expected_dv(&case_id, expect_obj, &result, abs_tol, rel_tol);
                } else {
                    assert!(dv.sqrt().is_err(), "{}: expected sqrt error", case_id);
                }
            }
            "ln" => {
                let dv = build_input_dv(case, &case_id);
                if ok {
                    let result = dv.ln()
                        .unwrap_or_else(|e| panic!("{}: ln failed: {}", case_id, e));
                    assert_expected_scalar(&case_id, expect_obj, result, abs_tol, rel_tol);
                } else {
                    assert!(dv.ln().is_err(), "{}: expected ln error", case_id);
                }
            }
            "log2" => {
                let dv = build_input_dv(case, &case_id);
                if ok {
                    let result = dv.log2()
                        .unwrap_or_else(|e| panic!("{}: log2 failed: {}", case_id, e));
                    assert_expected_scalar(&case_id, expect_obj, result, abs_tol, rel_tol);
                } else {
                    assert!(dv.log2().is_err(), "{}: expected log2 error", case_id);
                }
            }
            "log10" => {
                let dv = build_input_dv(case, &case_id);
                if ok {
                    let result = dv.log10()
                        .unwrap_or_else(|e| panic!("{}: log10 failed: {}", case_id, e));
                    assert_expected_scalar(&case_id, expect_obj, result, abs_tol, rel_tol);
                } else {
                    assert!(dv.log10().is_err(), "{}: expected log10 error", case_id);
                }
            }
            "sin" => {
                let dv = build_input_dv(case, &case_id);
                if ok {
                    let result = dv.sin()
                        .unwrap_or_else(|e| panic!("{}: sin failed: {}", case_id, e));
                    assert_expected_scalar(&case_id, expect_obj, result, abs_tol, rel_tol);
                } else {
                    assert!(dv.sin().is_err(), "{}: expected sin error", case_id);
                }
            }
            "cos" => {
                let dv = build_input_dv(case, &case_id);
                if ok {
                    let result = dv.cos()
                        .unwrap_or_else(|e| panic!("{}: cos failed: {}", case_id, e));
                    assert_expected_scalar(&case_id, expect_obj, result, abs_tol, rel_tol);
                } else {
                    assert!(dv.cos().is_err(), "{}: expected cos error", case_id);
                }
            }
            "tan" => {
                let dv = build_input_dv(case, &case_id);
                if ok {
                    let result = dv.tan()
                        .unwrap_or_else(|e| panic!("{}: tan failed: {}", case_id, e));
                    assert_expected_scalar(&case_id, expect_obj, result, abs_tol, rel_tol);
                } else {
                    assert!(dv.tan().is_err(), "{}: expected tan error", case_id);
                }
            }
            "asin" => {
                let dv = build_input_dv(case, &case_id);
                if ok {
                    let result = dv.asin()
                        .unwrap_or_else(|e| panic!("{}: asin failed: {}", case_id, e));
                    assert_expected_dv(&case_id, expect_obj, &result, abs_tol, rel_tol);
                } else {
                    assert!(dv.asin().is_err(), "{}: expected asin error", case_id);
                }
            }
            "acos" => {
                let dv = build_input_dv(case, &case_id);
                if ok {
                    let result = dv.acos()
                        .unwrap_or_else(|e| panic!("{}: acos failed: {}", case_id, e));
                    assert_expected_dv(&case_id, expect_obj, &result, abs_tol, rel_tol);
                } else {
                    assert!(dv.acos().is_err(), "{}: expected acos error", case_id);
                }
            }
            "atan" => {
                let dv = build_input_dv(case, &case_id);
                if ok {
                    let result = dv.atan()
                        .unwrap_or_else(|e| panic!("{}: atan failed: {}", case_id, e));
                    assert_expected_dv(&case_id, expect_obj, &result, abs_tol, rel_tol);
                } else {
                    assert!(dv.atan().is_err(), "{}: expected atan error", case_id);
                }
            }
            "neg" => {
                let dv = build_input_dv(case, &case_id);
                let result = -&dv;
                assert_expected_dv(&case_id, expect_obj, &result, abs_tol, rel_tol);
            }
            "abs" => {
                let dv = build_input_dv(case, &case_id);
                let result = dv.abs();
                assert_expected_dv(&case_id, expect_obj, &result, abs_tol, rel_tol);
            }
            _ => panic!("{}: unsupported op {}", case_id, op),
        }
    }
}

#[test]
fn integration_units_vectors() {
    run_vectors("units_tests.json");
}

#[test]
fn integration_math_vectors() {
    run_vectors("math_tests.json");
}
