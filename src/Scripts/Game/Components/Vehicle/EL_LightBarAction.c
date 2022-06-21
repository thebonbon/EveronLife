class EL_LightBarAction : ScriptedUserAction
{

	EL_LightBarComponent m_LightBar;
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		if (!pOwnerEntity || !pUserEntity)
			return;
		
		IEntity child = pOwnerEntity.GetChildren();
		while(child)
		{
			EL_LightBarComponent childLightBar = EL_LightBarComponent.Cast(child.FindComponent(EL_LightBarComponent));
			if (childLightBar)
				childLightBar.ToggleLights();
			child = child.GetSibling();
		}
		GetActionName();
	}
	
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		string prefix = "";
		UIInfo actionInfo = GetUIInfo();
		if(actionInfo)
			prefix = actionInfo.GetName() + " ";

		if (m_LightBar && m_LightBar.b_IsActive)
			outName = prefix + "Off";
		else
			outName = prefix + "On";

		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		IEntity child = pOwnerEntity.GetChildren();
		while(child)
		{
			EL_LightBarComponent childLightBar = EL_LightBarComponent.Cast(child.FindComponent(EL_LightBarComponent));
			if (childLightBar)
			{
				m_LightBar = childLightBar;
				return;
			}
			child = child.GetSibling();
		}
	}
}
