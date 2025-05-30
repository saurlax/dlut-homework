mod graph_app;
mod graph_visualizer;

use eframe;
use graph_app::GraphApp;

fn main() -> Result<(), eframe::Error> {
    let options = eframe::NativeOptions {
        viewport: eframe::egui::ViewportBuilder::default().with_inner_size([800.0, 600.0]),
        ..Default::default()
    };

    eframe::run_native(
        "Graph Visualization",
        options,
        Box::new(|cc| Ok(Box::new(GraphApp::new(cc)))),
    )
}
