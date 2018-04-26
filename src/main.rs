#![feature(plugin)]
#![plugin(rocket_codegen)]

extern crate rocket;
#[macro_use] extern crate rocket_contrib;

use rocket_contrib::{Json, Value};

#[get("/")]
fn validate() {
    println!("Hello!")
}

fn main() {
    rocket::ignite()
        .mount("/validate", routes![validate])
        .launch();
}
