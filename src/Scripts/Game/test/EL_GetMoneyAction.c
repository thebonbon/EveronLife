class EL_GetMoneyAction : ScriptedUserAction
{
	private const ResourceName MONEY_PREFAB = "{FDEE11D818A4C675}Prefabs/Items/Money/DollarBill.et";
	
	[Attribute("", UIWidgets.Auto, "")]
	private int m_iAmount;
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
		
		for (int i=0; i < m_iAmount; i++)
		{
			inventoryManager.TrySpawnPrefabToStorage(MONEY_PREFAB);
		}
	}
}


class EL_UnsetActiveComponentClass: ScriptComponentClass
{
};


class EL_UnsetActiveComponent: ScriptComponent
{
	//------------------------------------------------------------------------------------------------
	override void EOnInit(IEntity owner)
	{
		InventoryItemComponent comp = InventoryItemComponent.Cast(owner.FindComponent(InventoryItemComponent));

	}
	
	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{
		SetEventMask(owner, EntityEvent.INIT);
		owner.SetFlags(EntityFlags.ACTIVE, false);
	}
};
