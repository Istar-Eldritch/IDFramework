use actix_web::{post, web, App, HttpResponse, HttpServer, Responder};
use serde::{Deserialize, Serialize};
use serenity::model::guild::PartialGuild;
use serenity::model::id::GuildId;
use clap::Parser;

/// Simple program to greet a person
#[derive(Parser, Debug)]
#[command(version, about, long_about = None)]
struct Args {
    /// Name of the person to greet
    #[arg(short, long, env="DISCORD_TOKEN")]
    discord_token: String
}

#[derive(Clone)]
struct AppState {
    discord_token: String,
}

#[derive(Deserialize)]
struct GetPlayerInfo {
    guild_id: String,
    player_name: String
}

#[derive(Serialize)]
struct DZResponse {
    ok: bool,
    msg: String
}

#[derive(Serialize)]
struct PlayerInfo {
    name: String,
    roles: Vec<String>
}

#[derive(Serialize)]
struct PlayerInfoResponse
{
    ok: bool,
    player: PlayerInfo
}

#[post("/get-player-info")]
async fn handler(data: web::Data<AppState>, body: web::Json<GetPlayerInfo>) -> impl Responder {
    let discord = serenity::http::Http::new(&data.discord_token);
    let guild_id = GuildId::new(u64::from_str_radix(&body.guild_id, 10).unwrap());
    let guild: PartialGuild = match discord.get_guild(guild_id).await {
            Ok(guild) => guild,
            Err(err) => return HttpResponse::InternalServerError().json(DZResponse { ok: false, msg: format!("{:?}", err)})
    };

    let members = match discord.search_guild_members(guild_id, &body.player_name, Some(1)).await {
        Ok(members) => members,
        Err(err) => return HttpResponse::InternalServerError().json(DZResponse { ok: false, msg: format!("{:?}", err)})
    };

    if let Some(member) = members.get(0) {
        return HttpResponse::Ok().json(PlayerInfoResponse {
            ok: true,
            player: PlayerInfo {
                name: (member.nick.as_ref().unwrap_or(&member.user.name)).to_string(),
                roles: member.roles.iter().map(|id| guild.roles.get(&id).unwrap().name.to_string()).collect()
            }
        });
    }
    HttpResponse::Ok().json(DZResponse { ok: false, msg: "Non existing user".to_string()})

}

#[actix_web::main]
async fn main() -> std::io::Result<()> {

    let args = Args::parse();
    let data = AppState {
        discord_token: args.discord_token
    };

    HttpServer::new(move || {
        App::new()
            .app_data(web::Data::new(data.clone()))
            .service(handler)
    })
    .bind(("0.0.0.0", 80))?
    .run()
    .await
}