use eframe::egui;
use petgraph::{
    graph::{DiGraph, Graph, NodeIndex},
    visit::EdgeRef,
};
use std::collections::HashMap;
use std::fmt::{Debug, Display};

pub struct NodeVisual {
    pub pos: egui::Pos2,
    pub dragging: bool,
}

pub struct GraphVisualizer<N: Debug, E: Display> {
    pub graph: Graph<N, E, petgraph::Directed>,
    pub node_visuals: HashMap<NodeIndex, NodeVisual>,
    next_pos: egui::Pos2,
    zoom: f32,
    pan_offset: egui::Vec2,
    is_directed: bool,
}

impl<N: Debug, E: Display> GraphVisualizer<N, E> {
    pub fn new() -> Self {
        Self {
            graph: DiGraph::new(),
            node_visuals: HashMap::new(),
            next_pos: egui::pos2(100.0, 100.0),
            zoom: 1.0,
            pan_offset: egui::Vec2::ZERO,
            is_directed: true,
        }
    }

    pub fn reset(&mut self, directed: bool) {
        self.graph = DiGraph::new();
        self.is_directed = directed;
        self.node_visuals.clear();
        self.next_pos = egui::pos2(100.0, 100.0);
    }

    pub fn add_node(&mut self, weight: N) -> NodeIndex {
        let node_idx = self.graph.add_node(weight);
        
        self.node_visuals.insert(
            node_idx,
            NodeVisual {
                pos: self.next_pos,
                dragging: false,
            },
        );
        
        self.next_pos.x += 80.0;
        if self.next_pos.x > 800.0 {
            self.next_pos.x = 100.0;
            self.next_pos.y += 80.0;
        }
        
        node_idx
    }

    pub fn add_edge(&mut self, from: NodeIndex, to: NodeIndex, weight: E) -> Option<petgraph::graph::EdgeIndex> 
    where E: Clone {
        let edge = self.graph.add_edge(from, to, weight.clone());
        
        if !self.is_directed {
            self.graph.add_edge(to, from, weight);
        }
        
        Some(edge)
    }

    pub fn apply_force_layout(&mut self, iterations: usize) {
        // Force-directed layout algorithm constants
        const REPULSION: f32 = 5000.0;
        const ATTRACTION: f32 = 0.05;
        const DAMPING: f32 = 0.85;
        
        const MIN_X: f32 = 50.0;
        const MAX_X: f32 = 750.0;
        const MIN_Y: f32 = 50.0;
        const MAX_Y: f32 = 550.0;
        
        let mut velocities: HashMap<NodeIndex, egui::Vec2> = HashMap::new();
        for idx in self.graph.node_indices() {
            velocities.insert(idx, egui::Vec2::ZERO);
        }
        
        for _ in 0..iterations {
            for i in self.graph.node_indices() {
                if !self.node_visuals.contains_key(&i) {
                    continue;
                }
                
                let mut force = egui::Vec2::ZERO;
                
                for j in self.graph.node_indices() {
                    if i == j || !self.node_visuals.contains_key(&j) {
                        continue;
                    }
                    
                    let pos_i = self.node_visuals[&i].pos;
                    let pos_j = self.node_visuals[&j].pos;
                    
                    let diff = pos_i - pos_j;
                    let distance = diff.length().max(1.0);
                    
                    force += diff.normalized() * (REPULSION / (distance * distance));
                }
                
                for edge in self.graph.edges(i) {
                    let neighbor = edge.target();
                    if !self.node_visuals.contains_key(&neighbor) {
                        continue;
                    }
                    
                    let pos_i = self.node_visuals[&i].pos;
                    let pos_n = self.node_visuals[&neighbor].pos;
                    
                    let diff = pos_n - pos_i;
                    let distance = diff.length().max(1.0);
                    
                    force += diff.normalized() * (distance * ATTRACTION);
                }
                
                let velocity = velocities.get_mut(&i).unwrap();
                *velocity = (*velocity + force) * DAMPING;
                
                if let Some(visual) = self.node_visuals.get_mut(&i) {
                    if !visual.dragging {
                        visual.pos += *velocity;
                        
                        visual.pos.x = visual.pos.x.clamp(MIN_X, MAX_X);
                        visual.pos.y = visual.pos.y.clamp(MIN_Y, MAX_Y);
                    }
                }
            }
        }
    }

    pub fn draw(&mut self, ui: &mut egui::Ui, selected_node: Option<NodeIndex>) -> Option<NodeIndex> {
        let (response, painter) = ui.allocate_painter(ui.available_size(), egui::Sense::click_and_drag());
        let mut clicked_node = None;
        
        if response.dragged() {
            self.pan_offset += response.drag_delta();
        }
        
        // Draw edges
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
                    
                    let stroke = egui::Stroke::new(2.0, egui::Color32::from_gray(150));
                    painter.line_segment([start_pos, end_pos], stroke);
                    
                    if self.is_directed {
                        let direction = (end_pos - start_pos).normalized();
                        let normal = egui::vec2(-direction.y, direction.x);
                        let arrow_size = 10.0;
                        
                        let tip_pos = end_pos - direction * 15.0;
                        
                        painter.line_segment(
                            [tip_pos, tip_pos - direction * arrow_size + normal * arrow_size],
                            stroke,
                        );
                        painter.line_segment(
                            [tip_pos, tip_pos - direction * arrow_size - normal * arrow_size],
                            stroke,
                        );
                    }
                    
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
        
        // Draw nodes
        for (idx, visual) in &mut self.node_visuals {
            let screen_pos = visual.pos * self.zoom + self.pan_offset;
            let node_radius = 20.0 * self.zoom;
            
            let is_selected = selected_node.map_or(false, |n| n == *idx);
            let node_color = if is_selected {
                egui::Color32::LIGHT_BLUE
            } else {
                egui::Color32::WHITE
            };
            
            painter.circle(
                screen_pos,
                node_radius,
                node_color,
                egui::Stroke::new(2.0, egui::Color32::BLACK),
            );
            
            if let Some(weight) = self.graph.node_weight(*idx) {
                painter.text(
                    screen_pos,
                    egui::Align2::CENTER_CENTER,
                    format!("{:?}", weight),
                    egui::FontId::default(),
                    egui::Color32::BLACK,
                );
            }
            
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
