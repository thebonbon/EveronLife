class EL_CharacterOwnerComponentClass: ScriptComponentClass
{
}

class EL_CharacterOwnerComponent: ScriptComponent
{
	protected string m_sCharacterId;
	bool m_IsLocalOwner;
	
	//------------------------------------------------------------------------------------------------
	void SetCharacterOwner(string characterId)
	{
		m_sCharacterId = characterId;
	}

	//------------------------------------------------------------------------------------------------
	string GetCharacterOwner()
	{
		return m_sCharacterId;
	}
}
