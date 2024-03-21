modded class MissionBase
{
    void MissionBase()
    {
        IE_ID_ConfigLoader configLoader = GetIDConfigLoader();
        GetRPCManager().AddRPC("IE_ID", "RequestConfigRPC", configLoader, SingleplayerExecutionType.Both);

        IE_ID_StateLoader stateLoader = GetIDStateLoader();
        GetRPCManager().AddRPC("IE_ID", "RequestStateRPC", stateLoader, SingleplayerExecutionType.Both);
        GetRPCManager().AddRPC("IE_ID", "RespondStateRPC", stateLoader, SingleplayerExecutionType.Both);

        if (GetGame().IsClient()) {
            configLoader.RequestConfiguration();
        }
    }

}

class PlayerInfo
{
	string name;
	ref array<string> roles;
	string discord_id;
}

class ListRolesCallback: RestCallback
{
	MissionServer m_mission;

	void ListRolesCallback(MissionServer mission)
	{
		m_mission = mission;
	}

	override void OnError(int errorCode)
	{
		Print("[ID Framework] Error requesting roles from discord: " + errorCode);
		m_mission.OnDiscordRoles(null);
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
			m_mission.OnDiscordRoles(null);
			return;
		}

		m_mission.OnDiscordRoles(roleResponse);
	}
}


class SearchGuildMemberCallback: RestCallback
{
	PlayerBase m_player;
	MissionServer m_mission;

	void SearchGuildMemberCallback(PlayerBase player, MissionServer mission)
	{
		m_player = player;
		m_mission = mission;
	}

	override void OnError(int errorCode)
	{
		Print("[ID Framework] Error searching for member from discord: " + errorCode);
		m_mission.OnDiscordIdentity(m_player, null);
	}
	
	override void OnSuccess(string data, int dataSize)
	{
		JsonSerializer serializer = new JsonSerializer;
		string error;
		array<SearchGuildMemberMemberResponse> memberResponse = new array<SearchGuildMemberMemberResponse>;
		serializer.ReadFromString(memberResponse, data, error);
		
		foreach(auto member: memberResponse)
		{
			array<string> roles = new array<string>;
			foreach(string roleId: member.roles)
			{
				foreach(auto role: m_mission.m_DiscordRoles)
				{
					if (role.id == roleId)
					{
						roles.Insert(role.name);
						break;
					}
				}
			}
			PlayerInfo info = new PlayerInfo;
			info.name = member.nick;
			info.roles = roles;
			info.discord_id = member.user.id;
			m_mission.OnDiscordIdentity(m_player, info);
			break;
		}
	}
}

modded class MissionServer
{
	ref DiscordAPI m_api;
	bool m_DiscordInitialized = false;
	bool m_newPlayer = false;
	ref array<ref ListGuildRolesRoleResponse> m_DiscordRoles;

	void MissionServer()
	{
		IE_ID_Config config = GetIDConfig();
		m_api = new DiscordAPI(config.proxyUrl, config.proxyToken, config.discordBotToken);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(RequestListRoles, 10000);
	}
	
	void OnDiscordRoles(array<ref ListGuildRolesRoleResponse> roles)
	{
		if (roles != null)
		{
			m_DiscordRoles = roles;
		}
		else
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(RequestListRoles, 10000);
		}
	}
	
	void RequestListRoles()
	{
		IE_ID_Config config = GetIDConfig();
		m_api.ListGuildRoles(config.discordGuildID, new ListRolesCallback(this));
	}

	void OnDiscordIdentity(PlayerBase player, PlayerInfo playerInfo)
	{
		if (playerInfo != null && playerInfo.name == player.GetIdentity().GetName())
		{
			
			auto config = GetIDConfig();
			bool hasRole = false;
			foreach (auto role: config.roles)
			{
				foreach(auto roleName: playerInfo.roles)
				{
					if (role.name == roleName)
					{
						auto state = GetIDStateLoader();
						auto oldDiscordId = state.GetIdentity(player.GetIdentity().GetId());
						auto newDiscordId = state.CreateOrUpdateIdentity(player, roleName, playerInfo.discord_id);
						bool hasNewId = oldDiscordId == null || oldDiscordId.id != newDiscordId.id || oldDiscordId.name != newDiscordId.name;
						if (m_newPlayer || hasNewId)
						{
							int spawnCount = role.spawnpoints.Count();
							int spawnIdx = Math.RandomInt(0, spawnCount);
							array<float> pos = role.spawnpoints.Get(spawnIdx);
							vector v = "0 0 0";
							v[0] = pos[0];
							v[1] = pos[1];
							v[2] = pos[2];

							DeveloperTeleport.SetPlayerPosition(player, v);

							array<EntityAI> items = new array<EntityAI>;
							player.GetInventory().EnumerateInventory(InventoryTraversalType.POSTORDER, items);
							foreach (auto item: items)
							{
								if (item.GetType() != player.GetType())
								{
									player.GetInventory().LocalDestroyEntity(item);
								}
							}
							foreach (string cls: role.loadout)
							{
								CreateItemOnPlayerInventory(player, cls);
							}
							
							if(role.spawns_with_card && role.card != "")
							{
								ItemBase cardItem = CreateItemOnPlayerInventory(player, role.card);
								IE_IdentityCard_Base card = IE_IdentityCard_Base.Cast(cardItem);
								card.EvaluateOwner(player);
							}

							m_DiscordInitialized = true;
							m_newPlayer = false;
						}
						return;
					}
				}
			}
		}

		KickPlayer(player);
	}
	
	ItemBase CreateItemOnPlayerInventory(PlayerBase player, string cls)
	{
		ItemBase item = ItemBase.Cast(player.GetInventory().CreateInInventory(cls));

		item.SetSynchDirty();
		
		return item;

	}
	

	
	void KickPlayer(PlayerBase player)
	{
		if (!GetGame() || !player)
			return;

		auto missionServer = MissionServer.Cast(GetGame().GetMission());

		if (!missionServer)
			return;

		GetGame().SendLogoutTime(player, 0);

		missionServer.PlayerDisconnected(player, player.GetIdentity(), player.GetIdentity().GetId());
	}

	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player, identity);
		IE_ID_Config config = GetIDConfig();
		m_api.SearchGuildMember(config.discordGuildID, 1, identity.GetName(), new SearchGuildMemberCallback(player, this));
	}

	override PlayerBase OnClientNewEvent(PlayerIdentity identity, vector pos, ParamsReadContext ctx)
	{
		super.OnClientNewEvent(identity, pos, ctx);
		m_newPlayer = true;
		return m_player;
	}
}