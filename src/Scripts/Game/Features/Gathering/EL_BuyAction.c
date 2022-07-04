class EL_BuyAction : ScriptedUserAction
{
	[Attribute("", UIWidgets.ResourcePickerThumbnail, "", "et")]
	private ResourceName m_BuyablePrefab;
	
	[Attribute("", UIWidgets.Auto, "Item price")]
	private ref EL_PriceConfig m_PriceConfig;
	
	private const ResourceName MONEY_PREFAB = "{FDEE11D818A4C675}Prefabs/Items/Money/DollarBill.et";
	private SCR_InventoryStorageManagerComponent m_InventoryManager;
	private EL_Price m_ItemPriceConfig;
	private bool m_bIsPrefabInCofig;
	ref SCR_PrefabNamePredicate m_pPrefabNamePredicate = new SCR_PrefabNamePredicate();
	private IEntity m_BuyableEntity;
	
	//------------------------------------------------------------------------------------------------
	private bool TryGetPrefabPriceConfig()
	{
		foreach (EL_Price price : m_PriceConfig.m_aPriceConfigs)
		{
			if (price.m_Prefab == m_BuyablePrefab)
			{
				m_ItemPriceConfig = price;
				return true;
			}
		}
		
		Print("No price config found for " + m_BuyablePrefab, LogLevel.WARNING);
		return false;
	}
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
		array<IEntity> moneyItems = new array<IEntity>;
		inventoryManager.FindItems(moneyItems, m_pPrefabNamePredicate);
		for (int i=0; i < m_ItemPriceConfig.m_iBuyPrice; i++)
		{
			inventoryManager.TryDeleteItem(moneyItems[i]);
		}
		inventoryManager.TrySpawnPrefabToStorage(m_BuyablePrefab);
		
	}
	
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		outName = string.Format("Buy %1 ($%2)", m_ItemPriceConfig.m_sName, m_ItemPriceConfig.m_iBuyPrice);
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
 	{
		if (!m_bIsPrefabInCofig)
			return false;
		
		m_InventoryManager = SCR_InventoryStorageManagerComponent.Cast(user.FindComponent(SCR_InventoryStorageManagerComponent));
		int moneyInInv = m_InventoryManager.GetDepositItemCountByResource(MONEY_PREFAB);
		if (moneyInInv < m_ItemPriceConfig.m_iBuyPrice)
		{	
			SetCannotPerformReason("Not enough money");
			return false;
		}

		if (!m_InventoryManager.CanInsertItem(m_BuyableEntity))
		{
			SetCannotPerformReason("Inventory full");
			return false;
		}

		return true;
	}
	
	//------------------------------------------------------------------------------------------------	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		m_bIsPrefabInCofig = TryGetPrefabPriceConfig();
		m_pPrefabNamePredicate.prefabName = MONEY_PREFAB;
		m_BuyableEntity = GetGame().SpawnEntityPrefab(Resource.Load(m_BuyablePrefab));
	}

}