class EL_SellAction : ScriptedUserAction
{
	[Attribute("", UIWidgets.ResourcePickerThumbnail, "", "et")]
	private ResourceName m_SellablePrefab;
	
	[Attribute("", UIWidgets.Auto, "Item price")]
	private ref EL_PriceConfig m_PriceConfig;
	
	private const ResourceName MONEY_PREFAB = "{FDEE11D818A4C675}Prefabs/Items/Money/DollarBill.et";
	private EL_Price m_ItemPriceConfig;
	private bool m_bIsPrefabInCofig;
	ref SCR_PrefabNamePredicate m_pPrefabNamePredicate = new SCR_PrefabNamePredicate();
	private IEntity m_SellableEntity;
	//------------------------------------------------------------------------------------------------
	private bool TryGetPrefabPriceConfig()
	{
		foreach (EL_Price price : m_PriceConfig.m_aPriceConfigs)
		{
			if (price.m_Prefab == m_SellablePrefab)
			{
				m_ItemPriceConfig = price;
				return true;
			}
		}
		
		Print("No price config found for " + m_SellablePrefab, LogLevel.WARNING);
		return false;
	}
	
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
		outName = string.Format("Sell %1 ($%2)", m_ItemPriceConfig.m_sName, m_ItemPriceConfig.m_iSellPrice);
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
 	{
		SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(user.FindComponent(SCR_InventoryStorageManagerComponent));
		return (m_bIsPrefabInCofig && inventoryManager.FindItem(m_pPrefabNamePredicate));
	}
	
	//------------------------------------------------------------------------------------------------	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		SetCannotPerformReason("Item not found");
		m_bIsPrefabInCofig = TryGetPrefabPriceConfig();
		m_pPrefabNamePredicate.prefabName = m_SellablePrefab;
		m_SellableEntity = GetGame().SpawnEntityPrefab(Resource.Load(m_SellablePrefab));
	}

}
