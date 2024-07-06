class EL_BuyItemAction : ScriptedUserAction
{
	[Attribute("1", UIWidgets.EditBox, "Amount of items to buy at once")]
	protected int m_iBuyAmount;

	protected EL_ItemPrice m_ItemPriceConfig;
	protected ResourceName m_BuyablePrefab;

	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		if (!EL_NetworkUtils.IsOwner(pOwnerEntity)) return;

		//Play pickup animation
		CharacterControllerComponent controller = CharacterControllerComponent.Cast(pUserEntity.FindComponent(CharacterControllerComponent));
		if (controller)
		{
			controller.TryPlayItemGesture(EItemGesture.EItemGesturePickUp);
		}
		else
		{
			Print("Error: CharacterControllerComponent not found!", LogLevel.ERROR);
			return;
		}

		SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
		if (!inventoryManager)
		{
			Print("Error: SCR_InventoryStorageManagerComponent not found!", LogLevel.ERROR);
			return;
		}

		for (int i = 0; i < m_iBuyAmount; i++)
		{
			EL_InventoryUtils.AddAmount(pUserEntity, m_BuyablePrefab, 1);
			EL_MoneyUtils.RemoveCash(pUserEntity, m_ItemPriceConfig.m_iBuyPrice);
		}
	}

	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		if (m_ItemPriceConfig)
			outName = string.Format("Buy %1x %2 ($%3)", m_iBuyAmount, EL_UIInfoUtils.GetInfo(m_BuyablePrefab).GetName(), m_ItemPriceConfig.m_iBuyPrice * m_iBuyAmount);
		else
			outName = "Price Config not found!";
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
	{
		if (!m_ItemPriceConfig || !m_BuyablePrefab)
			return false;

		//1. Check money
		if (EL_MoneyUtils.GetCash(user) < m_ItemPriceConfig.m_iBuyPrice * m_iBuyAmount)
		{
			SetCannotPerformReason("Can't afford");
			return false;
		}

		//3. Check free inventory space
		SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(user.FindComponent(SCR_InventoryStorageManagerComponent));
		if (!inventoryManager || !inventoryManager.CanInsertItem(GetOwner()))
		{
			SetCannotPerformReason("Inventory full");
			return false;
		}

		return true;
	}

	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
	{
		return (
			m_ItemPriceConfig &&
			m_ItemPriceConfig.m_iBuyPrice != -1 &&
			m_iBuyAmount != 0 &&
			(m_iBuyAmount == 1 || m_ItemPriceConfig.m_bAllowMultiBuy) &&
			m_BuyablePrefab
		);
	}

	//------------------------------------------------------------------------------------------------
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		if (!pOwnerEntity)
		{
			Print("Error: pOwnerEntity is null!", LogLevel.ERROR);
			return;
		}

		EL_ShopItemComponent shopItemComponent = EL_ShopItemComponent.Cast(pOwnerEntity.FindComponent(EL_ShopItemComponent));
		if (!shopItemComponent)
		{
			Print("Error: EL_ShopItemComponent not found!", LogLevel.ERROR);
			return;
		}

		m_BuyablePrefab = shopItemComponent.GetShopItemPrefab();
		m_ItemPriceConfig = shopItemComponent.GetShopItemPriceConfig();

		if (!m_BuyablePrefab)
		{
			Print("Error: m_BuyablePrefab is null!", LogLevel.ERROR);
			return;
		}

		if (!m_ItemPriceConfig)
		{
			Print("Error: m_ItemPriceConfig is null!", LogLevel.ERROR);
			return;
		}

		InventoryItemComponent invItem = EL_Component<InventoryItemComponent>.Find(pOwnerEntity);
		if (invItem)
		{
			invItem.SetAdditionalVolume(EL_PrefabUtils.GetPrefabItemVolume(m_BuyablePrefab));
			invItem.SetAdditionalWeight(EL_PrefabUtils.GetPrefabItemWeight(m_BuyablePrefab));
		}
		else
		{
			Print("Error: InventoryItemComponent not found!", LogLevel.ERROR);
		}
	}
};
