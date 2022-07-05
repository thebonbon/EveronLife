[ComponentEditorProps(category: "EveronLife/Shop")]
class EL_ShopCartManagerClass : ScriptComponentClass
{
}

[BaseContainerProps()]
class EL_ShopCartItem
{
	[Attribute()]
	ResourceName prefab;
	[Attribute()]
	int buyPrice;
	[Attribute()]
	int sellPrice;
	[Attribute()]
	ref Color color;
	[Attribute()]
	typename type;
	[Attribute()]
	string name;
}

class EL_ShopCartManager : ScriptComponent
{
    ref array<ref EL_ShopCartItem> m_aShopCartItems = new ref array<ref EL_ShopCartItem>();

	//------------------------------------------------------------------------------------------------
	void AddToCart(EL_Price priceConfig, typename type, Color newColor = null)
	{
		EL_ShopCartItem newShopCartItem = new EL_ShopCartItem();
		newShopCartItem.prefab = priceConfig.m_Prefab;
		newShopCartItem.buyPrice = priceConfig.m_iBuyPrice;
		newShopCartItem.sellPrice = priceConfig.m_iSellPrice;
		newShopCartItem.type = type;
		newShopCartItem.name = priceConfig.m_sName;
		if (newColor)
			newShopCartItem.color = newColor;
		
		m_aShopCartItems.Insert(newShopCartItem);
	}
	
	//------------------------------------------------------------------------------------------------
	int GetTotalCartCost()
	{
		int totalCost = 0;
		foreach (EL_ShopCartItem cartItem : m_aShopCartItems)
		{
			totalCost += cartItem.buyPrice;
		}
		return totalCost;
	}

	//------------------------------------------------------------------------------------------------
	void EmptyCart()
	{
		m_aShopCartItems.Clear();
	}
	
	//------------------------------------------------------------------------------------------------
	int GetCartCount()
	{
		return m_aShopCartItems.Count();
	}	
	//------------------------------------------------------------------------------------------------
	typename GetCartType()
	{
		if (GetCartCount() > 0)
		{
			EL_ShopCartItem shopCartItem = m_aShopCartItems[0];
			return shopCartItem.type;
		}
		return typename.Empty;
	}
}