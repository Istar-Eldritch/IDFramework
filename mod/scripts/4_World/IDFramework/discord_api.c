class GetPlayerInfo
{
	string guild_id;
	string player_name;

	void GetPlayerInfo(string guildId, string player)
	{
		guild_id = guildId;
		player_name = player;
	}
}

class DiscordAPI
{
	void RequestRoles(string playerName, RestCallback cb)
	{
		auto config = GetIDConfig();
		string guildId = config.discordGuidID;
		string url = config.discordBotURL;
		auto data = new GetPlayerInfo(guildId, playerName);
		JsonSerializer js = new JsonSerializer();
		string body;
		js.WriteToString(data, false, body);

		RestContext ctx = GetRestApi().GetRestContext(url);
		ctx.SetHeader( "application/json" );
		ctx.POST(cb, "get-player-info", body);
	}
}