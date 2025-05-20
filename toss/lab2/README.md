# Lab2 - 图数据结构可视化

20232241392-软2305-文浩然

## 1. 示例代码理解

在学习 egui_graphs 和 petgraph 的过程中，我发现它们提供了强大的图数据结构操作和可视化功能。petgraph 库提供了高效的图结构实现，支持有向图和无向图，以及多种图算法如 Dijkstra 最短路径、拓扑排序和环检测等。egui_graphs 则巧妙地将 petgraph 与 egui 结合，提供了交互式图形可视化的能力。

通过分析示例代码，我了解到图可视化的核心在于将抽象的数据结构转换为直观的视觉元素。egui_graphs 使用了 Graph、Node 和 Edge 等核心数据结构来存储图数据及其视觉属性，同时提供了 GraphView 和 SettingsStore 等组件来控制图的呈现方式。其布局算法（如力导向布局）能够自动计算节点位置，使图结构清晰美观。交互功能如拖拽、选择和缩放则增强了用户体验，使图形可视化更加直观有效。

## 2. 项目介绍

本项目是基于 petgraph 和 egui 实现的交互式图数据结构可视化工具。不同于直接使用 egui_graphs 库，我们从零开始实现了图的渲染和交互系统，以更好地理解图可视化的底层原理。

项目提供了以下主要功能：

- 创建和编辑图（添加/移动节点，添加带权重的边）
- 支持有向图和无向图
- 自动力导向布局算法，优化图的视觉展示
- 实现图算法可视化（Dijkstra 最短路径和环检测）
- 随机生成不同规模的图
- 完整的交互控制（拖拽节点，选择元素）

## 3. 代码分析

### 3.1 核心数据结构

图可视化的核心是`GraphVisualizer`结构体，它封装了图数据和视觉属性：

```rust
struct GraphVisualizer<N: Debug, E: Display> {
    graph: Graph<N, E, petgraph::Directed>,
    node_visuals: HashMap<NodeIndex, NodeVisual>,
    next_pos: egui::Pos2,
    zoom: f32,
    pan_offset: egui::Vec2,
}
```

每个节点的可视化属性由`NodeVisual`管理：

```rust
struct NodeVisual {
    pos: egui::Pos2,
    dragging: bool,
}
```

### 3.2 力导向布局算法实现

力导向布局算法是图可视化的核心，以下是实现细节：

```rust
fn apply_force_layout(&mut self, iterations: usize) {
    // 定义物理模拟常量
    const REPULSION: f32 = 5000.0;
    const ATTRACTION: f32 = 0.05;
    const DAMPING: f32 = 0.85;

    // 初始化节点速度
    let mut velocities: HashMap<NodeIndex, egui::Vec2> = HashMap::new();
    for idx in self.graph.node_indices() {
        velocities.insert(idx, egui::Vec2::ZERO);
    }

    // 迭代模拟物理作用力
    for _ in 0..iterations {
        // 计算节点间斥力
        for i in self.graph.node_indices() {
            // ...计算与其他节点的斥力...

            // 计算与相连节点的引力
            for edge in self.graph.edges(i) {
                // ...计算边的引力...
            }

            // 根据力更新节点速度和位置
            // ...更新节点位置...
        }
    }
}
```

此算法模拟了节点间的物理作用力，使图布局更加自然美观：

- 所有节点之间产生斥力，避免节点重叠
- 相连节点之间产生引力，使相连节点靠近
- 使用阻尼系数避免布局振荡

### 3.3 交互式绘制与操作

图的绘制和交互通过`draw`方法实现：

```rust
fn draw(&mut self, ui: &mut egui::Ui, selected_node: Option<NodeIndex>) -> Option<NodeIndex> {
    // 分配绘图区域
    let (response, painter) = ui.allocate_painter(ui.available_size(), egui::Sense::click_and_drag());
    let mut clicked_node = None;

    // 处理画布拖拽
    if response.dragged() {
        self.pan_offset += response.drag_delta();
    }

    // 先绘制所有边
    for edge in self.graph.edge_indices() {
        // ...绘制边、箭头和权重...
    }

    // 再绘制所有节点
    for (idx, visual) in &mut self.node_visuals {
        // ...绘制节点和文本...

        // 处理节点选择和拖拽
        let node_response = ui.interact(node_rect, ui.id().with(*idx), egui::Sense::click_and_drag());

        if node_response.clicked() {
            clicked_node = Some(*idx);
        }

        if node_response.dragged() {
            visual.pos += node_response.drag_delta() / self.zoom;
            visual.dragging = true;
        }
    }

    clicked_node
}
```

这个方法实现了:

- 绘制边和节点的视觉表示
- 处理节点选择和拖拽
- 处理整个画布的平移

### 3.4 算法实现

以 Dijkstra 最短路径算法为例：

```rust
fn run_dijkstra(&mut self) {
    if let (Some(start), Some(end)) = (self.selected_from, self.selected_to) {
        // 使用petgraph的dijkstra算法
        let node_map = dijkstra(&self.graph.graph, start, Some(end), |e| *e.weight());

        // 处理计算结果
        if let Some(distance) = node_map.get(&end) {
            self.algorithm_result = format!("Shortest path from {:?} to {:?}: {}",
                self.graph.graph.node_weight(start),
                self.graph.graph.node_weight(end),
                distance);
        } else {
            self.algorithm_result = "No path found!".to_string();
        }
    }
}
```

这个方法演示了如何利用 petgraph 的算法 API，并将结果可视化呈现给用户。

### 3.5 用户界面集成

应用的主界面使用 egui 的布局系统组织：

```rust
fn update(&mut self, ctx: &egui::Context, _frame: &mut eframe::Frame) {
    // 左侧控制面板
    egui::SidePanel::left("control_panel").show(ctx, |ui| {
        // ...控制按钮和表单...
    });

    // 中央图形面板
    egui::CentralPanel::default().show(ctx, |ui| {
        // 绘制图并处理交互
        if let Some(clicked_node) = self.graph.draw(ui, self.selected_from) {
            // ...处理节点选择逻辑...
        }
    });
}
```
