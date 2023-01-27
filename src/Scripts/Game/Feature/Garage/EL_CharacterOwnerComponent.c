class EL_CharacterOwnerComponentClass: ScriptComponentClass
{
}

class EL_CharacterOwnerComponent: ScriptComponent
{
	protected string m_sCharacterId;
	protected bool m_IsLocalOwner;
	
	//------------------------------------------------------------------------------------------------
	bool IsLocalOwner()
	{
		return m_IsLocalOwner;
	}

	//------------------------------------------------------------------------------------------------
	void SetLocalOwner()
	{
		m_IsLocalOwner = true;
	}

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
