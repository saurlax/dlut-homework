use tokio::io::{AsyncReadExt, AsyncWriteExt};
use tokio::net::{TcpListener, TcpStream};
use tokio::sync::mpsc;

// Global runtime and stop signal
static mut RUNTIME: Option<tokio::runtime::Runtime> = None;
static mut STOP_SIGNAL: Option<mpsc::Sender<()>> = None;
static mut SERVER_THREAD: Option<std::thread::JoinHandle<()>> = None;

pub fn start(host: &str, port: u16) {
    let address = format!("{}:{}", host, port);
    
    // Create stop signal channel
    let (tx, mut rx) = mpsc::channel::<()>(1);
    
    // Create tokio runtime
    let rt = tokio::runtime::Runtime::new().unwrap();
    
    // Store runtime reference
    let rt_handle = rt.handle().clone();
    
    // Create server thread
    let server_thread = std::thread::spawn(move || {
        rt_handle.block_on(async {
            let listener = TcpListener::bind(&address).await.unwrap();
            
            loop {
                tokio::select! {
                    accept_result = listener.accept() => {
                        match accept_result {
                            Ok((socket, _)) => {
                                tokio::spawn(async move {
                                    handle_connection(socket).await;
                                });
                            }
                            Err(e) => {
                                eprintln!("Error accepting connection: {}", e);
                                break;
                            }
                        }
                    }
                    _ = rx.recv() => {
                        println!("Stop signal received, stopping async I/O server...");
                        break;
                    }
                }
            }
            
            println!("Async I/O server stopped");
        });
    });
    
    // Safely store global references
    unsafe {
        RUNTIME = Some(rt);
        STOP_SIGNAL = Some(tx);
        SERVER_THREAD = Some(server_thread);
    }
}

pub fn stop() {
    unsafe {
        if let Some(tx) = STOP_SIGNAL.take() {
            let _ = tx.blocking_send(());
            
            if let Some(handle) = SERVER_THREAD.take() {
                let _ = handle.join();
            }
            
            if let Some(_rt) = RUNTIME.take() {
                // Runtime will end with the thread
            }
            
            println!("Async I/O server fully stopped");
        }
    }
}

async fn handle_connection(mut stream: TcpStream) {
    let mut buffer = [0; 1024];
    let n = stream.read(&mut buffer).await.unwrap();
    
    let request = String::from_utf8_lossy(&buffer[..n]);
    let (method, path) = crate::utils::parse_request(&request);
    
    let response = crate::utils::build_response(method, path);
    
    stream.write_all(response.as_bytes()).await.unwrap();
    stream.flush().await.unwrap();
}
