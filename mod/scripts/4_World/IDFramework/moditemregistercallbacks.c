modded class ModItemRegisterCallbacks
{
	override void RegisterOneHanded(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior)
	{
		super.RegisterOneHanded(pType, pBehavior);
    	pType.AddItemInHandsProfileIK("IE_IdentityCard_Base","dz/anims/workspaces/player/player_main/player_main_1h.asi",pBehavior,"dz/anims/anm/player/ik/gear/Map_chernarus.anm");
		pType.AddItemInHandsProfileIK("IE_IdentityCard_Default","dz/anims/workspaces/player/player_main/player_main_1h.asi",pBehavior,"dz/anims/anm/player/ik/gear/Map_chernarus.anm");
    }
}
