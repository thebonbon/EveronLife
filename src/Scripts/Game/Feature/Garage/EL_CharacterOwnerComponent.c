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
		EL_Logger.Log("EL-CharOwner", string.Format("Set %1 for %2", characterId, GetOwner()), EL_LogLevel.DEBUG);
		m_sCharacterId = characterId;
	}

	//------------------------------------------------------------------------------------------------
	string GetCharacterOwner()
	{
		EL_Logger.Log("EL-CharOwner", string.Format("Getting OwnerId: %1 for %2", m_sCharacterId, GetOwner()), EL_LogLevel.DEBUG);
		return m_sCharacterId;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool RplLoad(ScriptBitReader reader)
	{
		//We lost LocalOwner data after streaming. Ask server if this is owned by us.
		EL_NetworkUtils.GetLocalRpcSender().AskIsLocalOwner(GetOwner());
		return true;
	}	
}
