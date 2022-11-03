class EL_BuyItemAction : ScriptedUserAction
{
	ResourceName m_BuyablePrefab;
	
	private const ResourceName MONEY_PREFAB = "{FDEE11D818A4C675}Prefabs/Items/Money/DollarBill.et";
	private EL_Price m_ItemPriceConfig;
	ref SCR_PrefabNamePredicate m_pPrefabNamePredicate = new SCR_PrefabNamePredicate();
	private IEntity m_BuyableEntity;

	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
		
		//Delete money items
		array<IEntity> moneyItems = new array<IEntity>();
		inventoryManager.FindItems(moneyItems, m_pPrefabNamePredicate);

		for (int i=0; i < m_ItemPriceConfig.m_iBuyPrice; i++)
		{
			inventoryManager.TryDeleteItem(moneyItems[i]);
		}
		
		//Spawn bought item
		inventoryManager.TrySpawnPrefabToStorage(m_BuyablePrefab);
	}
	
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		if (m_ItemPriceConfig)
			outName = string.Format("Buy %1 ($%2)", m_ItemPriceConfig.m_sName, m_ItemPriceConfig.m_iBuyPrice);
		else
			outName = "Price Config not found!";
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
 	{
		if (!m_ItemPriceConfig || !m_BuyablePrefab)
			return false;
		
		
		SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(user.FindComponent(SCR_InventoryStorageManagerComponent));
		array<IEntity> moneyItems = new array<IEntity>();
		int moneyCount = inventoryManager.FindItems(moneyItems, m_pPrefabNamePredicate);
		
		if (moneyCount < m_ItemPriceConfig.m_iBuyPrice)
		{
			SetCannotPerformReason("Not enough money");
			return false;
		}
		
		if (!inventoryManager.CanInsertItem(m_BuyableEntity))
		{
			SetCannotPerformReason("Inventory full");
			return false;
		}
		
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
 	{
		return (m_BuyablePrefab);
	}
	
	//------------------------------------------------------------------------------------------------	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{	
		EL_ShopItemComponent shopItemComponent = EL_ShopItemComponent.Cast(pOwnerEntity.FindComponent(EL_ShopItemComponent));
		m_BuyablePrefab = shopItemComponent.GetShopItemPrefab();
		m_BuyableEntity = shopItemComponent.GetShopItemEntity();
		m_ItemPriceConfig = shopItemComponent.GetShopItempriceConfig();
		
		m_pPrefabNamePredicate.prefabName = MONEY_PREFAB;

	}

}
