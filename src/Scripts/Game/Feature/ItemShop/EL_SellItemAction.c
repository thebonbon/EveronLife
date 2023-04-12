class EL_SellItemAction : ScriptedUserAction
{
	[Attribute("1", UIWidgets.EditBox, "Amount of items to sell at once (0 to hide | -1 for all in inv)")]
	protected int m_iSellAmount;
	protected int m_iActualSellAmount;
	
	[Attribute("0", UIWidgets.Flags, "Action can only be used with this whitelist", "", ParamEnumArray.FromEnum(EL_WhitelistType) )]
	protected EL_WhitelistType m_eWhitelistOnlyType;

	protected ResourceName m_SellablePrefab;
	protected EL_ItemPrice m_ItemPriceConfig;

	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		if (!EL_NetworkUtils.IsOwner(pOwnerEntity)) return;

		if (m_iSellAmount == -1)
			m_iActualSellAmount = EL_InventoryUtils.GetAmount(pUserEntity, m_SellablePrefab);

		
		
		EL_InventoryUtils.RemoveAmount(pUserEntity, m_SellablePrefab, m_iActualSellAmount);
		EL_MoneyUtils.AddCash(pUserEntity, m_ItemPriceConfig.m_iSellPrice * m_iActualSellAmount);
	}

	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		if (m_ItemPriceConfig)
		{
			outName = string.Format("Sell %1x %2 ($%3)", m_iActualSellAmount, EL_UIInfoUtils.GetInfo(m_SellablePrefab).GetName(), m_ItemPriceConfig.m_iSellPrice * m_iActualSellAmount);
		}
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
 	{
		if (m_iSellAmount == -1)
			m_iActualSellAmount = EL_InventoryUtils.GetAmount(user, m_SellablePrefab);

		int amountInInv = EL_InventoryUtils.GetAmount(user, m_SellablePrefab);
		
		//Check if Whitelist only
		//No Whitelist specified
		if (m_eWhitelistOnlyType != 0)
		{
			//Check if user has at least one whitelist flag set
			EL_PlayerWhitelistComponent whitelistComponent = EL_PlayerWhitelistComponent.Cast(user.FindComponent(EL_PlayerWhitelistComponent));
			if(!whitelistComponent.HasWhitelist(m_eWhitelistOnlyType))
			{
				SetCannotPerformReason("Not allowed");
				return false;
			}
		}
		return (m_SellablePrefab && m_ItemPriceConfig && amountInInv != 0 && amountInInv >= m_iActualSellAmount);
	}

	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
 	{
		return (
			m_ItemPriceConfig &&
			m_ItemPriceConfig.m_iSellPrice != -1 &&
			m_iSellAmount != 0 &&
			(m_iSellAmount == 1 || m_ItemPriceConfig.m_bAllowMultiSell) &&
			m_SellablePrefab
		);
	}

	//------------------------------------------------------------------------------------------------
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		m_iActualSellAmount = m_iSellAmount;
		SetCannotPerformReason("Item not found");

		EL_ShopItemComponent shopItemComponent = EL_ShopItemComponent.Cast(pOwnerEntity.FindComponent(EL_ShopItemComponent));
		m_SellablePrefab = shopItemComponent.GetShopItemPrefab();
		m_ItemPriceConfig = shopItemComponent.GetShopItemPriceConfig();
	}
}
