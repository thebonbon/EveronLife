[ComponentEditorProps(category: "EveronLife/Feature/Chat")]
class EL_ChatManagerComponentClass : ScriptComponentClass
{
}

class EL_ChatManagerComponent: ScriptComponent
{
	bool m_bHideChat;
	
	//------------------------------------------------------------------------------------------------
	// Chat related methods are here, as we assume any menu can have it's chat panel and therefore needs to implement these.
	void ToggleChat(bool active)
	{
		SCR_ChatPanelManager chatManager = SCR_ChatPanelManager.GetInstance();
		if (!chatManager)
			return;
		chatManager.HideAllChatPanels();
	}
	
	//------------------------------------------------------------------------------------------------
	void LateInit()
	{
		ToggleChat(false)
	}
	
	//------------------------------------------------------------------------------------------------
	override void EOnInit(IEntity owner)
	{
		GetGame().GetCallqueue().CallLater(LateInit, 1000, false);
	}	
		
    
    //------------------------------------------------------------------------------------------------
    override void OnPostInit(IEntity owner)
    {
        SetEventMask(owner, EntityEvent.INIT | EntityEvent.FRAME);
        owner.SetFlags(EntityFlags.ACTIVE, true);
    }
}
