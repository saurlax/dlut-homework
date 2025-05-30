// Adopted from https://github.com/huggingface/candle/blob/96f1a28e390fceeaa12b3272c8ac5dc ccc8eb5fa/candle-examples/examples/phi/main.rs
use anyhow::{Error as E, Result};
use candle_core::{DType, Device, Tensor};
use candle_transformers::generation::LogitsProcessor;
use candle_transformers::models::quantized_mixformer::Config;
use candle_transformers::models::quantized_mixformer::MixFormerSequentialForCausalLM as QMixFormer;
use hf_hub::{api::sync::Api, Repo};
use lazy_static::lazy_static;
use serde_json::json;
use tokenizers::Tokenizer;
use crate::database::Content;

lazy_static! {
    pub static ref PHI: (QMixFormer, Tokenizer) =
        load_model().expect("Unable to load model");
}

pub fn load_model() -> Result<(QMixFormer, Tokenizer)> {
    let api = Api::new()?.repo(Repo::model("Demonthos/dolphin-2_6-phi-2-candle".to_string()));
    let tokenizer_filename = api.get("tokenizer.json")?;
    let weights_filename = api.get("model-q4k.gguf")?;
    let tokenizer = Tokenizer::from_file(tokenizer_filename).map_err(E::msg)?;
    let config = Config::v2();
    let device = Device::Cpu;
    let vb = candle_transformers::quantized_var_builder::VarBuilder::from_gguf(&weights_filename, &device)?;
    let model = QMixFormer::new_v2(&config, vb)?;
    Ok((model, tokenizer))
}

struct TextGeneration {
    model: QMixFormer,
    device: Device,
    tokenizer: Tokenizer,
    logits_processor: LogitsProcessor,
    repeat_penalty: f32,
    repeat_last_n: usize,
}

impl TextGeneration {
    #[allow(clippy::too_many_arguments)]
    fn new(
        model: QMixFormer,
        tokenizer: Tokenizer,
        seed: u64,
        temp: Option<f64>,
        top_p: Option<f64>,
        repeat_penalty: f32,
        repeat_last_n: usize,
        device: &Device,
    ) -> Self {
        let logits_processor = LogitsProcessor::new(seed, temp, top_p);
        Self {
            model,
            tokenizer,
            logits_processor,
            repeat_penalty,
            repeat_last_n,
            device: device.clone(),
        }
    }

    fn run(&mut self, prompt: &str, sample_len: usize) -> Result<String> {
        // Encode the prompt into tokens
        let tokens = self.tokenizer.encode(prompt, true).map_err(E::msg)?;
        let mut tokens = tokens.get_ids().to_vec();
        let eos_token = match self.tokenizer.get_vocab(true).get("<|im_end|>") {
            Some(token) => *token,
            None => anyhow::bail!("cannot find the endoftext token"),
        };

        // Loop over the sample length to generate the response
        let mut response = String::new();
        for index in 0..sample_len {
            // Get the context for the current iteration
            let context_size = if index > 0 { 1 } else { tokens.len() };
            let ctxt = &tokens[tokens.len().saturating_sub(context_size)..];
            let input = Tensor::new(ctxt, &self.device)?.unsqueeze(0)?;
            
            // Run the model forward pass
            let logits = self.model.forward(&input)?;
            let logits = logits.squeeze(0)?.to_dtype(DType::F32)?;
            let start_at = tokens.len().saturating_sub(self.repeat_last_n);
            
            // Apply the repetition penalty
            let logits = candle_transformers::utils::apply_repeat_penalty(
                &logits,
                self.repeat_penalty,
                &tokens[start_at..],
            )?;
            
            // Sample the next token
            let next_token = self.logits_processor.sample(&logits)?;
            tokens.push(next_token);
            
            // Check if the generated token is the endoftext token
            if next_token == eos_token {
                break;
            }
            
            let token = self
                .tokenizer
                .decode(&[next_token], true)
                .map_err(E::msg)?;
            response += &token;
        }
        Ok(response.trim().to_string())
    }
}

pub async fn answer_with_context(query: &str, references: Vec<Content>) -> Result<String> {
    // Create the context for the prompt
    let mut context = Vec::new();
    for reference in references.clone() {
        context.push(json!({"content": reference.content}))
    }
    let context = json!(context).to_string();
    
    // Create the prompt
    let prompt = format!(
        "<|im_start|>system\nAs a friendly and helpful AI assistant named Chacha. Your answer should be very concise and to the point. Do not repeat question or references.\n<|im_end|>\n<|im_start|>user\nquestion: \"{question}\"\nreferences: \"{context}\"\n<|im_end|>\n<|im_start|>assistant\n",
        context = context,
        question = query
    );
    
    let (model, tokenizer) = &*PHI;
    let mut pipeline = TextGeneration::new(
        model.clone(),
        tokenizer.clone(),
        398752958,
        Some(0.3),
        None,
        1.1,
        64,
        &Device::Cpu,
    );
    let response = pipeline.run(&prompt, 400)?;
    Ok(response)
}
