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
}

class PlayerInfoResponse
{
	bool ok;
	ref PlayerInfo player;	
}

class RolesCallback: RestCallback
{
	PlayerBase m_player;

	void RolesCallback(PlayerBase player)
	{
		m_player = player;
	}
	
	void KickPlayer()
	{
		if (!GetGame() || !m_player)
			return;

		auto missionServer = MissionServer.Cast(GetGame().GetMission());

		if (!missionServer)
			return;

		GetGame().SendLogoutTime(m_player, 0);

		missionServer.PlayerDisconnected(m_player, m_player.GetIdentity(), m_player.GetIdentity().GetId());
	}

	override void OnError(int errorCode)
	{
		KickPlayer();
	}
	
	override void OnSuccess(string data, int dataSize)
	{
		JsonSerializer serializer = new JsonSerializer;
		string error;
		PlayerInfoResponse playerInfo;
		serializer.ReadFromString(playerInfo, data, error);
		if (!playerInfo.ok || playerInfo.player.name != m_player.GetIdentity().GetName())
		{
			KickPlayer();
		}
		else
		{
			m_player.SetRoles(playerInfo.player.roles);
		}
	}
}

modded class MissionServer
{
	ref DiscordAPI m_api;
	void MissionServer()
	{
		m_api = new DiscordAPI();
	}


	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player, identity);
		m_api.RequestRoles(identity.GetName(), new RolesCallback(player));
	}
	
}