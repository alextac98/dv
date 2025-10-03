use dv_rs::DimensionalVariable as DV;

fn main() {
    let v = DV::new(10.0, "m/s").expect("create v");
    let t = DV::new(2.0, "s").expect("create t");
    let d = &v * &t; // 20 m
    let meters = d.value_in("m").expect("meters");
    println!("distance = {:.2} m", meters);
}
