class IE_IDTerminal_Base: ItemBase
{
	const int RPC_UPDATE_PERMISSIONS = 2125;
	ref array<string> m_Permissions = new array<string>;

    void IE_IDTerminal_Base()
    {
        RegisterNetSyncVariableBool("m_IsPlaceSound");
    }

    override void SetActions()
	{
		super.SetActions();
		AddAction(ActionTogglePlaceObject);
		AddAction(ActionPlaceObject);
        AddAction(ActionEndTerminalSession);
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
		
		if (IsPlaceSound())
		{
			PlayPlaceSound();
		}
	}
	
	void SetSessionPermissions(array<string> permissions, int timeout) {
		m_Permissions = permissions;
		RPCSingleParam(RPC_UPDATE_PERMISSIONS, new Param1<array<string>>(m_Permissions), true);

		if (timeout > 0)
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SetSessionPermissions, timeout, false, new array<string>, 0);
	}
	
	array<string> GetSessionPermissions()
	{
		return m_Permissions;
	}

	override void OnPlacementComplete(Man player, vector position = "0 0 0", vector orientation = "0 0 0")
	{
		super.OnPlacementComplete(player, position, orientation);
		SetIsPlaceSound(true);
	}

	override string GetPlaceSoundset()
	{
		return "placePowerGenerator_SoundSet";
	}
	
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx) {
        super.OnRPC(sender, rpc_type, ctx);
        Param1<array<string>> data;
        if (GetGame().IsClient() && rpc_type == RPC_UPDATE_PERMISSIONS && ctx.Read(data))
        {
            m_Permissions = data.param1;
        }
    }
}