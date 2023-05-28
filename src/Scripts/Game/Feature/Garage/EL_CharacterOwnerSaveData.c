[EPF_ComponentSaveDataType(EL_CharacterOwnerComponent), BaseContainerProps()]
class EL_CharacterOwnerSaveDataClass : EPF_ComponentSaveDataClass
{
};

[EDF_DbName.Automatic()]
class EL_CharacterOwnerSaveData : EPF_ComponentSaveData
{
	protected string m_sCharacterId;

	//------------------------------------------------------------------------------------------------
	override EPF_EReadResult ReadFrom(IEntity owner, GenericComponent component, EPF_ComponentSaveDataClass attributes)
	{
		EL_CharacterOwnerComponent charOwner = EL_CharacterOwnerComponent.Cast(component);
		m_sCharacterId = charOwner.GetCharacterOwner();
		EL_Logger.Log("EL-CharOwner-SaveData", string.Format("Saving OwnerId: %1 for %2", m_sCharacterId, charOwner.GetOwner()), EL_LogLevel.DEBUG);
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override EPF_EApplyResult ApplyTo(IEntity owner, GenericComponent component, EPF_ComponentSaveDataClass attributes)
	{
		EL_CharacterOwnerComponent charOwner = EL_CharacterOwnerComponent.Cast(component);
		//Add to vehicle cache
		EL_GameModeRoleplay gamemode = EL_GameModeRoleplay.Cast(GetGame().GetGameMode());
		gamemode.AddVehicleOwnerCache(m_sCharacterId, charOwner.GetOwner());
		//Add to actual vehicle
		charOwner.SetCharacterOwner(m_sCharacterId);
		
		EL_Logger.Log("EL-CharOwner-SaveData", string.Format("Applying OwnerId: %1 for %2", m_sCharacterId, charOwner.GetOwner()), EL_LogLevel.DEBUG);
		
		return true;
	}
}
