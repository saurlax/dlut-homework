# chatbot

## Setup

```bash
cargo run
```

## 项目报告

按照实验手册的要求使用 SurrealDB 的 RocksDB 后端作为本地知识库存储。然而，在 Windows 环境下编译时遇到了严重的依赖编译问题。具体表现为 zstd-sys 库在编译过程中出现大量的链接错误，特别是 libclang 相关的错误信息："Unable to find libclang: couldn't find any valid shared libraries matching: ['clang.dll', 'libclang.dll']"。

经过分析发现，RocksDB 后端需要编译大量的原生 C++代码，而 Windows 环境下的构建工具链配置复杂，容易出现兼容性问题。为了解决这一问题，我将数据库后端从 `kv-rocksdb` 迁移到了 `kv-mem` 内存数据库。这种迁移避免了原生代码编译的复杂性，虽然数据不会持久化到磁盘，但对于实验验证和演示目的来说是充分的。修改后的配置在 Cargo.toml 中体现为：`surrealdb = { version = "1.0.0", features = ["kv-mem"] }`。

项目需要从 HuggingFace Hub 下载预训练模型，包括 BERT 嵌入模型（BAAI/bge-small-en-v1.5）和 Phi-2 语言模型（Demonthos/dolphin-2_6-phi-2-candle）。在国内网络环境下，直接访问 HuggingFace 经常出现连接超时或下载中断的问题。

为了解决网络访问问题，我配置了 TUN 模式的全局代理。TUN 代理通过创建虚拟网络接口，在网络层面拦截所有流量并通过代理服务器转发，相比传统的 HTTP 代理具有更好的兼容性和稳定性。通过 TUN 代理，成功解决了模型下载过程中的网络连接问题，确保了项目依赖的机器学习模型能够正常获取。

在集成 Candle 机器学习框架的过程中，发现了一些 API 变化导致的编译错误。主要包括：

- BERT 模型的 forward 方法需要传入额外的 attention_mask 参数
- VarBuilder 的 from_gguf 方法增加了 Device 参数要求
- SurrealDB 的引擎导入路径发生了变化

通过仔细阅读最新的 API 文档和源代码，逐一修复了这些兼容性问题，确保项目能够正确编译和运行。

最终实现的聊天机器人支持两个主要功能：

1. `remember`命令：将用户提供的内容存储到本地知识库中，同时生成对应的文本嵌入向量
2. `ask`命令：根据用户查询从知识库中检索相关内容，并结合大语言模型生成相应的回答
