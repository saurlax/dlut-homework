use std::sync::mpsc;
use std::sync::{Arc, Mutex};
use std::thread;
use std::time::Duration;

fn main() {
    // 基本线程创建和join
    basic_threading();

    // 使用互斥锁共享数据
    shared_data_with_mutex();

    // 使用通道进行线程间通信
    channel_communication();

    // 简单线程池实现
    simple_thread_pool();
}

fn basic_threading() {
    println!("\n=== 基本线程创建和join ===");

    let handle = thread::spawn(|| {
        for i in 1..5 {
            println!("子线程: 计数 {}", i);
            thread::sleep(Duration::from_millis(1));
        }
    });

    for i in 1..5 {
        println!("主线程: 计数 {}", i);
        thread::sleep(Duration::from_millis(1));
    }

    // 等待子线程完成
    handle.join().unwrap();
    println!("子线程已完成");
}

fn shared_data_with_mutex() {
    println!("\n=== 使用互斥锁共享数据 ===");

    // 创建一个可以跨线程共享的计数器
    let counter = Arc::new(Mutex::new(0));
    let mut handles = vec![];

    for i in 0..5 {
        // 为每个线程克隆一个计数器的引用
        let counter_clone = Arc::clone(&counter);

        let handle = thread::spawn(move || {
            let mut num = counter_clone.lock().unwrap();
            *num += 1;
            println!("线程 {}: 计数器现在是 {}", i, *num);
        });

        handles.push(handle);
    }

    // 等待所有线程完成
    for handle in handles {
        handle.join().unwrap();
    }

    println!("最终计数器值: {}", *counter.lock().unwrap());
}

fn channel_communication() {
    println!("\n=== 使用通道进行线程间通信 ===");

    // 创建一个通道
    let (tx, rx) = mpsc::channel();

    // 创建多个发送者
    let tx1 = tx.clone();
    thread::spawn(move || {
        let messages = vec!["你好", "从", "线程1"];
        for msg in messages {
            tx1.send(msg).unwrap();
            thread::sleep(Duration::from_millis(100));
        }
    });

    thread::spawn(move || {
        let messages = vec!["消息", "来自", "线程2"];
        for msg in messages {
            tx.send(msg).unwrap();
            thread::sleep(Duration::from_millis(100));
        }
    });

    // 在主线程中接收消息
    for _ in 0..6 {
        let received = rx.recv().unwrap();
        println!("收到: {}", received);
    }
}

fn simple_thread_pool() {
    println!("\n=== 简单线程池实现 ===");

    // 创建任务通道
    let (tx, rx) = mpsc::channel();

    // 创建一个接收器，可以在多个线程之间共享
    let rx = Arc::new(Mutex::new(rx));

    // 创建线程池，这里使用4个线程
    let mut handles = vec![];
    for id in 0..4 {
        let rx = Arc::clone(&rx);

        let handle = thread::spawn(move || {
            loop {
                // 尝试获取任务
                let task = match rx.lock().unwrap().recv() {
                    Ok(job) => job,
                    Err(_) => break, // 通道已关闭，退出循环
                };

                // 执行任务（这里的任务就是一个要打印的数字）
                println!("线程 {} 处理任务: {}", id, task);
                thread::sleep(Duration::from_millis(100));
            }
        });

        handles.push(handle);
    }

    // 发送一些任务
    for i in 0..10 {
        tx.send(i).unwrap();
    }

    // 丢弃发送者，这会关闭通道
    drop(tx);

    // 等待所有工作线程完成
    for handle in handles {
        handle.join().unwrap();
    }

    println!("所有任务已完成");
}
