use std::io::{self, Write};
use std::process::{Command, Stdio};
use std::env;
use std::fs;
use std::path::Path;

fn main() {
    println!("Simple Shell - Type 'exit' to quit");
    
    loop {
        // Show prompt with current directory
        let dir = env::current_dir().unwrap_or_else(|_| Path::new(".").to_path_buf());
        print!("{}> ", dir.to_string_lossy());
        io::stdout().flush().unwrap();
        
        // Read input
        let mut input = String::new();
        if io::stdin().read_line(&mut input).is_err() {
            continue;
        }
        
        let input = input.trim();
        if input.is_empty() { continue; }
        if input == "exit" || input == "quit" { break; }
        
        // Execute command(s)
        let commands: Vec<&str> = input.split('|').map(|cmd| cmd.trim()).collect();
        if commands.len() > 1 {
            execute_piped_commands(&commands);
        } else {
            execute_command(input);
        }
    }
}

// Execute a single command
fn execute_command(cmd_line: &str) {
    let parts: Vec<&str> = cmd_line.split_whitespace().collect();
    if parts.is_empty() {
        return;
    }
    
    let command = parts[0];
    let args = &parts[1..];
    
    match command {
        "cd" => {
            if args.is_empty() {
                println!("Error: cd requires a directory path");
                return;
            }
            
            match env::set_current_dir(args[0]) {
                Ok(_) => {},
                Err(e) => println!("Error changing directory: {}", e),
            }
        },
        "ls" => {
            // Built-in ls command
            let dir = if args.is_empty() {
                ".".to_string()
            } else {
                args[0].to_string()
            };
            
            match fs::read_dir(dir) {
                Ok(entries) => {
                    for entry in entries {
                        match entry {
                            Ok(entry) => {
                                let file_name = entry.file_name();
                                let file_type = match entry.file_type() {
                                    Ok(ft) => {
                                        if ft.is_dir() {
                                            "DIR"
                                        } else if ft.is_file() {
                                            "FILE"
                                        } else {
                                            "OTHER"
                                        }
                                    },
                                    Err(_) => "UNKNOWN"
                                };
                                println!("{} {}", file_type, file_name.to_string_lossy());
                            },
                            Err(e) => println!("Error reading entry: {}", e)
                        }
                    }
                },
                Err(e) => println!("Error reading directory: {}", e)
            }
        },
        _ => {
            // External command execution
            match Command::new(command)
                .args(args)
                .status() {
                    Ok(status) => {
                        if !status.success() {
                            println!("Command failed, exit code: {:?}", status.code());
                        }
                    },
                    Err(e) => {
                        println!("Failed to execute command: {}", e);
                    }
                }
        }
    }
}

// Execute piped commands
fn execute_piped_commands(commands: &[&str]) {
    if commands.is_empty() {
        return;
    }
    
    let mut previous_command = None::<std::process::Child>;
    
    for (i, cmd_line) in commands.iter().enumerate() {
        let parts: Vec<&str> = cmd_line.split_whitespace().collect();
        if parts.is_empty() {
            continue;
        }
        
        let command = parts[0];
        let args = &parts[1..];
        
        let mut cmd = Command::new(command);
        cmd.args(args);
        
        // Configure pipes
        if i == 0 {
            // First command: output to pipe
            cmd.stdout(Stdio::piped());
        } else if i == commands.len() - 1 {
            // Last command: input from previous command
            if let Some(prev) = &mut previous_command {
                cmd.stdin(Stdio::from(prev.stdout.take().unwrap()));
            }
        } else {
            // Middle command: input from previous, output to next
            if let Some(prev) = &mut previous_command {
                cmd.stdin(Stdio::from(prev.stdout.take().unwrap()));
            }
            cmd.stdout(Stdio::piped());
        }
        
        // Execute command
        match cmd.spawn() {
            Ok(proc) => {
                previous_command = Some(proc);
            },
            Err(e) => {
                println!("Pipe command error: {}", e);
                return;
            }
        }
    }
    
    // Wait for the final command to complete
    if let Some(mut last_command) = previous_command {
        match last_command.wait() {
            Ok(status) => {
                if !status.success() {
                    println!("Pipeline failed, exit code: {:?}", status.code());
                }
            },
            Err(e) => println!("Error waiting for command: {}", e),
        }
    }
}
