class ActionStartTerminalSession: ActionSingleUseBase {
	
	const string ENABLE_TERMINAL_PERMISSION = "TerminalSession";
	
	override string GetText() {
        return "Start session";
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

		return terminal != null && card != null && terminal.GetSessionPermissions().Count() == 0 && HasPermission(card.GetPermissions(), ENABLE_TERMINAL_PERMISSION);
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

    override void OnExecute( ActionData action_data ) {
		IE_IDTerminal_Base terminal = IE_IDTerminal_Base.Cast( action_data.m_Target.GetObject() );
		IE_IdentityCard_Base card = IE_IdentityCard_Base.Cast( action_data.m_MainItem );
		
		auto config = GetIDConfig();
		terminal.SetSessionPermissions(card.GetPermissions(), config.terminal_session_duration * 1000);
	}
}