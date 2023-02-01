[EL_ComponentSaveDataType(EL_CharacterOwnerSaveData, EL_CharacterOwnerComponent, "CharacterOwner"), BaseContainerProps()]
class EL_CharacterOwnerSaveData : EL_ComponentSaveDataBase
{
	protected string m_sCharacterId;

	//------------------------------------------------------------------------------------------------
	override bool ReadFrom(notnull GenericComponent worldEntityComponent)
	{
		EL_CharacterOwnerComponent charOwner = EL_CharacterOwnerComponent.Cast(worldEntityComponent);
		m_sCharacterId = charOwner.GetCharacterOwner();
		EL_Logger.Log("EL-CharOwner-SaveData", string.Format("Saving OwnerId: %1 for %2", m_sCharacterId, charOwner.GetOwner()), EL_LogLevel.DEBUG);
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override bool ApplyTo(notnull GenericComponent worldEntityComponent)
	{
		EL_CharacterOwnerComponent charOwner = EL_CharacterOwnerComponent.Cast(worldEntityComponent);
		//Add to vehicle cache
		EL_GameModeRoleplay gamemode = EL_GameModeRoleplay.Cast(GetGame().GetGameMode());
		gamemode.AddVehicleOwnerCache(m_sCharacterId, charOwner.GetOwner());
		//Add to actual vehicle
		charOwner.SetCharacterOwner(m_sCharacterId);
		
		EL_Logger.Log("EL-CharOwner-SaveData", string.Format("Applying OwnerId: %1 for %2", m_sCharacterId, charOwner.GetOwner()), EL_LogLevel.DEBUG);
		
		return true;
	}
}
