
class ActionEndTerminalSession: ActionInteractBase
{	
	void ActionEndTerminalSession()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_Text = "End Session";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		IE_IDTerminal_Base terminal = IE_IDTerminal_Base.Cast( target.GetObject() );
		return terminal != null && terminal.GetSessionPermissions().Count() > 0;
	}

	override void OnExecuteServer(ActionData action_data)
	{
		IE_IDTerminal_Base terminal = IE_IDTerminal_Base.Cast( action_data.m_Target.GetObject() );
		array<string> empty = new array<string>;
		terminal.SetSessionPermissions(empty, 0);
	}
};