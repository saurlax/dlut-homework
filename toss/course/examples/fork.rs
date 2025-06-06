use nix::sys::wait::wait;
use nix::unistd::ForkResult::{Child, Parent};
use nix::unistd::{fork, getpid, sleep};

fn main() {
    let pid = fork();
    match pid.expect("Fork failed") {
        Child => {
            println!("Child process with PID: {}", getpid());
            sleep(2); // Simulate some work in the child
            println!("Child process exiting");
        }
        Parent(child_pid) => {
            println!("Parent process with PID: {}", getpid());
            println!("Waiting for child process with PID: {}", child_pid);
            wait().expect("Failed to wait for child");
            println!("Child process has exited");
        }
    }
}
