use dv_rs::DimensionalVariable as DV;

fn main() {
    let v = DV::new(10.0, "m/s").expect("create v");
    let t = DV::new(2.0, "s").expect("create t");
    let d = &v * &t; // 20 m
    let meters = d.value_in("m").expect("meters");
    println!("distance = {:.2} m", meters);
    
    // Working with angles
    let angle_rad = DV::new(std::f64::consts::PI, "rad").expect("create angle");
    let angle_deg = angle_rad.value_in("deg").expect("degrees");
    println!("π radians = {:.2} degrees", angle_deg);
}
