use std::io::prelude::*;
use std::net::{TcpListener, TcpStream};
use std::sync::atomic::{AtomicBool, Ordering};
use std::sync::Arc;
use std::time::Duration;
use crate::utils::{parse_request, build_response};

static mut SERVER_THREAD: Option<std::thread::JoinHandle<()>> = None;
static mut STOP_FLAG: Option<Arc<AtomicBool>> = None;

pub fn start(host: &str, port: u16) {
    let address = format!("{}:{}", host, port);
    
    // Create stop flag
    let stop_flag = Arc::new(AtomicBool::new(false));
    let stop_flag_clone = stop_flag.clone();
    
    // Create server thread
    let server_thread = std::thread::spawn(move || {
        let listener = TcpListener::bind(&address).unwrap();
        listener.set_nonblocking(true).unwrap();
        
        while !stop_flag_clone.load(Ordering::SeqCst) {
            match listener.accept() {
                Ok((stream, _)) => {
                    handle_connection(stream);
                }
                Err(e) if e.kind() == std::io::ErrorKind::WouldBlock => {
                    // No connection available, sleep a bit and try again
                    std::thread::sleep(Duration::from_millis(10));
                    continue;
                }
                Err(e) => {
                    eprintln!("Error accepting connection: {}", e);
                    break;
                }
            }
        }
        
        println!("Single-threaded server stopped");
    });
    
    // Safely store global references
    unsafe {
        STOP_FLAG = Some(stop_flag);
        SERVER_THREAD = Some(server_thread);
    }
}

pub fn stop() {
    unsafe {
        if let Some(flag) = STOP_FLAG.as_ref() {
            flag.store(true, Ordering::SeqCst);
            
            if let Some(handle) = SERVER_THREAD.take() {
                let _ = handle.join();
            }
            
            STOP_FLAG = None;
            println!("Single-threaded server fully stopped");
        }
    }
}

fn handle_connection(mut stream: TcpStream) {
    let mut buffer = [0; 1024];
    stream.read(&mut buffer).unwrap();
    
    let request = String::from_utf8_lossy(&buffer[..]);
    let (method, path) = parse_request(&request);
    
    let response = build_response(method, path);
    
    stream.write(response.as_bytes()).unwrap();
    stream.flush().unwrap();
}
