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

modded class MissionServer
{
	
	ItemBase CreateItemOnPlayerInventory(PlayerBase player, string cls)
	{
		ItemBase item = ItemBase.Cast(player.GetInventory().CreateInInventory(cls));

		item.SetSynchDirty();
		
		return item;

	}

	override void OnClientPrepareEvent(PlayerIdentity identity, out bool useDB, out vector pos, out float yaw, out int preloadTimeout)
	{
		super.OnClientPrepareEvent(identity, useDB, pos, yaw, preloadTimeout);
		identity.Init();
	}
	
	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player, identity);
		PlayerInfo playerInfo = identity.GetPlayerInfo();
		if (playerInfo == null || playerInfo.name != identity.GetName())
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(IEKickPlayer, 0, false, player);
		}
		else
		{
			auto state = GetIDStateLoader();
			auto discordState = state.GetIdentity(identity.GetId());
			if (discordState)
			{
				player.SetIDIdentity(discordState.id);
				player.SetDiscordID(playerInfo.discord_id);
				player.SetRole(playerInfo.role.name, true);
			}
		}
	}
	
	private void IEKickPlayer(PlayerBase player)
	{
		if (!GetGame() || !player)
			return;

		auto missionServer = MissionServer.Cast(GetGame().GetMission());

		if (!missionServer)
			return;

		GetGame().SendLogoutTime(player, 0);

		missionServer.PlayerDisconnected(player, player.GetIdentity(), player.GetIdentity().GetId());

	}

	override PlayerBase OnClientNewEvent(PlayerIdentity identity, vector pos, ParamsReadContext ctx)
	{
		string characterType = GetGame().CreateRandomPlayer();
		
		// get login data for new character
		if (ProcessLoginData(ctx) && (m_RespawnMode == GameConstants.RESPAWN_MODE_CUSTOM) && !GetGame().GetMenuDefaultCharacterData(false).IsRandomCharacterForced())
		{
			if (GetGame().ListAvailableCharacters().Find(GetGame().GetMenuDefaultCharacterData().GetCharacterType()) > -1)
				characterType = GetGame().GetMenuDefaultCharacterData().GetCharacterType();
		}

		CreateIDCharacter(identity, pos, ctx, characterType);
		
		return m_player;
	}
	
	PlayerBase CreateIDCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
	{
		IE_ID_Config config = GetIDConfig();
		PlayerInfo playerInfo = identity.GetPlayerInfo();
		Entity playerEnt;
		if (playerInfo != null)
		{
			auto state = GetIDStateLoader();
			auto oldDiscordId = state.GetIdentity(identity.GetId());
			auto newDiscordId = state.CreateOrUpdateIdentity(identity.GetName(), identity.GetId(), playerInfo.role.name, playerInfo.discord_id);
			int spawnCount = playerInfo.role.spawnpoints.Count();
			int spawnIdx = Math.RandomInt(0, spawnCount);
			array<float> spawnpoint = playerInfo.role.spawnpoints.Get(spawnIdx);
			vector playerPos = "0 0 0";
			playerPos[0] = spawnpoint[0];
			playerPos[1] = spawnpoint[1];
			playerPos[2] = spawnpoint[2];

			playerEnt = GetGame().CreatePlayer(identity, characterName, playerPos, 0, "NONE");
			Class.CastTo(m_player, playerEnt);
			m_player.SetIDIdentity(newDiscordId.id);
			m_player.SetDiscordID(playerInfo.discord_id);
			m_player.SetRole(playerInfo.role.name, true);
			GetGame().SelectPlayer(identity, m_player);

			foreach (string cls: playerInfo.role.loadout)
			{
				CreateItemOnPlayerInventory(m_player, cls);
			}
			
			if(playerInfo.role.spawns_with_card && playerInfo.role.card != "")
			{
				ItemBase cardItem = CreateItemOnPlayerInventory(m_player, playerInfo.role.card);
				IE_IdentityCard_Base card = IE_IdentityCard_Base.Cast(cardItem);
				card.EvaluateOwner(m_player);
			}
		}
		else
		{
			playerEnt = GetGame().CreatePlayer(identity, characterName, pos, 0, "NONE");
			Class.CastTo(m_player, playerEnt);
		}

		return m_player;
	}
}