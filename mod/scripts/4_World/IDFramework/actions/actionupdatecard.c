class ActionUpdateCard: ActionSingleUseBase {
	
	const string UPDATE_CARD_PERMISSION = "UpdateCard";
	
	protected PlayerBase m_cardPlayer;

	override string GetText() {
        return "Update card";
	}

	override void CreateConditionComponents() 
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined( UAMaxDistances.DEFAULT );
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ATTACHITEM;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item ) {
		IE_IDTerminal_Base terminal = IE_IDTerminal_Base.Cast( target.GetObject() );
        IE_IdentityCard_Base card = IE_IdentityCard_Base.Cast( item );

		if ( terminal != null && card != null && HasPermission(terminal.GetSessionPermissions(), UPDATE_CARD_PERMISSION)) {
			
			IE_ID_Config config = GetIDConfig();
			auto state = GetIDStateLoader();
			m_cardPlayer = card.GetPlayerBase();
			if (m_cardPlayer)
			{
				return card.GetRoleInCard() != m_cardPlayer.GetRole();
			}
		}

		return false;
	}
	
	bool HasPermission(array<string> permissions, string permission)
	{
		foreach(string p: permissions)
		{
			if (p == permission)
				return true;
		}
		return false;
	}

    override void OnExecuteServer( ActionData action_data ) {

		string role = m_cardPlayer.GetRole();
		auto config = GetIDConfigLoader();
		auto roleconfig = config.GetRoleByName(role);
		auto inventory = HumanInventory.Cast(action_data.m_Player.GetInventory());
		if (inventory.ReplaceItemInHandsWithNew(InventoryMode.PREDICTIVE, new ReplaceItemWithNewLambda(action_data.m_MainItem, roleconfig.card, action_data.m_Player)));
		{
			IE_IdentityCard_Base card = IE_IdentityCard_Base.Cast(inventory.GetEntityInHands());
			if (card)
			{
				Print(m_cardPlayer);
				card.EvaluateOwner(m_cardPlayer);
			}
		}
	}
}