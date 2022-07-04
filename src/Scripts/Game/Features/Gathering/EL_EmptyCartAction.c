class EL_EmptyCartAction : ScriptedUserAction
{

	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{		
		EL_ShopCartManager shopCartManager = EL_ShopCartManager.Cast(pUserEntity.FindComponent(EL_ShopCartManager));
		shopCartManager.EmptyCart();
	}
	
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		outName = string.Format("Empty cart");
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
 	{
		EL_ShopCartManager shopCartManager = EL_ShopCartManager.Cast(user.FindComponent(EL_ShopCartManager));
		return (shopCartManager && shopCartManager.GetCartCount() != 0);
	}
	
	//------------------------------------------------------------------------------------------------	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		SetCannotPerformReason("No items in cart");
	}

}
