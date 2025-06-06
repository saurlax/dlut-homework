use clap::{Arg, Command as ClapCommand};
use std::io::prelude::*;
use std::net::TcpStream;
use std::sync::{
    atomic::{AtomicBool, AtomicU32, Ordering},
    Arc,
};
use std::thread;
use std::time::{Duration, Instant};

mod async_tokio;
mod multi_thread;
mod single_thread;
mod utils;

fn main() {
    let matches = ClapCommand::new("Rust Web Server")
        .about("A simple web server implemented in Rust")
        .arg(
            Arg::new("type")
                .short('t')
                .long("type")
                .value_name("TYPE")
                .help("Server type: single, multi, async, or bench")
                .default_value("single"),
        )
        .arg(
            Arg::new("host")
                .short('H')
                .long("host")
                .value_name("HOST")
                .help("Host address to listen on")
                .default_value("localhost"),
        )
        .arg(
            Arg::new("port")
                .short('p')
                .long("port")
                .value_name("PORT")
                .help("Port to listen on")
                .default_value("7878"),
        )
        .get_matches();

    let server_type = matches.get_one::<String>("type").unwrap();
    let host = matches.get_one::<String>("host").unwrap();
    let port = matches
        .get_one::<String>("port")
        .unwrap()
        .parse::<u16>()
        .unwrap_or(7878);

    println!(
        "Starting {} version of Web server on: http://{}:{}...",
        server_type, host, port
    );

    match server_type.as_str() {
        "single" => {
            single_thread::start(host, port);
            // Keep the server running until the program is terminated
            loop {
                thread::sleep(Duration::from_secs(60));
            }
        }
        "multi" => {
            multi_thread::start(host, port);
            // Keep the server running until the program is terminated
            loop {
                thread::sleep(Duration::from_secs(60));
            }
        }
        "async" => {
            async_tokio::start(host, port);
            // Keep the server running until the program is terminated
            loop {
                thread::sleep(Duration::from_secs(60));
            }
        }
        "bench" => run_benchmark(host, port),
        _ => println!("Unknown server type, please use: single, multi, async or bench"),
    }
}

fn run_benchmark(host: &str, port: u16) {
    println!("Starting performance comparison test...");

    // Set test parameters
    let concurrency = 10;            // Number of concurrent clients
    let requests_per_client = 100;  // Requests per client
    let total_requests = concurrency * requests_per_client;
    
    // Run single-threaded version test
    println!("\nTesting single-threaded version...");
    let single_result = benchmark_server(host, port, "single", concurrency, total_requests);
    println!("Single-threaded test completed in: {:?}", single_result);
    
    // Run multi-threaded version test
    println!("\nTesting multi-threaded version...");
    let multi_result = benchmark_server(host, port, "multi", concurrency, total_requests);
    println!("Multi-threaded test completed in: {:?}", multi_result);
    
    // Run async I/O version test
    println!("\nTesting async I/O version...");
    let async_result = benchmark_server(host, port, "async", concurrency, total_requests);
    println!("Async I/O test completed in: {:?}", async_result);
    
    // Print comparison results
    println!("\nPerformance Comparison Results:");
    println!("-------------------------------");
    println!("Single-threaded: {:?}", single_result);
    println!("Multi-threaded:  {:?}", multi_result);
    println!("Async I/O:       {:?}", async_result);
    println!("-------------------------------");
}

fn benchmark_server(
    host: &str,
    port: u16,
    server_type: &str,
    concurrency: u32,
    total_requests: u32,
) -> Duration {
    // Start the appropriate server
    match server_type {
        "single" => single_thread::start(host, port),
        "multi" => multi_thread::start(host, port),
        "async" => async_tokio::start(host, port),
        _ => panic!("Unknown server type"),
    }

    // Wait for server to start
    thread::sleep(Duration::from_millis(500));

    // Create shared counter and completion flag
    let received_responses = Arc::new(AtomicU32::new(0));
    let benchmark_complete = Arc::new(AtomicBool::new(false));

    // Start timing
    let start = Instant::now();

    // Create client threads
    let mut client_handles = vec![];
    for i in 0..concurrency {
        let thread_host = host.to_string();
        let thread_received = Arc::clone(&received_responses);
        let thread_complete = Arc::clone(&benchmark_complete);
        let requests_per_client = total_requests / concurrency;

        let handle = thread::spawn(move || {
            let server_addr = format!("{}:{}", thread_host, port);
            let mut successful_responses = 0;

            while !thread_complete.load(Ordering::SeqCst)
                && successful_responses < requests_per_client
            {
                match TcpStream::connect(&server_addr) {
                    Ok(mut stream) => {
                        // Set timeout to prevent blocking
                        let _ = stream.set_read_timeout(Some(Duration::from_millis(500)));
                        let _ = stream.set_write_timeout(Some(Duration::from_millis(500)));

                        // Send HTTP GET request
                        let request = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n";
                        if let Ok(_) = stream.write_all(request.as_bytes()) {
                            let _ = stream.flush();

                            // Read response
                            let mut buffer = [0; 1024];
                            if let Ok(bytes_read) = stream.read(&mut buffer) {
                                if bytes_read > 0 {
                                    // Successfully received response
                                    successful_responses += 1;
                                    let total = thread_received.fetch_add(1, Ordering::Relaxed) + 1;

                                    // Print progress every 1000 responses
                                    if total % 1000 == 0 {
                                        println!("Received {}/{} responses", total, total_requests);
                                    }

                                    // Check if completed
                                    if total >= total_requests {
                                        thread_complete.store(true, Ordering::SeqCst);
                                    }
                                }
                            }
                        }
                    }
                    Err(_) => {
                        // Connection failed, wait briefly and retry
                        thread::sleep(Duration::from_millis(1));
                    }
                }
            }

            println!(
                "Client thread {} completed, successfully received {} responses",
                i, successful_responses
            );
        });

        client_handles.push(handle);
    }

    // Wait for all client threads to complete
    for handle in client_handles {
        let _ = handle.join();
    }

    // Calculate total elapsed time
    let elapsed = start.elapsed();

    // Stop the server
    match server_type {
        "single" => single_thread::stop(),
        "multi" => multi_thread::stop(),
        "async" => async_tokio::stop(),
        _ => {}
    }

    // Wait for server to fully stop
    thread::sleep(Duration::from_millis(500));

    // Return benchmark time
    elapsed
}
