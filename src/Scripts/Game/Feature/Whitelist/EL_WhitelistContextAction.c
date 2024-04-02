/*[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class EL_EnableWhitelistContextAction : SCR_BaseContextAction
{
	[Attribute("", UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(EL_WhitelistType))]
	EL_WhitelistType m_eType;

	//------------------------------------------------------------------------------------------------
	override bool CanBeShown(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{
		return hoveredEntity.GetEntityType() == EEditableEntityType.CHARACTER;
	}
	
	//------------------------------------------------------------------------------------------------	
	override bool CanBePerformed(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override void Perform(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags, int param = -1)
	{
		EL_WhitelistRespawnHandlerComponent whitelistComponent = EL_WhitelistRespawnHandlerComponent.Cast(GetGame().GetGameMode().FindComponent(EL_WhitelistRespawnHandlerComponent));
		whitelistComponent.EnableWhitelistOnPlayer(EL_Utils.GetPlayerUID(hoveredEntity.GetOwner()), m_eType);
	}
}

[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class EL_DisableWhitelistContextAction : SCR_BaseContextAction
{
	[Attribute("", UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(EL_WhitelistType))]
	EL_WhitelistType m_eType;

	//------------------------------------------------------------------------------------------------
	//! Clientside
	override bool CanBeShown(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{
		return hoveredEntity.GetEntityType() == EEditableEntityType.CHARACTER;
	}
	
	//------------------------------------------------------------------------------------------------	
	//! Clientside 
	override bool CanBePerformed(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override void Perform(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags, int param = -1)
	{
		EL_WhitelistRespawnHandlerComponent whitelistComponent = EL_WhitelistRespawnHandlerComponent.Cast(GetGame().GetGameMode().FindComponent(EL_WhitelistRespawnHandlerComponent));
		whitelistComponent.DisableWhitelistOnPlayer(EL_Utils.GetPlayerUID(hoveredEntity.GetOwner()), m_eType);
	}
}
