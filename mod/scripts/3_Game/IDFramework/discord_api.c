class ProxyRequest
{
	string token;
	string url;
	string method;
	ref array<array<string>> headers = new array<array<string>>;
	ref array<array<string>> query_parameters = new array<array<string>>;
	string body;
}

class DiscordAPI
{
	const string DISCORD_URL = "https://discord.com/api/v10";

	protected string m_proxyURL;
	protected string m_proxyToken;
	protected string m_discordToken;

	void DiscordAPI(string proxyUrl, string proxyToken, string botToken)
	{
		m_proxyURL = proxyUrl;
		m_proxyToken = proxyToken;
		m_discordToken = botToken;
	}

	void SearchGuildMember(string guildId, int limit, string playerName, RestCallback cb)
	{
		ProxyRequest request = new ProxyRequest;
		request.token = m_proxyToken;
		request.url = DISCORD_URL + "/guilds/" + guildId + "/members/search";
		request.method = "GET";
		array<string> authHeader = {"authorization", "Bot " + m_discordToken};
		request.headers = { authHeader };
		array<string> limitParam = {"limit", "" + limit};
		array<string> queryParam = {"query", playerName};
		request.query_parameters = {limitParam, queryParam};

		JsonSerializer js = new JsonSerializer();
		string body;
		js.WriteToString(request, false, body);

		RestContext ctx = GetRestApi().GetRestContext(m_proxyURL);
		ctx.SetHeader("application/json");
		ctx.POST(cb, "/", body);
	}
	
	array<SearchGuildMemberMemberResponse> SearchGuildMember_now(string guildId, int limit, string playerName)
	{
		ProxyRequest request = new ProxyRequest;
		request.token = m_proxyToken;
		request.url = DISCORD_URL + "/guilds/" + guildId + "/members/search";
		request.method = "GET";
		array<string> authHeader = {"authorization", "Bot " + m_discordToken};
		request.headers = { authHeader };
		array<string> limitParam = {"limit", "" + limit};
		array<string> queryParam = {"query", playerName};
		request.query_parameters = {limitParam, queryParam};

		JsonSerializer js = new JsonSerializer();
		string body;
		js.WriteToString(request, false, body);

		RestContext ctx = GetRestApi().GetRestContext(m_proxyURL);
		ctx.SetHeader("application/json");
		string data = ctx.POST_now("/", body);
		array<SearchGuildMemberMemberResponse> memberResponse = new array<SearchGuildMemberMemberResponse>;
		string error = "";
		JsonSerializer serializer = new JsonSerializer;
		serializer.ReadFromString(memberResponse, data, error);
		if (error)
			Print("[IDFramework] Error requesting SearchGuildMember: "  + error);
			return null;
		return memberResponse;
		
	}

	void ListGuildRoles(string guildId, RestCallback cb)
	{
		ProxyRequest request = new ProxyRequest;
		request.token = m_proxyToken;
		request.url = DISCORD_URL + "/guilds/" + guildId + "/roles";
		request.method = "GET";
		array<string> authHeader = {"authorization", "Bot " + m_discordToken};
		request.headers = {
			authHeader
		};

		JsonSerializer js = new JsonSerializer();
		string body;
		js.WriteToString(request, false, body);

		RestContext ctx = GetRestApi().GetRestContext(m_proxyURL);
		ctx.SetHeader("application/json");
		ctx.POST(cb, "/", body);
	}
	
	array<ListGuildRolesRoleResponse> ListGuildRoles_now(string guildId)
	{
		ProxyRequest request = new ProxyRequest;
		request.token = m_proxyToken;
		request.url = DISCORD_URL + "/guilds/" + guildId + "/roles";
		request.method = "GET";
		array<string> authHeader = {"authorization", "Bot " + m_discordToken};
		request.headers = {
			authHeader
		};

		JsonSerializer js = new JsonSerializer();
		string body;
		js.WriteToString(request, false, body);

		RestContext ctx = GetRestApi().GetRestContext(m_proxyURL);
		ctx.SetHeader("application/json");
		string data = ctx.POST_now("/", body);
		
		JsonSerializer serializer = new JsonSerializer;
		string error;
		array<ListGuildRolesRoleResponse> guildResponse = new array<ListGuildRolesRoleResponse>;
		serializer.ReadFromString(guildResponse, data, error);
		
		if (error)
			Print("[IDFramework] Error requesting ListGuildRoles: "  + error);
			return null;
		return guildResponse;
	}
}

class ListGuildRolesRoleResponse {
	string id;
	string name;
	string description;
	string permissions;
	int position;
	int color;
	bool hoist;
	bool managed;
	bool mentionable;
	string icon;
	string unicode_emoji;
	int flags;
}

class SearchGuildMemberMemberResponse {
	string avatar;
	string communication_disabled_until;
	int flags;
	string joined_at;
	string nick;
	bool pending;
	string premium_since;
	array<string> roles;
	string unusual_dm_activity_until;
	bool mute;
	bool deaf;
	SearchGuildMemberMemberUser user;
}

class SearchGuildMemberMemberUser {
	string id;
	string username;
	string avatar;
	string discriminator;
	int public_flags;
	int flags;
	string banner;
	string accent_color;
	string global_name;
	string avatar_decorator_data;
}