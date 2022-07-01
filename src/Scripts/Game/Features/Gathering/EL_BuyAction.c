class EL_BuyAction : ScriptedUserAction
{
	[Attribute("{18A98641909A7F52}Configs/Shops/EL_ShopPrices.conf", UIWidgets.Auto, "Item price")]
	private ref EL_PriceConfig m_PriceConfig;
	private ResourceName moneyResourceName = "{FDEE11D818A4C675}Prefabs/Items/Money/DollarBill.et";
	private ResourceName m_PrefabName;
	private SCR_InventoryStorageManagerComponent m_InventoryManager;
	private EL_Price m_ItemPriceConfig;
	private bool m_bIsPrefabInCofig;
	ref SCR_PrefabNamePredicate m_pPrefabNamePredicate = new SCR_PrefabNamePredicate();
	//------------------------------------------------------------------------------------------------
	private bool TryGetPrefabPriceConfig()
	{
		foreach (EL_Price price : m_PriceConfig.m_aPriceConfigs)
		{
			if (price.m_Prefab == m_PrefabName)
			{
				m_ItemPriceConfig = price;
				return true;
			}
		}
		
		Print("No price config found for " + m_PrefabName, LogLevel.WARNING);
		return false;
	}
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
		array<IEntity> moneyItems = new array<IEntity>;
		inventoryManager.FindItems(moneyItems, m_pPrefabNamePredicate);
		for (int i=0; i < m_ItemPriceConfig.m_iPrice; i++)
		{
			inventoryManager.TryDeleteItem(moneyItems[i]);
		}
		inventoryManager.TrySpawnPrefabToStorage(m_PrefabName);
		
	}
	
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		outName = string.Format("Buy %1 ($%2)", m_ItemPriceConfig.m_sName, m_ItemPriceConfig.m_iPrice);
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
 	{
		m_InventoryManager = SCR_InventoryStorageManagerComponent.Cast(user.FindComponent(SCR_InventoryStorageManagerComponent));
		int moneyInInv = m_InventoryManager.GetDepositItemCountByResource(moneyResourceName);
		if (!m_bIsPrefabInCofig)
			return false;
		if (!m_InventoryManager.CanInsertItem(GetOwner()))
		{
			SetCannotPerformReason("Inventory full");
			return false;
		}
		if (moneyInInv < m_ItemPriceConfig.m_iPrice)
		{	
			SetCannotPerformReason("Not enough money");
			return false;
		}
		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		m_PrefabName = SCR_BaseContainerTools.GetPrefabResourceName(pOwnerEntity.GetPrefabData().GetPrefab());
		m_bIsPrefabInCofig = TryGetPrefabPriceConfig();
		m_pPrefabNamePredicate.prefabName = moneyResourceName;
	}

}
