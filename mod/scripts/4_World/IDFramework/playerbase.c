modded class PlayerBase
{
    const int RPC_UPDATE_ID_VARS = 2124;

    protected string m_IDRole;
    protected string m_DiscordID;
	protected int m_IDIdentity = 0;
	
	override void Init()
	{
		super.Init();
		RegisterNetSyncVariableInt("m_IDIdentity", 0, 100000);
	}

	void SetIDIdentity(int id)
	{
		m_IDIdentity = id;
		SetSynchDirty();
	}

    void SetRole(string role, bool sync = false)
    {
        m_IDRole = role;
		if (sync)
		{
			RPCSingleParam(RPC_UPDATE_ID_VARS, new Param2<string, string>(m_DiscordID, m_IDRole), true);
		}
    }

    string GetRole()
    {
        return m_IDRole;
    }

    void SetDiscordID(string id, bool sync = false)
    {
        m_DiscordID = id;
		if (sync)
		{
			RPCSingleParam(RPC_UPDATE_ID_VARS, new Param2<string, string>(m_DiscordID, m_IDRole), true);
		}
    }

    string GetDiscordID()
    {
        return m_DiscordID;
    }
	
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx) {
        super.OnRPC(sender, rpc_type, ctx);
        Param2<string, string> data;
        if (GetGame().IsClient() && rpc_type == RPC_UPDATE_ID_VARS && ctx.Read(data))
        {
            m_DiscordID = data.param1;
			m_IDRole = data.param2;
        }
    }
}