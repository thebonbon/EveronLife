class EL_GetMoneyAction : ScriptedUserAction
{
	[Attribute("", UIWidgets.Auto, "")]
	private int m_iAmount;
	
	
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		string bankAccountId = EL_GlobalBankAccountManager.GetInstance().GetPlayerBankAccount(pOwnerEntity);
		if (!EL_GlobalBankAccountManager.GetInstance().TryWithdraw(bankAccountId, m_iAmount))
		{
			SCR_HintManagerComponent.GetInstance().ShowCustomHint("You only have " + EL_GlobalBankAccountManager.GetInstance().GetBankAccountMoney(bankAccountId) + "$ left", "Bank", 20);
			return;
		}
		SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
		ResourceName moneyPrefab = EL_GlobalBankAccountManager.GetInstance().GetMoneyPrefab();
		for (int i=0; i < m_iAmount; i++)
		{
			inventoryManager.TrySpawnPrefabToStorage(moneyPrefab);
		}		
	}
	
		
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
 	{
		string bankAccountId = EL_GlobalBankAccountManager.GetInstance().GetPlayerBankAccount(user);
		SetCannotPerformReason("Not enough money in bank");
		return EL_GlobalBankAccountManager.GetInstance().CanWithdraw(bankAccountId, m_iAmount);
	}
	
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		outName = "Withdraw " + m_iAmount + "$";
		return true;
	}
}

class EL_DepositMoneyAction : ScriptedUserAction
{
	
	[Attribute("", UIWidgets.Auto, "")]
	private int m_iAmount;
	
	ref SCR_PrefabNamePredicate m_pPrefabNamePredicate = new SCR_PrefabNamePredicate();
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		string bankAccountId = EL_GlobalBankAccountManager.GetInstance().GetPlayerBankAccount(pOwnerEntity);
		EL_GlobalBankAccountManager.GetInstance().Deposit(bankAccountId, m_iAmount);
		
		SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
		array<IEntity> moneyItems = new array<IEntity>();
		inventoryManager.FindItems(moneyItems, m_pPrefabNamePredicate);

		for (int i=0; i < m_iAmount; i++)
		{
			inventoryManager.TryDeleteItem(moneyItems[i]);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
 	{
		SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(user.FindComponent(SCR_InventoryStorageManagerComponent));
		array<IEntity> moneyItems = new array<IEntity>();
		int moneyCount = inventoryManager.FindItems(moneyItems, m_pPrefabNamePredicate);
		
		if (moneyCount < m_iAmount)
		{
			SetCannotPerformReason("Not enough money in Inv");
			return false;
		}
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		outName = "Deposit " + m_iAmount + "$";
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	void LateInit()
	{
		m_pPrefabNamePredicate.prefabName = EL_GlobalBankAccountManager.GetInstance().GetMoneyPrefab();
	}
	
	//------------------------------------------------------------------------------------------------
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		GetGame().GetCallqueue().CallLater(LateInit, 1);
	}

}