//------------------------------------------------------------------------------------------------
class EL_ButtonConfirmHandler : ScriptedWidgetEventHandler
{
	ref ScriptInvoker m_OnModalClickOut = new ScriptInvoker();
    override bool OnModalClickOut(Widget modalRoot, int x, int y, int button)
    {
		m_OnModalClickOut.Invoke();
        return true;
    }
};

class EL_BuyAction : ScriptedUserAction
{
	[Attribute("", UIWidgets.ResourcePickerThumbnail, "", "et")]
	private ResourceName m_BuyablePrefab;
	
	[Attribute("", UIWidgets.Auto, "Item price")]
	private ref EL_PriceConfig m_PriceConfig;
	
	private const ResourceName COLOR_PICKER_LAYOUT = "{1608FC6EC0DEBC25}UI/Layouts/Shop/EL_VehicleColorPicker.layout";
	private const ResourceName MONEY_PREFAB = "{FDEE11D818A4C675}Prefabs/Items/Money/DollarBill.et";
	private SCR_InventoryStorageManagerComponent m_InventoryManager;
	private EL_Price m_ItemPriceConfig;
	private bool m_bIsPrefabInCofig;
	ref SCR_PrefabNamePredicate m_pPrefabNamePredicate = new SCR_PrefabNamePredicate();
	private IEntity m_BuyableEntity;
	private EL_ShopCartManager m_ShopCartManager;
	EL_VehicleColorPickerClass m_ColorPicker;
	
	//------------------------------------------------------------------------------------------------
	private void OpenColorPicker()
	{
		MenuBase colorPickerBase = GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.EL_VehicleColorPicker);
		m_ColorPicker = EL_VehicleColorPickerClass.Cast(colorPickerBase);
		
        SCR_ButtonTextComponent confirmButton = SCR_ButtonTextComponent.GetButtonText("ButtonConfirm", m_ColorPicker.m_wRoot);
		confirmButton.m_OnClicked.Insert(ConfirmAddToCart);

	}
	
	//------------------------------------------------------------------------------------------------
	void ConfirmAddToCart()
	{
		m_ShopCartManager.AddToCart(m_ItemPriceConfig, m_ColorPicker.m_NewColor);
	}
	
	
	//------------------------------------------------------------------------------------------------
	private void MakeStoreObject()
	{
		//Break engine
		if (GetOwner().Type() == Vehicle)
		{
			SCR_CarControllerComponent vehicleControls = SCR_CarControllerComponent.Cast(GetOwner().FindComponent(SCR_CarControllerComponent));
			vehicleControls.SetEngineDrowned(true);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	private bool TryGetPrefabPriceConfig()
	{
		if (!m_PriceConfig)
			return false;
		
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
	private void PerformActionVehicle(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		//Open Color selector UI..
		
		//Spawn Vehicle with new color
		m_ShopCartManager = EL_ShopCartManager.Cast(pUserEntity.FindComponent(EL_ShopCartManager));
		if (!m_ShopCartManager)
		{
			Print("No EL_ShopCartManager found on player " + pOwnerEntity, LogLevel.WARNING);
			return;
		}
		OpenColorPicker();
	}
	
	//------------------------------------------------------------------------------------------------
	private void PerformActionItem(IEntity pOwnerEntity, IEntity pUserEntity)
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
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		switch (GetOwner().Type())
		{
			case Vehicle:
			{
				PerformActionVehicle(pOwnerEntity, pUserEntity);
				break;
			}
			case GenericEntity:
			{
				PerformActionItem(pOwnerEntity, pUserEntity);
				break;
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		if (m_ItemPriceConfig)
			outName = string.Format("Add %1 to cart ($%2)", m_ItemPriceConfig.m_sName, m_ItemPriceConfig.m_iBuyPrice);
		else
			outName = "Price Config not found!";
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
 	{
		if (!m_bIsPrefabInCofig)
			return false;
		
		//Only one cart item?
		/*m_ShopCartManager = EL_ShopCartManager.Cast(user.FindComponent(EL_ShopCartManager));
		if (m_ShopCartManager.GetCartCount() != 0)
		{
			SetCannotPerformReason("Cart full");
			return false;
		}
		*/
		m_InventoryManager = SCR_InventoryStorageManagerComponent.Cast(user.FindComponent(SCR_InventoryStorageManagerComponent));

		if (GetOwner().Type() != Vehicle && !m_InventoryManager.CanInsertItem(m_BuyableEntity))
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
		MakeStoreObject();
	}

}
