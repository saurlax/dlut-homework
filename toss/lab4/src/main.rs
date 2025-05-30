use clap::Parser;
mod cli;
mod database;
mod embeddings;
mod llm;

#[tokio::main]
async fn main() -> anyhow::Result<()> {
    // 初始化日志
    tracing_subscriber::fmt::init();
    
    println!("Chacha AI Assistant Starting...");
    let args = cli::Cli::parse();
    
    match args.command {
        cli::Commands::Ask { query } => {
            println!("Processing query: {}", query);
            
            // 从知识库检索相关内容
            match database::retrieve(&query).await {
                Ok(references) => {
                    println!("Found {} references", references.len());
                    
                    // 使用LLM生成答案
                    match llm::answer_with_context(&query, references).await {
                        Ok(answer) => println!("Answer: {}", answer),
                        Err(e) => {
                            println!("Failed to generate answer: {}", e);
                            println!("Providing simple response...");
                            println!("Answer: I'm sorry, I couldn't process your question properly at the moment.");
                        }
                    }
                },
                Err(e) => {
                    println!("Failed to retrieve from database: {}", e);
                    println!("Answer: I don't have any relevant information stored yet. Try adding some content first with the 'remember' command.");
                }
            }
        },
        cli::Commands::Remember { content } => {
            println!("Storing content: {}", content);
            
            // 将内容存储到知识库
            match database::insert(&content).await {
                Ok(_) => println!("Content remembered successfully!"),
                Err(e) => println!("Failed to store content: {}", e),
            }
        }
    }
    
    Ok(())
}
