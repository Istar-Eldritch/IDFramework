class IE_IdentityCard_Base: InventoryItemSuper
{
    protected int m_Identity = 0;
    protected string m_Name = "";

    const int RPC_UPDATE_NAME = 2123;
	
	override void InitItemVariables()
	{
		super.InitItemVariables();
		RegisterNetSyncVariableInt("m_Identity",0, 100000);
	}

    override void OnStoreSave(ParamsWriteContext ctx)
    {
        super.OnStoreSave(ctx);
        ctx.Write(m_Identity);
        ctx.Write(m_Name.Length());
		for(int i = 0; i < m_Name.Length(); i++)
		{
			ctx.Write(m_Name.Get(i).ToAscii());
		}
    }

    override bool OnStoreLoad (ParamsReadContext ctx, int version)
    {
        if (!super.OnStoreLoad(ctx, version))
            return false;
        
        if (!ctx.Read(m_Identity))
            return false;

		int strLen;
		if (!ctx.Read(strLen))
            return false;

		int strCh;
		m_Name = "";
		for(int i = 0; i < strLen; i++)
		{
			if (!ctx.Read(strCh))
				return false;
			m_Name += strCh.AsciiToString();
		}

		SetSynchDirty();
        RPCSingleParam(RPC_UPDATE_NAME, new Param1<string>(m_Name), true);
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
		output += "\n\nId No:  " + GetFormattedId() + "\nName:    " + m_Name;
		return true;
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx) {
        super.OnRPC(sender, rpc_type, ctx);
        Param1<string> data;
        if (GetGame().IsClient() && rpc_type == RPC_UPDATE_NAME && ctx.Read(data))
        {
            m_Name = data.param1;
        }
    }
}

class IE_IdentityCard_Default: IE_IdentityCard_Base
{

}