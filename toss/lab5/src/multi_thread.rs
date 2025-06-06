use std::io::prelude::*;
use std::net::{TcpListener, TcpStream};
use std::sync::{Arc, Mutex};
use std::sync::atomic::{AtomicBool, Ordering};
use std::thread;
use std::time::Duration;
use crate::utils::{parse_request, build_response};

// 全局服务器线程和停止标志
static mut SERVER_THREAD: Option<thread::JoinHandle<()>> = None;
static mut STOP_FLAG: Option<Arc<AtomicBool>> = None;
static mut THREAD_POOL: Option<Arc<ThreadPool>> = None; // 修改为Arc<ThreadPool>

// 简单的线程池实现
struct ThreadPool {
    workers: Vec<Worker>,
    sender: Option<std::sync::mpsc::Sender<Job>>,
}

type Job = Box<dyn FnOnce() + Send + 'static>;

impl ThreadPool {
    fn new(size: usize) -> ThreadPool {
        assert!(size > 0);
        
        let (sender, receiver) = std::sync::mpsc::channel();
        let receiver = Arc::new(Mutex::new(receiver));
        
        let mut workers = Vec::with_capacity(size);
        
        for id in 0..size {
            workers.push(Worker::new(id, Arc::clone(&receiver)));
        }
        
        ThreadPool {
            workers,
            sender: Some(sender),
        }
    }
    
    fn execute<F>(&self, f: F)
    where
        F: FnOnce() + Send + 'static,
    {
        let job = Box::new(f);
        self.sender.as_ref().unwrap().send(job).unwrap();
    }
}

impl Drop for ThreadPool {
    fn drop(&mut self) {
        drop(self.sender.take());
        
        for worker in &mut self.workers {
            if let Some(thread) = worker.thread.take() {
                thread.join().unwrap();
            }
        }
    }
}

struct Worker {
    id: usize,
    thread: Option<thread::JoinHandle<()>>,
}

impl Worker {
    fn new(id: usize, receiver: Arc<Mutex<std::sync::mpsc::Receiver<Job>>>) -> Worker {
        let thread = thread::spawn(move || loop {
            let message = receiver.lock().unwrap().recv();
            
            match message {
                Ok(job) => {
                    job();
                }
                Err(_) => {
                    break;
                }
            }
        });
        
        Worker {
            id,
            thread: Some(thread),
        }
    }
}

pub fn start(host: &str, port: u16) {
    let address = format!("{}:{}", host, port);
    
    // Create stop flag
    let stop_flag = Arc::new(AtomicBool::new(false));
    let stop_flag_clone = stop_flag.clone();
    
    // Create thread pool
    let pool = Arc::new(ThreadPool::new(4)); // Wrap thread pool with Arc
    let pool_clone = pool.clone(); // Create clone for use in thread
    
    // Create server thread
    let server_thread = thread::spawn(move || {
        let listener = TcpListener::bind(&address).unwrap();
        listener.set_nonblocking(true).unwrap();
        
        while !stop_flag_clone.load(Ordering::SeqCst) {
            match listener.accept() {
                Ok((stream, _)) => {
                    pool_clone.execute(|| { // Use thread pool clone
                        handle_connection(stream);
                    });
                }
                Err(e) if e.kind() == std::io::ErrorKind::WouldBlock => {
                    // No connection available, sleep a bit and try again
                    thread::sleep(Duration::from_millis(10));
                    continue;
                }
                Err(e) => {
                    eprintln!("Error accepting connection: {}", e);
                    break;
                }
            }
        }
        
        println!("Multi-threaded server stopped");
    });
    
    // Safely store global references
    unsafe {
        STOP_FLAG = Some(stop_flag);
        SERVER_THREAD = Some(server_thread);
        THREAD_POOL = Some(pool); // Store original thread pool
    }
}

pub fn stop() {
    unsafe {
        if let Some(flag) = STOP_FLAG.as_ref() {
            flag.store(true, Ordering::SeqCst);
            
            if let Some(handle) = SERVER_THREAD.take() {
                let _ = handle.join();
            }
            
            // Dropping the thread pool will clean up all worker threads
            THREAD_POOL = None;
            STOP_FLAG = None;
            
            println!("Multi-threaded server fully stopped");
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
