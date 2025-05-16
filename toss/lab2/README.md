# Lab2 - 图数据结构可视化

## 1. 实验说明

本实验要求：

- 阅读开源项目 egui_graphs
- 学习 petgraph 库中的图的 API
- 学习 egui_graphs 中的示例代码
- 基于 petgraph 和 egui 实现图数据结构的可视化呈现

## 2. 实验要求

### 2.1 学习内容

请阅读开源项目 egui_graphs，学习 petgraph 库中的图的 API、egui_graphs 中的示例代码，仔细阅读示例代码，对示例代码的阅读和理解，记录在 lab2 的实验报告中。

### 2.2 实现要求

对自己基于 petgraph 和 egui 实现的图数据结构可视化呈现的过程记录在 lab2 的实验报告中。

### 2.3 报告格式

lab2 的实验报告用 markdown 格式撰写。

## 3. 实验过程

### 3.1 理解 petgraph 库

记录学习 petgraph 库中图 API 的过程、理解和关键概念。

petgraph 是 Rust 中用于图数据结构操作的强大库，提供了多种图表示和算法：

#### 核心数据结构

- `Graph<N, E, Ty, Ix>` - 主要图类型，支持有向图和无向图
  - `N` - 节点权重类型
  - `E` - 边权重类型
  - `Ty` - 图类型（有向/无向）
  - `Ix` - 索引类型

#### 创建图

```rust
// 创建有向图
let mut graph = DiGraph::<String, f32>::new();

// 创建无向图
let mut graph = UnGraph::<String, f32>::new_undirected();
```

#### 添加节点和边

```rust
// 添加节点
let n1 = graph.add_node("节点1".to_string());
let n2 = graph.add_node("节点2".to_string());

// 添加边并设置权重
let edge = graph.add_edge(n1, n2, 5.0);
```

#### 图遍历

```rust
// 遍历所有节点
for node_id in graph.node_indices() {
    let weight = &graph[node_id];
    println!("节点 {:?}: {:?}", node_id, weight);
}

// 遍历所有边
for edge in graph.edge_indices() {
    let (source, target) = graph.edge_endpoints(edge).unwrap();
    let weight = graph[edge];
    println!("边 {:?} -> {:?}: {}", source, target, weight);
}
```

#### 图算法

petgraph 提供了多种内置算法：

- 最短路径（Dijkstra、A\*）
- 拓扑排序
- 强连通分量
- 环检测
- 最小生成树

### 3.2 分析 egui_graphs 示例代码

分析并理解 egui_graphs 中的示例代码实现，包括关键组件和实现方式。

egui_graphs 是将 petgraph 与 egui 结合的库，用于创建交互式图形可视化。主要组件包括：

#### 1. 核心数据结构

- `Graph<N, E, Ty, Ix>` - 封装了 petgraph 的图并添加了视觉属性
- `Node<N, E, Ty, Ix>` - 包含节点数据和视觉属性
- `Edge<N, E, Ty, Ix>` - 包含边数据和视觉属性

#### 2. 可视化组件

- `GraphView` - 提供图的交互式视图
- `SettingsStore` - 控制图的视觉呈现

#### 3. 交互功能

- 拖拽节点
- 选择节点和边
- 缩放和平移视图
- 动态添加/删除节点和边

#### 4. 布局算法

egui_graphs 支持多种布局算法：

- 力导向布局
- 分层布局
- 圆形布局
- 网格布局

通过研究示例代码，我们可以了解如何将图算法与交互式 UI 结合，创建直观的可视化工具。

### 3.3 实现自己的图可视化程序

本项目基于 petgraph 和 egui 实现了交互式图可视化，具体实现包括：

#### 设计考量

- **模块化设计**：将图数据结构、可视化和 UI 交互分离
- **类型安全**：利用 Rust 强类型系统确保操作安全
- **性能优化**：高效地渲染大型图结构

#### 实现挑战

1. **布局算法**：实现了力导向布局，需要处理节点间的物理模拟
2. **版本兼容性**：处理 petgraph 和 egui 不同版本之间的 API 差异
3. **用户交互**：实现直观的节点选择、边创建和图操作界面

#### 关键组件

1. **图可视化包装器**

```rust
struct GraphVisualizer<N: Debug, E: Display> {
    graph: Graph<N, E, petgraph::Directed>,
    node_visuals: HashMap<NodeIndex, NodeVisual>,
    next_pos: egui::Pos2,
    zoom: f32,
    pan_offset: egui::Vec2,
}
```

2. **力导向布局实现**

```rust
fn apply_force_layout(&mut self, iterations: usize) {
    // 计算节点间的斥力和边的引力
    // 迭代更新节点位置，使图结构美观分布
}
```

3. **图绘制和交互处理**

```rust
fn draw(&mut self, ui: &mut egui::Ui, selected_node: Option<NodeIndex>) -> Option<NodeIndex> {
    // 绘制边、箭头和节点
    // 处理选择和拖拽事件
}
```

## 4. 项目结构

```
lab2/
├── src/
│   └── main.rs      # 主程序代码
├── assets/          # 资源文件(如有)
├── Cargo.toml       # 项目依赖配置
└── README.md        # 本文档
```

## 5. 运行方式

使用以下命令运行项目：

```bash
cargo run --release
```

## 6. 功能说明

- **节点操作**：创建、选择和移动节点
- **边操作**：添加带权重的边，显示方向（对有向图）
- **图类型**：支持有向图和无向图
- **布局**：自动力导向布局，可手动调整
- **算法**：
  - Dijkstra 最短路径算法 - 计算并显示两点间最短路径
  - 环检测 - 检查图中是否存在环路
- **随机生成**：自动生成不同规模的随机图
- **交互控制**：
  - 拖拽节点调整位置
  - 缩放和平移视图
  - 可视化选择结果
