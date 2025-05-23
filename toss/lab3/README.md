# 命令行解释器（Shell）实验报告

## 1. 个人信息

软 2305-文浩然-20232241392

## 2. 实验说明

本实验要求使用 Rust 语言实现一个简单的命令行解释器（Shell），要求支持基本的命令解析、执行以及错误处理。可选要求实现 Unix 风格的管道机制。

## 3. 实验要求

- 在 Linux 环境下实现
- 支持对命令的参数解析
- 做好错误处理
- 可选：实现类似 Unix 的命令间管道机制的支持

## 4. 实现思路

### 4.1 总体设计

设计一个基本的 Shell 交互循环（REPL：Read, Evaluate, Print, Loop）：

1. 显示带有当前目录的提示符
2. 读取用户输入的命令
3. 解析并执行命令
4. 返回到步骤 1

### 4.2 核心功能模块

- **交互循环**：不断读取并执行用户输入的命令，直到用户退出
- **命令解析**：将用户输入的命令行分割为命令名和参数
- **命令执行**：包含内置命令的实现和外部命令的调用
- **管道机制**：支持多个命令通过管道连接执行

## 5. 代码实现与分析

### 5.1 主函数与交互循环

```rust
fn main() {
    println!("Simple Shell - Type 'exit' to quit");

    loop {
        // 显示包含当前目录的提示符
        let dir = env::current_dir().unwrap_or_else(|_| Path::new(".").to_path_buf());
        print!("{}> ", dir.to_string_lossy());
        io::stdout().flush().unwrap();

        // 读取用户输入
        let mut input = String::new();
        if io::stdin().read_line(&mut input).is_err() {
            continue;
        }

        let input = input.trim();
        if input.is_empty() { continue; }
        if input == "exit" || input == "quit" { break; }

        // 执行命令
        let commands: Vec<&str> = input.split('|').map(|cmd| cmd.trim()).collect();
        if commands.len() > 1 {
            execute_piped_commands(&commands);
        } else {
            execute_command(input);
        }
    }
}
```

主函数实现了一个简单的 REPL 循环：

- 显示带有当前工作目录的提示符
- 读取用户输入，处理空输入和退出命令
- 检测输入是否包含管道符号`|`，根据情况调用不同的执行函数

### 5.2 单个命令执行

```rust
fn execute_command(cmd_line: &str) {
    let parts: Vec<&str> = cmd_line.split_whitespace().collect();
    if parts.is_empty() {
        return;
    }

    let command = parts[0];
    let args = &parts[1..];

    match command {
        "cd" => {
            // 内置cd命令实现
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
            // 内置ls命令实现
            // ...省略详细实现...
        },
        _ => {
            // 外部命令执行
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
```

该函数处理单个命令的执行：

- 将命令行拆分为命令和参数
- 提供内置命令(`cd`, `ls`)的实现
- 对于其他命令，调用系统命令执行
- 包含基本的错误处理

### 5.3 管道命令执行

```rust
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

        // 配置管道
        if i == 0 {
            // 第一个命令：输出到管道
            cmd.stdout(Stdio::piped());
        } else if i == commands.len() - 1 {
            // 最后一个命令：从上一个命令获取输入
            if let Some(prev) = &mut previous_command {
                cmd.stdin(Stdio::from(prev.stdout.take().unwrap()));
            }
        } else {
            // 中间命令：从上一个获取输入，输出到下一个
            if let Some(prev) = &mut previous_command {
                cmd.stdin(Stdio::from(prev.stdout.take().unwrap()));
            }
            cmd.stdout(Stdio::piped());
        }

        // 执行命令
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

    // 等待最后一个命令完成
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
```

该函数处理管道命令的执行：

- 遍历管道中的每个命令
- 为每个命令配置适当的标准输入/输出
- 对于第一个命令，将标准输出重定向到管道
- 对于中间命令，从上一个命令接收输入并将输出发送到下一个命令
- 对于最后一个命令，从上一个命令接收输入并保持默认输出
- 等待最后一个命令执行完成

## 6. 功能测试

### 6.1 基本命令执行

- 显示当前工作目录：成功显示当前目录作为提示符
- 内置命令`cd`：可以成功切换目录
- 内置命令`ls`：可以列出目录内容，包括文件类型和名称
- 外部命令执行：可以执行系统中存在的命令
- 错误处理：对不存在的命令提供错误信息

### 6.2 管道机制测试

- 简单管道：`ls | grep rust` 可以执行
- 多级管道：`ls | grep r | wc -l` 可以执行
- 错误处理：管道中的命令错误能够被捕获并报告
