use eframe::egui;
use petgraph::{
    graph::{DiGraph, Graph, NodeIndex},
    visit::EdgeRef,
};
use rand::Rng;
use std::collections::HashMap;
use std::fmt::{Debug, Display};

struct NodeVisual {
    pos: egui::Pos2,
    dragging: bool,
}

struct GraphVisualizer<N: Debug, E: Display> {
    graph: Graph<N, E, petgraph::Directed>,
    node_visuals: HashMap<NodeIndex, NodeVisual>,
    next_pos: egui::Pos2,
    zoom: f32,
    pan_offset: egui::Vec2,
    is_directed: bool,
}

impl<N: Debug, E: Display> GraphVisualizer<N, E> {
    fn new() -> Self {
        Self {
            graph: DiGraph::new(),
            node_visuals: HashMap::new(),
            next_pos: egui::pos2(100.0, 100.0),
            zoom: 1.0,
            pan_offset: egui::Vec2::ZERO,
            is_directed: true,
        }
    }

    fn reset(&mut self, directed: bool) {
        self.graph = DiGraph::new();
        self.is_directed = directed;
        self.node_visuals.clear();
        self.next_pos = egui::pos2(100.0, 100.0);
    }

    fn add_node(&mut self, weight: N) -> NodeIndex {
        let node_idx = self.graph.add_node(weight);
        
        // Set initial position for the new node
        self.node_visuals.insert(
            node_idx,
            NodeVisual {
                pos: self.next_pos,
                dragging: false,
            },
        );
        
        // Update position for next node
        self.next_pos.x += 80.0;
        if self.next_pos.x > 800.0 {
            self.next_pos.x = 100.0;
            self.next_pos.y += 80.0;
        }
        
        node_idx
    }

    fn add_edge(&mut self, from: NodeIndex, to: NodeIndex, weight: E) -> Option<petgraph::graph::EdgeIndex> 
    where E: Clone {
        let edge = self.graph.add_edge(from, to, weight.clone());
        
        // For undirected graph, add reverse edge
        if (!self.is_directed) {
            self.graph.add_edge(to, from, weight);
        }
        
        Some(edge)
    }

    fn apply_force_layout(&mut self, iterations: usize) {
        // Force-directed layout algorithm constants
        const REPULSION: f32 = 5000.0;
        const ATTRACTION: f32 = 0.05;
        const DAMPING: f32 = 0.85;
        
        const MIN_X: f32 = 50.0;
        const MAX_X: f32 = 750.0;
        const MIN_Y: f32 = 50.0;
        const MAX_Y: f32 = 550.0;
        
        // Initialize velocities
        let mut velocities: HashMap<NodeIndex, egui::Vec2> = HashMap::new();
        for idx in self.graph.node_indices() {
            velocities.insert(idx, egui::Vec2::ZERO);
        }
        
        for _ in 0..iterations {
            // Calculate repulsive forces between all node pairs
            for i in self.graph.node_indices() {
                if !self.node_visuals.contains_key(&i) {
                    continue;
                }
                
                let mut force = egui::Vec2::ZERO;
                
                // Apply repulsive forces from other nodes
                for j in self.graph.node_indices() {
                    if i == j || !self.node_visuals.contains_key(&j) {
                        continue;
                    }
                    
                    let pos_i = self.node_visuals[&i].pos;
                    let pos_j = self.node_visuals[&j].pos;
                    
                    let diff = pos_i - pos_j;
                    let distance = diff.length().max(1.0);
                    
                    // Repulsion inversely proportional to distance squared
                    force += diff.normalized() * (REPULSION / (distance * distance));
                }
                
                // Apply attractive forces to connected nodes
                for edge in self.graph.edges(i) {
                    let neighbor = edge.target();
                    if !self.node_visuals.contains_key(&neighbor) {
                        continue;
                    }
                    
                    let pos_i = self.node_visuals[&i].pos;
                    let pos_n = self.node_visuals[&neighbor].pos;
                    
                    let diff = pos_n - pos_i;
                    let distance = diff.length().max(1.0);
                    
                    // Attraction proportional to distance
                    force += diff.normalized() * (distance * ATTRACTION);
                }
                
                // Update velocity with damping
                let velocity = velocities.get_mut(&i).unwrap();
                *velocity = (*velocity + force) * DAMPING;
                
                // Update position (if not being dragged)
                if let Some(visual) = self.node_visuals.get_mut(&i) {
                    if !visual.dragging {
                        visual.pos += *velocity;
                        
                        // Constrain within bounds
                        visual.pos.x = visual.pos.x.clamp(MIN_X, MAX_X);
                        visual.pos.y = visual.pos.y.clamp(MIN_Y, MAX_Y);
                    }
                }
            }
        }
    }

    fn draw(&mut self, ui: &mut egui::Ui, selected_node: Option<NodeIndex>) -> Option<NodeIndex> {
        let (response, painter) = ui.allocate_painter(ui.available_size(), egui::Sense::click_and_drag());
        let mut clicked_node = None;
        
        // Handle canvas panning
        if response.dragged() {
            self.pan_offset += response.drag_delta();
        }
        
        // Draw all edges first
        for edge in self.graph.edge_indices() {
            if let Some((source, target, weight)) = self.graph.edge_endpoints(edge).map(|(s, t)| {
                (s, t, self.graph.edge_weight(edge).unwrap())
            }) {
                if let (Some(src_visual), Some(tgt_visual)) = (
                    self.node_visuals.get(&source),
                    self.node_visuals.get(&target),
                ) {
                    let start_pos = src_visual.pos * self.zoom + self.pan_offset;
                    let end_pos = tgt_visual.pos * self.zoom + self.pan_offset;
                    
                    // Draw edge line
                    let stroke = egui::Stroke::new(2.0, egui::Color32::from_gray(150));
                    painter.line_segment([start_pos, end_pos], stroke);
                    
                    // Draw arrow for directed graph
                    if self.is_directed {
                        let direction = (end_pos - start_pos).normalized();
                        let normal = egui::vec2(-direction.y, direction.x);
                        let arrow_size = 10.0;
                        
                        // Calculate arrow tip position
                        let tip_pos = end_pos - direction * 15.0;
                        
                        // Draw arrow
                        painter.line_segment(
                            [tip_pos, tip_pos - direction * arrow_size + normal * arrow_size],
                            stroke,
                        );
                        painter.line_segment(
                            [tip_pos, tip_pos - direction * arrow_size - normal * arrow_size],
                            stroke,
                        );
                    }
                    
                    // Draw edge weight
                    let weight_pos = start_pos + (end_pos - start_pos) * 0.5;
                    painter.text(
                        weight_pos,
                        egui::Align2::CENTER_CENTER,
                        format!("{}", weight),
                        egui::FontId::default(),
                        egui::Color32::BLACK,
                    );
                }
            }
        }
        
        // Draw all nodes on top of edges
        for (idx, visual) in &mut self.node_visuals {
            let screen_pos = visual.pos * self.zoom + self.pan_offset;
            let node_radius = 20.0 * self.zoom;
            
            // Highlight selected node
            let is_selected = selected_node.map_or(false, |n| n == *idx);
            let node_color = if is_selected {
                egui::Color32::LIGHT_BLUE
            } else {
                egui::Color32::WHITE
            };
            
            // Draw node circle
            painter.circle(
                screen_pos,
                node_radius,
                node_color,
                egui::Stroke::new(2.0, egui::Color32::BLACK),
            );
            
            // Draw node label
            if let Some(weight) = self.graph.node_weight(*idx) {
                painter.text(
                    screen_pos,
                    egui::Align2::CENTER_CENTER,
                    format!("{:?}", weight),
                    egui::FontId::default(),
                    egui::Color32::BLACK,
                );
            }
            
            // Handle node dragging and selection
            let node_rect = egui::Rect::from_center_size(
                screen_pos,
                egui::vec2(node_radius * 2.0, node_radius * 2.0),
            );
            
            let node_response = ui.interact(node_rect, ui.id().with(*idx), egui::Sense::click_and_drag());
            
            if node_response.clicked() {
                clicked_node = Some(*idx);
            }
            
            if node_response.dragged() {
                visual.pos += node_response.drag_delta() / self.zoom;
                visual.dragging = true;
            } else {
                visual.dragging = false;
            }
        }
        
        clicked_node
    }
}

struct GraphApp {
    graph: GraphVisualizer<String, f32>,
    node_name: String,
    edge_weight: f32,
    selected_from: Option<NodeIndex>,
    selected_to: Option<NodeIndex>,
    is_directed: bool,
}

impl GraphApp {
    fn new(_cc: &eframe::CreationContext<'_>) -> Self {
        Self {
            graph: GraphVisualizer::new(),
            node_name: String::new(),
            edge_weight: 1.0,
            selected_from: None,
            selected_to: None,
            is_directed: true,
        }
    }

    fn reset_graph(&mut self) {
        self.graph.reset(self.is_directed);
        self.selected_from = None;
        self.selected_to = None;
    }

    fn create_random_graph(&mut self) {
        self.reset_graph();
        
        let mut rng = rand::thread_rng();
        
        // Create 5 nodes
        let mut indices = Vec::new();
        for i in 0..5 {
            let node_name = format!("Node {}", i+1);
            let idx = self.graph.add_node(node_name);
            indices.push(idx);
        }
        
        // Randomly create edges
        for &i in &indices {
            for &j in &indices {
                if i != j && rng.gen_bool(0.3) {
                    let weight = rng.gen_range(1.0..10.0);
                    self.graph.add_edge(i, j, weight);
                }
            }
        }
        
        // Apply force-directed layout
        self.graph.apply_force_layout(50);
    }
}

impl eframe::App for GraphApp {
    fn update(&mut self, ctx: &egui::Context, _frame: &mut eframe::Frame) {
        // Left control panel
        egui::SidePanel::left("control_panel").min_width(250.0).show(ctx, |ui| {
            ui.heading("Graph Control Panel");
            ui.separator();
            
            // Graph type control
            let old_directed = self.is_directed;
            ui.checkbox(&mut self.is_directed, "Directed Graph");
            if old_directed != self.is_directed && ui.button("Apply Graph Type").clicked() {
                self.reset_graph();
            }
            
            if ui.button("Reset Graph").clicked() {
                self.reset_graph();
            }
            
            if ui.button("Optimize Layout").clicked() {
                self.graph.apply_force_layout(50);
            }
            
            // Random graph generation
            ui.separator();
            ui.heading("Random Graph Generation");
            
            if ui.button("Generate 5-Node Graph").clicked() {
                self.create_random_graph();
            }
            
            // Add node controls
            ui.separator();
            ui.heading("Add Node");
            
            ui.horizontal(|ui| {
                ui.label("Node Name:");
                ui.text_edit_singleline(&mut self.node_name);
            });
            
            if ui.button("Add Node").clicked() && !self.node_name.is_empty() {
                self.graph.add_node(self.node_name.clone());
                self.node_name.clear();
            }
            
            // Add edge controls
            ui.separator();
            ui.heading("Add Edge");
            
            // Source node selection
            ui.label("Select Source Node:");
            egui::ComboBox::new("from_node", "Select Source Node")
                .selected_text(match self.selected_from {
                    Some(idx) => format!("{:?}", self.graph.graph.node_weight(idx)),
                    None => "Not Selected".to_string(),
                })
                .show_ui(ui, |ui| {
                    for node_idx in self.graph.graph.node_indices() {
                        ui.selectable_value(
                            &mut self.selected_from,
                            Some(node_idx),
                            format!("{:?}", self.graph.graph.node_weight(node_idx)),
                        );
                    }
                });
            
            // Target node selection
            ui.label("Select Target Node:");
            egui::ComboBox::new("to_node", "Select Target Node")
                .selected_text(match self.selected_to {
                    Some(idx) => format!("{:?}", self.graph.graph.node_weight(idx)),
                    None => "Not Selected".to_string(),
                })
                .show_ui(ui, |ui| {
                    for node_idx in self.graph.graph.node_indices() {
                        ui.selectable_value(
                            &mut self.selected_to,
                            Some(node_idx),
                            format!("{:?}", self.graph.graph.node_weight(node_idx)),
                        );
                    }
                });
                
            // Edge weight control
            ui.horizontal(|ui| {
                ui.label("Edge Weight:");
                ui.add(egui::Slider::new(&mut self.edge_weight, 1.0..=10.0));
            });
            
            if ui.button("Add Edge").clicked() {
                if let (Some(from), Some(to)) = (self.selected_from, self.selected_to) {
                    self.graph.add_edge(from, to, self.edge_weight);
                }
            }
        });
        
        // Main panel for graph visualization
        egui::CentralPanel::default().show(ctx, |ui| {
            // Draw graph and handle interactions
            if let Some(clicked_node) = self.graph.draw(ui, self.selected_from) {
                // Node selection logic
                if self.selected_from.is_none() {
                    self.selected_from = Some(clicked_node);
                } else if self.selected_to.is_none() && self.selected_from != Some(clicked_node) {
                    self.selected_to = Some(clicked_node);
                } else {
                    self.selected_from = Some(clicked_node);
                    self.selected_to = None;
                }
            }
            
            ui.allocate_rect(ui.available_rect_before_wrap(), egui::Sense::hover());
        });
    }
}

fn main() -> Result<(), eframe::Error> {
    let options = eframe::NativeOptions {
        viewport: egui::ViewportBuilder::default()
            .with_inner_size([1000.0, 700.0]),
        ..Default::default()
    };
    
    eframe::run_native(
        "Graph Visualization Demo",
        options,
        Box::new(|cc| Ok(Box::new(GraphApp::new(cc)))),
    )
}
