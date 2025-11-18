use eframe::egui;
use petgraph::graph::NodeIndex;
use rand::Rng;
use crate::graph_visualizer::GraphVisualizer;

pub struct GraphApp {
    pub graph: GraphVisualizer<String, f32>,
    pub node_name: String,
    pub edge_weight: f32,
    pub selected_from: Option<NodeIndex>,
    pub selected_to: Option<NodeIndex>,
    pub is_directed: bool,
}

impl GraphApp {
    pub fn new(_cc: &eframe::CreationContext<'_>) -> Self {
        Self {
            graph: GraphVisualizer::new(),
            node_name: String::new(),
            edge_weight: 1.0,
            selected_from: None,
            selected_to: None,
            is_directed: true,
        }
    }

    pub fn reset_graph(&mut self) {
        self.graph.reset(self.is_directed);
        self.selected_from = None;
        self.selected_to = None;
    }

    pub fn create_random_graph(&mut self) {
        self.reset_graph();
        
        let mut rng = rand::rng();
        
        let mut indices = Vec::new();
        for i in 0..5 {
            let node_name = format!("Node {}", i+1);
            let idx = self.graph.add_node(node_name);
            indices.push(idx);
        }
        
        for &i in &indices {
            for &j in &indices {
                if i != j && rng.random_bool(0.3) {
                    let weight = rng.random_range(1.0..10.0);
                    self.graph.add_edge(i, j, weight);
                }
            }
        }
        
        self.graph.apply_force_layout(50);
    }
}

impl eframe::App for GraphApp {
    fn update(&mut self, ctx: &egui::Context, _frame: &mut eframe::Frame) {
        egui::SidePanel::left("control_panel").min_width(250.0).show(ctx, |ui| {
            ui.heading("Graph Control Panel");
            ui.separator();
            
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
            
            // Zoom controls
            ui.separator();
            ui.heading("View Controls");
            
            ui.horizontal(|ui| {
                if ui.button("Zoom In").clicked() {
                    self.graph.zoom_in();
                }
                if ui.button("Zoom Out").clicked() {
                    self.graph.zoom_out();
                }
                if ui.button("Reset View").clicked() {
                    self.graph.reset_view();
                }
            });
            
            ui.label(format!("Zoom: {:.1}%", self.graph.get_zoom() * 100.0));
            ui.label("Tip: Use mouse wheel to zoom");
            
            ui.separator();
            ui.heading("Random Graph Generation");
            
            if ui.button("Generate 5-Node Graph").clicked() {
                self.create_random_graph();
            }
            
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
            
            ui.separator();
            ui.heading("Add Edge");
            
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
        
        egui::CentralPanel::default().show(ctx, |ui| {
            if let Some(clicked_node) = self.graph.draw(ui, self.selected_from) {
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
