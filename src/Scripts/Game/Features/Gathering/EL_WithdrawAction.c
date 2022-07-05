class EL_WithdrawAction : ScriptedUserAction
{
	private const ResourceName MONEY_PREFAB = "{FDEE11D818A4C675}Prefabs/Items/Money/DollarBill.et";
	
	[Attribute("", UIWidgets.EditBox, "Withdraw amount")]
	int m_iMoneyAmount;
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
	
		for (int i=0; i < m_iMoneyAmount; i++)
		{
			inventoryManager.TrySpawnPrefabToStorage(MONEY_PREFAB);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		outName = string.Format("Withdraw $%1 ", m_iMoneyAmount);
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
 	{
		return true;
	}
	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
 	{
		return true;
	}

}
