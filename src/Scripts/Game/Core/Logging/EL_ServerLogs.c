modded class SCR_RegeneratingHitZone
{
	//------------------------------------------------------------------------------------------------
	override void OnDamage(EDamageType type, float damage, HitZone pOriginalHitzone, IEntity instigator, inout vector hitTransform[3], float speed, int colliderID, int nodeID)
	{
		super.OnDamage(type, damage, pOriginalHitzone, instigator, hitTransform, speed, colliderID, nodeID);
		Print(string.Format("[RP-LOG] %1 damaged %2 for %3 %4", EL_Utils.GetPlayerName(instigator), EL_Utils.GetPlayerName(GetOwner()), damage, typename.EnumToString(EDamageType, type)));
	}
}

modded class SCR_BaseGameMode
{
	//------------------------------------------------------------------------------------------------
	override void OnPlayerKilled(int playerId, IEntity player, IEntity killer)
	{
		super.OnPlayerKilled(playerId, player, killer);
		Print(string.Format("[RP-LOG] %1 killed %2", EL_Utils.GetPlayerName(killer), EL_Utils.GetPlayerName(player)));
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
		
		Print(string.Format("[RP-LOG] %1 moved item %2 from %3 to %4", EL_Utils.GetPlayerName(GetOwner()), EL_UIInfoUtils.GetUIInfoName(pItem), playerNameFrom, playerNameTo));
	}
}