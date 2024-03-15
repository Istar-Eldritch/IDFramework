class ActionEndTerminalSession: ActionInteractBase {
	
	override string GetText() {
        return "End session";
	}

	override void CreateConditionComponents() 
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined( UAMaxDistances.DEFAULT );
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item ) {
		IE_IDTerminal_Base terminal = IE_IDTerminal_Base.Cast( target.GetObject() );
		return terminal != null && terminal.GetSessionPermissions().Count() > 0;
	}

    override void OnExecute( ActionData action_data ) {
		IE_IDTerminal_Base terminal = IE_IDTerminal_Base.Cast( action_data.m_Target.GetObject() );
		array<string> empty = new array<string>;
		terminal.SetSessionPermissions(empty, 0);
	}
}