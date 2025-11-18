use std::fs;

// Parse HTTP request
pub fn parse_request(request: &str) -> (&str, &str) {
    let lines: Vec<&str> = request.lines().collect();
    if lines.is_empty() {
        return ("GET", "/");
    }
    
    let parts: Vec<&str> = lines[0].split_whitespace().collect();
    if parts.len() < 2 {
        return ("GET", "/");
    }
    
    (parts[0], parts[1])
}

// Build HTTP response
pub fn build_response(method: &str, path: &str) -> String {
    if method != "GET" {
        return build_method_not_allowed_response();
    }
    
    match path {
        "/" => build_ok_response(get_index_html()),
        "/about" => build_ok_response(get_about_html()),
        _ => build_not_found_response(),
    }
}

fn build_ok_response(content: String) -> String {
    format!(
        "HTTP/1.1 200 OK\r\n\
         Content-Length: {}\r\n\
         Content-Type: text/html; charset=utf-8\r\n\
         Connection: close\r\n\
         \r\n\
         {}",
        content.len(),
        content
    )
}

fn build_not_found_response() -> String {
    let content = get_404_html();
    format!(
        "HTTP/1.1 404 NOT FOUND\r\n\
         Content-Length: {}\r\n\
         Content-Type: text/html; charset=utf-8\r\n\
         Connection: close\r\n\
         \r\n\
         {}",
        content.len(),
        content
    )
}

fn build_method_not_allowed_response() -> String {
    let content = "Method Not Allowed".to_string();
    format!(
        "HTTP/1.1 405 METHOD NOT ALLOWED\r\n\
         Content-Length: {}\r\n\
         Content-Type: text/plain; charset=utf-8\r\n\
         Connection: close\r\n\
         \r\n\
         {}",
        content.len(),
        content
    )
}

// Return HTML content
fn get_index_html() -> String {
    match fs::read_to_string("index.html") {
        Ok(content) => content,
        Err(_) => {
            // Default HTML content
            "<html><head><title>Rust Web Server</title></head><body><h1>Welcome to Rust Web Server!</h1><p>This is a simple web server implemented in Rust.</p><p><a href=\"/about\">About</a></p></body></html>".to_string()
        }
    }
}

fn get_about_html() -> String {
    "<html><head><title>About</title></head><body><h1>About this server</h1><p>This is an experimental project demonstrating Rust web server implementation with three versions: single-threaded, multi-threaded, and async I/O based on Tokio.</p><p><a href=\"/\">Back to home</a></p></body></html>".to_string()
}

fn get_404_html() -> String {
    "<html><head><title>404 Not Found</title></head><body><h1>404 - Page Not Found</h1><p>The page you requested does not exist.</p><p><a href=\"/\">Back to home</a></p></body></html>".to_string()
}
