[ComponentEditorProps(category: "EveronLife/Feature/Whitelist", description: "Server Whitelist Manager")]
class EL_WhitelistRespawnHandlerComponentClass : EL_RespawnHandlerComponentClass
{
}

class EL_WhitelistRespawnHandlerComponent : EL_RespawnHandlerComponent
{
	[Attribute("", UIWidgets.Object)]
	ref array<ref EL_Whitelist> m_aWhitelists;

	//------------------------------------------------------------------------------------------------
	override void OnPlayerSpawned(int playerId, IEntity controlledEntity)
	{
		// Server only
		if (!m_pGameMode.IsMaster())
			return;
		
		super.OnPlayerSpawned(playerId, controlledEntity);

		EL_PlayerWhitelistComponent playerWhitelistComponent = EL_PlayerWhitelistComponent.Cast(controlledEntity.FindComponent(EL_PlayerWhitelistComponent));
		LoadAllWhitelists(playerWhitelistComponent);
	}

	//------------------------------------------------------------------------------------------------
	override void LoadPlayerData(int playerId, string playerUid)
	{
		if (!VerifyPlayer(EL_Utils.GetPlayerUID(playerId), EL_WhitelistType.CONNECT))
		{
			GetGame().GetPlayerManager().KickPlayer(playerId, PlayerManagerKickReason.KICK, 10);
			return;
		}
		PrintFormat("[EL-WHITELIST] Allowed UUID: %1 to join", EL_Utils.GetPlayerUID(playerId));
		
		super.LoadPlayerData(playerId, playerUid);
	}
	
	//------------------------------------------------------------------------------------------------
	void LoadAllWhitelists(EL_PlayerWhitelistComponent playerWhitelistComponent)
	{
		// Server only
		if (!m_pGameMode.IsMaster())
			return;
		
		EL_WhitelistType whitelistFlags;
		string uuid = EL_Utils.GetPlayerUID(playerWhitelistComponent.GetOwner());
		foreach (EL_Whitelist whitelist: m_aWhitelists)
		{
			if(whitelist.VerifyPlayer(uuid))
				whitelistFlags |= whitelist.m_eType;

		}
		//Send whitelist flags to owner
		playerWhitelistComponent.AskRpcSetWhitelists(whitelistFlags);
	}

	//------------------------------------------------------------------------------------------------
	bool WhitelistExists(EL_WhitelistType whitelistType)
	{
		// Server only
		if (!m_pGameMode.IsMaster())
			return false;

		foreach (EL_Whitelist whitelist: m_aWhitelists)
		{
			if (whitelist.m_eType == whitelistType)
				return true;
		}
		return false;
	}	
	
	//------------------------------------------------------------------------------------------------
	bool VerifyPlayer(string uuid, EL_WhitelistType whitelistType)
	{
		// Server only
		if (!m_pGameMode.IsMaster())
			return false;
		
		foreach (EL_Whitelist whitelist: m_aWhitelists)
		{
			if (whitelist.m_eType == whitelistType)
				return whitelist.VerifyPlayer(uuid);
		}
		//Whitelist not enabled
		return true;
	}

	//------------------------------------------------------------------------------------------------
	void EnableWhitelistOnPlayer(string uuid, EL_WhitelistType whitelistType)
	{
		// Server only
		if (!m_pGameMode.IsMaster())
			return;
		
		//Save to file
		AddUUIDToWhitelist(uuid, whitelistType);

		//Enable on player if online
		IEntity targetPlayer = EL_Utils.GetPlayerByUID(uuid);
		if (targetPlayer)
		{
			EL_PlayerWhitelistComponent playerWhitelistComponent = EL_PlayerWhitelistComponent.Cast(targetPlayer.FindComponent(EL_PlayerWhitelistComponent));
			playerWhitelistComponent.AskRpcEnableWhitelist(whitelistType);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void AddUUIDToWhitelist(string uuid, EL_WhitelistType whitelistType)
	{
		// Server only
		if (!m_pGameMode.IsMaster())
			return;
		
		foreach (EL_Whitelist whitelist: m_aWhitelists)
		{
			if (whitelist.m_eType == whitelistType)
				whitelist.AddNewUuidToFile(uuid);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void DisableWhitelistOnPlayer(string uid, EL_WhitelistType whitelistType)
	{
		// Server only
		if (!m_pGameMode.IsMaster())
			return;

		if (!WhitelistExists(whitelistType))
		{
			Print(string.Format("[%1-WHITELIST] Trying to remove uid from this whitelist but its not active in the gamemode!", typename.EnumToString(EL_WhitelistType, whitelistType)), LogLevel.WARNING);
			return;
		}

		//Remove from file and cache
		RemoveUUIDFromWhitelist(uid, whitelistType);

		//Disable on player if online
		IEntity targetPlayer = EL_Utils.GetPlayerByUID(uid);
		if (targetPlayer)
		{
			EL_PlayerWhitelistComponent playerWhitelistComponent = EL_PlayerWhitelistComponent.Cast(targetPlayer.FindComponent(EL_PlayerWhitelistComponent));
			playerWhitelistComponent.AskRpcDisableWhitelist(whitelistType);
		}
	}	
	
	//------------------------------------------------------------------------------------------------
	void RemoveUUIDFromWhitelist(string uid, EL_WhitelistType whitelistType)
	{
		// Server only
		if (!m_pGameMode.IsMaster())
			return;

		foreach (EL_Whitelist whitelist: m_aWhitelists)
		{
			if (whitelist.m_eType == whitelistType)
				whitelist.RemoveUUIDFromFile(uid);
		}
	}
}