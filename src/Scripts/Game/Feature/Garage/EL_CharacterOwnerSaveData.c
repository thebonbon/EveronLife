[EL_ComponentSaveDataType(EL_CharacterOwnerSaveData, EL_CharacterOwnerComponent, "CharacterOwner"), BaseContainerProps()]
class EL_CharacterOwnerSaveData : EL_ComponentSaveDataBase
{
	protected string m_sCharacterId;

	//------------------------------------------------------------------------------------------------
	override bool ReadFrom(notnull GenericComponent worldEntityComponent)
	{
		EL_CharacterOwnerComponent charOwner = EL_CharacterOwnerComponent.Cast(worldEntityComponent);
		m_sCharacterId = charOwner.GetCharacterOwner();

		return true;
	}

	//------------------------------------------------------------------------------------------------
	override bool ApplyTo(notnull GenericComponent worldEntityComponent)
	{
		EL_CharacterOwnerComponent charOwner = EL_CharacterOwnerComponent.Cast(worldEntityComponent);
		EL_GameModeRoleplay gamemode = EL_GameModeRoleplay.Cast(GetGame().GetGameMode());
		gamemode.AddVehicleOwnerCache(m_sCharacterId, charOwner.GetOwner());
		
		return true;
	}
}
