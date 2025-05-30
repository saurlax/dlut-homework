use anyhow::{Context, Error, Result};
use serde::{Deserialize, Serialize};
use surrealdb::engine::local::{Db, Mem};
use surrealdb::sql::{thing, Datetime, Thing, Uuid};
use surrealdb::Surreal;

lazy_static::lazy_static! {
    pub static ref DB: async_once::AsyncOnce<Surreal<Db>> =
        async_once::AsyncOnce::new(connect_db_async());
}

fn connect_db_async() -> impl std::future::Future<Output = Surreal<Db>> {
    async { connect_db().await.expect("Unable to connect to database") }
}

async fn connect_db() -> Result<Surreal<Db>, Box<dyn std::error::Error>> {
    let db = Surreal::new::<Mem>(()).await?;
    db.use_ns("rag").use_db("content").await?;
    Ok(db)
}

#[derive(Serialize, Deserialize, Debug, Clone)]
pub struct Content {
    pub id: Thing,
    pub content: String,
    pub vector: Vec<f32>,
    pub created_at: Datetime,
}

pub async fn retrieve(query: &str) -> Result<Vec<Content>, Error> {
    let embeddings: Vec<f32> = crate::embeddings::get_embeddings(&query)?
        .reshape((384,))?
        .to_vec1()?;
    let db = DB.get().await.clone();
    let mut result = db
        .query("SELECT *, vector::similarity::cosine(vector, $query) AS score FROM vector_index ORDER BY score DESC LIMIT 4")
        .bind(("query", embeddings))
        .await?;
    let vector_indexes: Vec<Content> = result.take(0)?;
    Ok(vector_indexes)
}

pub async fn insert(content: &str) -> Result<Content, Error> {
    let db = DB.get().await.clone();
    let id = Uuid::new_v4().0.to_string().replace("-", "");
    let id = thing(format!("vector_index:{}", id).as_str())?;
    let vector = crate::embeddings::get_embeddings(&content)?
        .reshape((384,))?
        .to_vec1()?;
    let vector_index: Content = db
        .create(("vector_index", id.clone()))
        .content(Content {
            id: id.clone(),
            content: content.to_string(),
            vector,
            created_at: Datetime::default(),
        })
        .await?
        .context("Unable to insert vector index")?;
    Ok(vector_index)
}
