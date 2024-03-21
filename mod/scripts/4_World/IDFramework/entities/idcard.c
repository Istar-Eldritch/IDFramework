class IE_IdentityCard_Base: InventoryItemSuper
{
    protected int m_Identity = 0;
    protected string m_Name = "";
	protected string m_Role = "";
	protected string m_PlayerId = "";
	protected ref array<string> m_Permissions = new array<string>;

    const int RPC_UPDATE_TEXT = 2123;
    const int RPC_REQUEST_TEXT = 2126;
	
	override void InitItemVariables()
	{
		super.InitItemVariables();
		RegisterNetSyncVariableInt("m_Identity",0, 100000);
	}
	
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
		RPCSingleParam(RPC_REQUEST_TEXT, null, true);
	}

	int GetIdInCard()
	{
		return m_Identity;
	}
	
	string GetRoleInCard()
	{
		return m_Role;
	}
	
	string GetPlayerId()
	{
		return m_PlayerId;
	}
	
	string GetNameIncard()
	{
		return m_Name;
	}
	
	array<string> GetPermissions()
	{
		return m_Permissions;
	}
	
	PlayerBase GetPlayerBase()
	{
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		foreach (Man cp: players)
		{
			PlayerBase p = PlayerBase.Cast(cp);
			if (p.GetIdentity().GetId() == GetPlayerId())
			{
				return p;
			}
		}
		return null;
	}

	/// This logic is used to initialize cards that are spawned on players inventories.
	void EvaluateOwner(PlayerBase player)
	{
		auto state = GetIDStateLoader();
		m_PlayerId = player.GetIdentity().GetId();
		auto identity = state.GetIdentity(m_PlayerId);
		m_Identity = identity.id;
		m_Name = identity.name;
		m_Role = identity.role;
		auto config = GetIDConfigLoader();
		auto role = config.GetRoleByName(m_Role);
		m_Permissions = role.permissions;
		SetSynchDirty();
	}

    override void OnStoreSave(ParamsWriteContext ctx)
    {
        super.OnStoreSave(ctx);
        ctx.Write(m_Identity);
        ctx.Write(m_Name.Length());
		int i;
		for(i = 0; i < m_Name.Length(); i++)
		{
			ctx.Write(m_Name.Get(i).ToAscii());
		}
        ctx.Write(m_Role.Length());
		for(i = 0; i < m_Role.Length(); i++)
		{
			ctx.Write(m_Role.Get(i).ToAscii());
		}
        ctx.Write(m_PlayerId.Length());
		for(i = 0; i < m_PlayerId.Length(); i++)
		{
			ctx.Write(m_PlayerId.Get(i).ToAscii());
		}
		ctx.Write(m_Permissions.Count());
		int j;
		string permission;
		for(i=0; i < m_Permissions.Count(); i++)
		{
			permission = m_Permissions.Get(i);
			ctx.Write(permission.Length());
			for(j = 0; j < permission.Length(); j++)
			{
				ctx.Write(permission.Get(j).ToAscii());
			}
		}
    }
	
	override void AfterStoreLoad()
	{
		if (GetGame().IsServer())
			RPCSingleParam(RPC_UPDATE_TEXT, new Param4<string, string, string, array<string>>(m_Name, m_Role, m_PlayerId, m_Permissions), true);
	}

    override bool OnStoreLoad (ParamsReadContext ctx, int version)
    {
        if (!super.OnStoreLoad(ctx, version))
            return false;
        
        if (!ctx.Read(m_Identity))
            return false;

		int i;
		int strCh;

		int strLen;
		if (!ctx.Read(strLen))
            return false;

		m_Name = "";
		for(i = 0; i < strLen; i++)
		{
			if (!ctx.Read(strCh))
				return false;
			m_Name += strCh.AsciiToString();
		}

		if (!ctx.Read(strLen))
            return false;

		m_Role = "";
		for(i = 0; i < strLen; i++)
		{
			if (!ctx.Read(strCh))
				return false;
			m_Role += strCh.AsciiToString();
		}

		if (!ctx.Read(strLen))
            return false;

		m_PlayerId = "";
		for(i = 0; i < strLen; i++)
		{
			if (!ctx.Read(strCh))
				return false;
			m_PlayerId += strCh.AsciiToString();
		}
		
		m_Permissions = new array<string>;
		int permCount;
		int j;
		string permission;
		if (!ctx.Read(permCount))
			return false;

		for(i = 0; i < permCount; i++)
		{
			if (!ctx.Read(strLen))
	            return false;
	
			permission = "";
			for(j = 0; j < strLen; j++)
			{
				if (!ctx.Read(strCh))
					return false;
				permission += strCh.AsciiToString();
			}
			m_Permissions.Insert(permission);
		}

        return true;
    }

    string GetFormattedId()
    {
        string sIdentity = m_Identity.ToString();
        string pad = "";
        for(int i = 0; i < 5 - sIdentity.Length(); i++)
        {
            pad += "0";
        }
        return pad + sIdentity;
    }
	
	override bool DescriptionOverride(out string output)
	{
		output = ConfigGetString("descriptionShort");
		output += "\n\nId No:  " + GetFormattedId() + "\nName:  " + m_Name + "\nClass:  " + m_Role;
		return true;
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx) {
        super.OnRPC(sender, rpc_type, ctx);
        Param4<string, string, string, array<string>> data;
        if (GetGame().IsClient() && rpc_type == RPC_UPDATE_TEXT && ctx.Read(data))
        {
            m_Name = data.param1;
			m_Role = data.param2;
			m_PlayerId = data.param3;
			m_Permissions = data.param4;
        }
		else if (GetGame().IsServer() && rpc_type == RPC_REQUEST_TEXT)
		{
			RPCSingleParam(RPC_UPDATE_TEXT, new Param4<string, string, string, array<string>>(m_Name, m_Role, m_PlayerId, m_Permissions), true, sender);
		}
    }
	
	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionUpdateCard);
		AddAction(ActionStartTerminalSession);
	}
}

class IE_IdentityCard_White: IE_IdentityCard_Base { }
class IE_IdentityCard_Blue: IE_IdentityCard_Base { }
class IE_IdentityCard_Orange: IE_IdentityCard_Base { }
class IE_IdentityCard_Green: IE_IdentityCard_Base { }