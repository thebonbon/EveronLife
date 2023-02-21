enum EL_LogLevel
{
	NORMAL, 	// Only important logs
	EXTENDED, 	// More logs
	DEBUG	 	// A lot of logs	
}

class EL_Logger
{
	static protected const EL_LogLevel m_eMaxLogLevel = EL_LogLevel.NORMAL;
	
	//------------------------------------------------------------------------------------------------
	static void Log(string prefix, string log, EL_LogLevel logLevel = EL_LogLevel.NORMAL)
	{
		if (m_eMaxLogLevel <= logLevel)
			Print(string.Format("[%1] %2", prefix, log), logLevel);
	}
}

modded class SCR_RegeneratingHitZone
{
	//------------------------------------------------------------------------------------------------
	override void OnDamage(EDamageType type, float damage, HitZone pOriginalHitzone, IEntity instigator, inout vector hitTransform[3], float speed, int colliderID, int nodeID)
	{
		EL_Logger.Log("RP-LOG", string.Format("[RP-LOG] %1 damaged %2 for %3 %4", EL_Utils.GetPlayerName(instigator), EL_Utils.GetPlayerName(GetOwner()), damage, typename.EnumToString(EDamageType, type)));
		super.OnDamage(type, damage, pOriginalHitzone, instigator, hitTransform, speed, colliderID, nodeID);
	}
}

modded class SCR_BaseGameMode
{
	//------------------------------------------------------------------------------------------------
	override void OnPlayerKilled(int playerId, IEntity player, IEntity killer)
	{
		EL_Logger.Log("RP-LOG", string.Format("%1 killed %2", EL_Utils.GetPlayerName(killer), EL_Utils.GetPlayerName(player)));
		super.OnPlayerKilled(playerId, player, killer);
	}
}

modded class SCR_InventoryStorageManagerComponent
{
	//------------------------------------------------------------------------------------------------
	override void InsertItem( IEntity pItem, BaseInventoryStorageComponent pStorageTo = null, BaseInventoryStorageComponent pStorageFrom = null, SCR_InvCallBack cb = null  )
	{
		string playerNameFrom = "Vicinity";
		if (pStorageFrom)
			playerNameFrom = EL_Utils.GetPlayerName(EL_InventoryUtils.GetStorageHierachyRoot(pItem));
		
		super.InsertItem(pItem, pStorageTo, pStorageFrom, cb);
		
		string playerNameTo "Vicinity";
		if (pStorageTo)
			playerNameTo = EL_Utils.GetPlayerName(EL_InventoryUtils.GetStorageHierachyRoot(pItem));
		
		EL_Logger.Log("RP-LOG", string.Format("%1 moved item %2 from %3 to %4", EL_Utils.GetPlayerName(GetOwner()), EL_UIInfoUtils.GetUIInfoName(pItem), playerNameFrom, playerNameTo));
	}
}