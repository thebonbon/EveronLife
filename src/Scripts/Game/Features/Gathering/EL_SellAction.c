class EL_SellAction : ScriptedUserAction
{
	ResourceName m_SellablePrefab;

	private const ResourceName MONEY_PREFAB = "{FDEE11D818A4C675}Prefabs/Items/Money/DollarBill.et";
	private EL_Price m_ItemPriceConfig;
	ref SCR_PrefabNamePredicate m_pPrefabNamePredicate = new SCR_PrefabNamePredicate();

	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
		inventoryManager.TryDeleteItem(inventoryManager.FindItem(m_pPrefabNamePredicate));
		
		for (int i=0; i < m_ItemPriceConfig.m_iSellPrice; i++)
		{
			inventoryManager.TrySpawnPrefabToStorage(MONEY_PREFAB);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		if (m_ItemPriceConfig)
			outName = string.Format("Sell %1 ($%2)", m_ItemPriceConfig.m_sName, m_ItemPriceConfig.m_iSellPrice);
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
 	{
		SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(user.FindComponent(SCR_InventoryStorageManagerComponent));
		return (m_SellablePrefab && m_ItemPriceConfig && inventoryManager.FindItem(m_pPrefabNamePredicate));
	}	
	
	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
 	{
		return (m_SellablePrefab);
	}
	
	//------------------------------------------------------------------------------------------------	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		SetCannotPerformReason("Item not found");
				
		EL_ShopItemComponent shopItemComponent = EL_ShopItemComponent.Cast(pOwnerEntity.FindComponent(EL_ShopItemComponent));
		m_SellablePrefab = shopItemComponent.GetShopItemPrefab();
		m_ItemPriceConfig = shopItemComponent.GetShopItempriceConfig();
		m_pPrefabNamePredicate.prefabName = m_SellablePrefab;
		
	}

}
