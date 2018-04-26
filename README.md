# Blitz
A Rust http server focused on speed and minimal memory usage when doing Certificate Authentication. This server looks for speed and low memory usage as its primary focus. Nothing else matters with these considerations in mind. 
Due to the absolutely brutal system in which designing extensive http services in C provides, Rust gives a nicer, lower-headache alternative to acheieving such functionality. This code will have a HUGE focus on concurrency when interacting with the centralized CA system, to allow for a very high volume of concurrent requests to be received and handled with ease. With the C microservice running on the centralized CA server, this code should be extremely fast.

## Compiling
### Requirements
- Rust Nightly Build
- Rocket RS >= 0.3.6 (using 0.3.8 currently)

### Running This Code
`cargo build`

`cargo run`

Navigate to https://localhost:80 in a [REST client](https://www.getpostman.com/) to begin making certificate requests.

**Note** The CA server must also be running for this to do anything.

### Side Note
This code will be supported via docker soon simply to make deployments smoother. Right now that system is not being developed in favor of achieving core functionality and performance. When the aforementioned goals are complete, and docker build system will be established.
