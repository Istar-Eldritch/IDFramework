modded class ActionConstructor
{

	override void RegisterActions(TTypenameArray actions)
	{
        super.RegisterActions(actions);
		actions.Insert(ActionStartTerminalSession);
        actions.Insert(ActionUpdateCard);
        actions.Insert(ActionEndTerminalSession);
	}
};