class EL_InventoryUtils
{
	//Not very performant..
	static bool CanSpawnPrefabToPlayerInv(IEntity player, ResourceName prefab)
	{
		InventoryStorageManagerComponent playerInv = InventoryStorageManagerComponent.Cast(player.FindComponent(InventoryStorageManagerComponent));
		IEntity tempEnt = GetGame().SpawnEntityPrefab(Resource.Load(prefab));
		bool canInsert = playerInv.CanInsertItem(tempEnt);
		delete tempEnt;
		return canInsert;
		
	}
	
}