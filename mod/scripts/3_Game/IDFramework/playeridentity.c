
modded class PlayerIdentity
{
	ref DiscordAPI m_api;
	ref PlayerInfo m_PlayerInfo;
	ref array<ref ListGuildRolesRoleResponse> m_DiscordRoles;
	
	void Init()
	{
		IE_ID_Config config = GetIDConfig();
		m_api = new DiscordAPI(config.proxyUrl, config.proxyToken, config.discordBotToken);
		RequestRoles();
	}

    PlayerInfo GetPlayerInfo()
    {
        return m_PlayerInfo;
    }
	
	protected void RequestRoles()
	{
		IE_ID_Config config = GetIDConfig();
		m_api.ListGuildRoles(config.discordGuildID, new ListRolesCallback(this));
	}
	
	void OnDiscordRoles(array<ref ListGuildRolesRoleResponse> roles)
	{
		if (roles != null)
		{
			m_DiscordRoles = roles;
			RequestMember();
		}
		else {
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(RequestRoles);
		}
	}
	
	protected void RequestMember()
	{
		IE_ID_Config config = GetIDConfig();
		m_api.SearchGuildMember(config.discordGuildID, 1, GetName(), new SearchGuildMemberCallback(this));	
	}

	void OnDiscordMembers(array<SearchGuildMemberMemberResponse> memberResponse)
	{
		IE_ID_Config config = GetIDConfig();
		if (memberResponse)
		{
			foreach(auto member: memberResponse)
			{
				foreach(string roleId: member.roles)
				{
					foreach(auto discordRole: m_DiscordRoles)
					{
						if (discordRole.id == roleId)
						{
							foreach (auto configRole: config.roles)
							{
								if (configRole.name == discordRole.name)
								{
									PlayerInfo info = new PlayerInfo;
									info.name = member.nick;
									info.role = configRole;
									info.discord_id = member.user.id;
									m_PlayerInfo = info;
									return;
								}
							}
						}
					}
				}
			}
		}
		else
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(RequestMember);
		}
	}
}

class ListRolesCallback: RestCallback
{
	PlayerIdentity m_Identity;

	void ListRolesCallback(PlayerIdentity identity)
	{
		m_Identity = identity;
	}

	override void OnError(int errorCode)
	{
		Print("[ID Framework] Error requesting roles from discord: " + errorCode);
		m_Identity.OnDiscordRoles(null);
	}
	
	override void OnSuccess(string data, int dataSize)
	{
		
		JsonSerializer serializer = new JsonSerializer;
		string error;
		array<ref ListGuildRolesRoleResponse> roleResponse = new array<ref ListGuildRolesRoleResponse>;
		serializer.ReadFromString(roleResponse, data, error);
		if (error)
		{
			Print("[ID Framework] Error parsing roles response from discord: " + error);
			m_Identity.OnDiscordRoles(null);
			return;
		}

		m_Identity.OnDiscordRoles(roleResponse);
	}
}


class SearchGuildMemberCallback: RestCallback
{
	PlayerIdentity m_Identity;

	void SearchGuildMemberCallback(PlayerIdentity identity)
	{
		m_Identity = identity;
	}

	override void OnError(int errorCode)
	{
		Print("[ID Framework] Error searching for member from discord: " + errorCode);
		m_Identity.OnDiscordMembers(null);
	}
	
	override void OnSuccess(string data, int dataSize)
	{
		JsonSerializer serializer = new JsonSerializer;
		string error;
		array<SearchGuildMemberMemberResponse> memberResponse = new array<SearchGuildMemberMemberResponse>;
		serializer.ReadFromString(memberResponse, data, error);
		if (error)
		{
			Print("[IDFramework] Error parsing discord member response: " + error);
			m_Identity.OnDiscordMembers(null);
		}
		else
		{
			m_Identity.OnDiscordMembers(memberResponse);
		}
	}
}


class PlayerInfo
{
	string name;
	ref IE_ID_RoleConfig role;
	string discord_id;
}