use actix_web::body::MessageBody;
use actix_web::{post, web, App, HttpServer, Responder, HttpResponse, http::{header::ContentType, StatusCode}};
use serde::Deserialize;
use clap::Parser;
use reqwest::{Client, header::{HeaderMap, HeaderName, HeaderValue}};
use derive_more::{Display, Error};

#[derive(Parser, Debug)]
#[command(version, about, long_about = None)]
struct Args {
    /// Name of the person to greet
    #[arg(short, long, env="AUTH_TOKEN")]
    auth_token: String,

    /// Name of the person to greet
    #[arg(short, long, env="LOG_LEVEL", default_value="dayz_proxy=info")]
    log_level: String
}

#[derive(Clone)]
struct AppState {
    auth_token: String,
    http_client: Client,
}

#[derive(Deserialize)]
struct ProxyRequest {
    token: String,
    url: String,
    method: String,
    headers: Vec<(String, String)>,
    query_parameters: Vec<(String, String)>,
    body: Option<String>
}


#[derive(Debug, Display, Error)]
enum ProxyError {
    #[display(fmt = "internal error")]
    InternalError,

    #[display(fmt = "unauthorized")]
    Unauthorized
}

impl actix_web::error::ResponseError for ProxyError {
    fn error_response(&self) -> HttpResponse {
        HttpResponse::build(self.status_code())
            .insert_header(ContentType::plaintext())
            .body(self.to_string())
    }

    fn status_code(&self) -> StatusCode {
        match *self {
            ProxyError::InternalError => StatusCode::INTERNAL_SERVER_ERROR,
            ProxyError::Unauthorized => StatusCode::UNAUTHORIZED
        }
    }
}

#[post("/")]
async fn handler(data: web::Data<AppState>, body: web::Json<ProxyRequest>) -> Result<impl Responder, ProxyError> {
    if body.token != data.auth_token {
        return Err(ProxyError::Unauthorized)
    }
    let method = &body.method.clone().try_into_bytes().map_err(|e| {
            log::error!("Error parsing method: {:?}", e);
            ProxyError::InternalError
        })?;

    let method = reqwest::Method::from_bytes(method).map_err(|e| {
        log::error!("Error parsing method from bytes: {:?}", e);
        ProxyError::InternalError
    })?;

    let mut reqbuilder = data.http_client.clone().request(method, &body.url);
    let mut headers = HeaderMap::new();

    for (k, v) in body.headers.iter() {
        let header_name = HeaderName::from_lowercase(k.as_bytes())
            .map_err(|e| {
                log::error!("Error parsing header name: {} - {:?}", k, e);
                ProxyError::InternalError
            })?;
        let header_value = HeaderValue::from_str(v)
        .map_err(|e| {
            log::error!("Error parsing header value: {:?}", e);
            ProxyError::InternalError
        })?;
        headers.insert(header_name, header_value);
    }

    reqbuilder = reqbuilder.headers(headers);
    reqbuilder = reqbuilder.query(&body.query_parameters);

    if let Some(body) = &body.body {
        reqbuilder = reqbuilder.body(body.to_string());
    }

    let response = reqbuilder.send().await
        .map_err(|e| {
            log::error!("Error sending request: {:?}", e);
            ProxyError::InternalError
        })?;

    let res_headers = response.headers();
    let mut response_builder = HttpResponse::build(response.status());
    
    for header in res_headers.iter() {
        response_builder.insert_header(header);
    }

    let res_body = response.bytes().await
    .map_err(|e| {
        log::error!("Error parsing response: {:?}", e);
        ProxyError::InternalError
    })?;

    Ok(response_builder.body(res_body))

}

#[actix_web::main]
async fn main() -> std::io::Result<()> {

    let args = Args::parse();
    let data = AppState {
        auth_token: args.auth_token,
        http_client: Client::new()
    };

    std::env::set_var("RUST_LOG", args.log_level);
    env_logger::init();

    log::info!("Starting proxy");

    HttpServer::new(move || {
        App::new()
            .app_data(web::Data::new(data.clone()))
            .service(handler)
    })
    .bind(("0.0.0.0", 80))?
    .run()
    .await
}