[EntityEditorProps(category: "EveronLife/Core")]
class EL_RpcSenderComponentClass : ScriptComponentClass
{
}

class EL_RpcSenderComponent  : ScriptComponent
{
	//------------------------------------------------------------------------------------------------
	//Vehicle Shop / Garage
	//------------------------------------------------------------------------------------------------
	
	//------------------------------------------------------------------------------------------------	
	//! Vehicle Owner
	void AskIsLocalOwner(IEntity vehicle)
	{		
		RplComponent rplComp = EL_ComponentFinder<RplComponent>.Find(vehicle);
		EL_Logger.Log("EL-CharOwner", string.Format("Asking Server if vehicle: %1 is owned by me..", rplComp.Id()), EL_LogLevel.DEBUG);
		Rpc(DoCheckLocalVehicleOwner, rplComp.Id());
	}
	
	//------------------------------------------------------------------------------------------------
	//! Vehicle Owner
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void DoCheckLocalVehicleOwner(RplId vehicleId)
	{
		RplComponent vehicleRpl = RplComponent.Cast(Replication.FindItem(vehicleId));
		if (!vehicleRpl)
		{
			EL_Logger.Log("EL-CharOwner", string.Format("Server got request to check veh owner.. VehicleRPL not found! %1", vehicleId), EL_LogLevel.DEBUG);
			return;
		}	
		EL_CharacterOwnerComponent charOwnerComp = EL_CharacterOwnerComponent.Cast(vehicleRpl.GetEntity().FindComponent(EL_CharacterOwnerComponent));
		if (EL_Utils.GetPlayerUID(GetOwner()) == charOwnerComp.GetCharacterOwner())
		{
			EL_Logger.Log("EL-CharOwner", string.Format("Server got request to check veh owner.. TRUE %1", vehicleId), EL_LogLevel.DEBUG);
			AskSetLocalVehicleOwner(vehicleId);
			return;
		}	
		EL_Logger.Log("EL-CharOwner", string.Format("Server got request to check veh owner.. FALSE %1", vehicleId), EL_LogLevel.DEBUG);	
	}
	
	//------------------------------------------------------------------------------------------------	
	//! Vehicle Owner
	void AskSetLocalVehicleOwner(RplId vehicleId)
	{
		Print("[EL-CharOwner] Ask set " + vehicleId);
		Rpc(DoSetLocalVehicleOwner, vehicleId);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Vehicle Owner
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void DoSetLocalVehicleOwner(RplId vehicleId)
	{
		RplComponent vehicleRpl = RplComponent.Cast(Replication.FindItem(vehicleId));
		if (!vehicleRpl)
			return;
		EL_CharacterOwnerComponent charOwnerComp = EL_CharacterOwnerComponent.Cast(vehicleRpl.GetEntity().FindComponent(EL_CharacterOwnerComponent));
		charOwnerComp.SetLocalOwner();
	}
	
	//------------------------------------------------------------------------------------------------	
	//! Buy Vehicle
	void AskBuyVehicle(ResourceName vehiclePrefab, int color, IEntity vehicleShop)
	{
		RplComponent rplComp = EL_ComponentFinder<RplComponent>.Find(vehicleShop);
		Rpc(Rpc_AskBuyVehicle, vehiclePrefab, color, rplComp.Id());
	}
	
	//------------------------------------------------------------------------------------------------
	//! Buy Vehicle
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void Rpc_AskBuyVehicle(ResourceName vehiclePrefab, int color, RplId shopId)
	{
		RplComponent vehicleShopRpl = RplComponent.Cast(Replication.FindItem(shopId));
		EL_VehicleShopManagerComponent vehicleShopManager = EL_ComponentFinder<EL_VehicleShopManagerComponent>.Find(vehicleShopRpl.GetEntity());
		
		vehicleShopManager.DoBuyVehicle(vehiclePrefab, color, GetOwner());
	}
	
	//------------------------------------------------------------------------------------------------	
	//! Load Garage
	void AskLoadGarage(IEntity garageEnt)
	{
		RplComponent rplComp = EL_ComponentFinder<RplComponent>.Find(garageEnt);
		Rpc(Rpc_AskLoadGarage, rplComp.Id());
	}
	
	//------------------------------------------------------------------------------------------------
	//! Load Garage
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void Rpc_AskLoadGarage(RplId garageId)
	{
		RplComponent garageRplComp = RplComponent.Cast(Replication.FindItem(garageId));
		EL_GarageManagerComponent garageManager = EL_ComponentFinder<EL_GarageManagerComponent>.Find(garageRplComp.GetEntity());
		
		garageManager.DoLoadGarage(GetOwner());
	}	
	
	//------------------------------------------------------------------------------------------------
	//! Withdraw Vehicle
	void AskWithdrawVehicle(IEntity garageEnt, int index)
	{
		RplComponent rplComp = EL_ComponentFinder<RplComponent>.Find(garageEnt);
		Rpc(Rpc_AskWithdrawVehicle, rplComp.Id(), index);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Withdraw Vehicle
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void Rpc_AskWithdrawVehicle(RplId garageId, int index)
	{
		RplComponent garageRplComp = RplComponent.Cast(Replication.FindItem(garageId));
		EL_GarageManagerComponent garageManager = EL_ComponentFinder<EL_GarageManagerComponent>.Find(garageRplComp.GetEntity());
		
		garageManager.DoWithdrawVehicle(GetOwner(), index);
	}

	//------------------------------------------------------------------------------------------------
	//Bank System
	//------------------------------------------------------------------------------------------------
	protected EL_GlobalBankAccountManager m_BankManager;
	protected bool m_bIsMaster;

	//------------------------------------------------------------------------------------------------
	void AskSetLocalBankAccount(EL_BankAccount account)
	{
		//Custom string encoder
		array<string> transactionComments = {};
		foreach (EL_BankTransaction transaction : account.m_aTransactions)
		{
			transactionComments.Insert(transaction.m_sComment);
		}
		Rpc(Rpc_DoSetLocalBankAccount, account, transactionComments);
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void Rpc_DoSetLocalBankAccount(EL_BankAccount account, array<string> transactionComments)
	{
		//Custom string decoder
		foreach (int i, string comment : transactionComments)
		{
			account.m_aTransactions[i].m_sComment = comment;
		}

		m_BankManager.m_LocalBankAccount = account;
		//Update open bank ui
		EL_BankMenu bankMenu = EL_BankMenu.Cast(GetGame().GetMenuManager().FindMenuByPreset(ChimeraMenuPreset.EL_BankMenu));
		if (bankMenu)
			bankMenu.OnMenuFocusGained();
	}

	//------------------------------------------------------------------------------------------------
	void AskTransactionFromBankAccount(int amount, string comment)
	{
		if (m_bIsMaster)
			Rpc_DoTransactionBankAccount(amount, comment);
		else
			Rpc(Rpc_DoTransactionBankAccount, amount, comment);
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void Rpc_DoTransactionBankAccount(int amount, string comment)
	{
		EL_BankAccount updatedAccount = m_BankManager.NewAccountTransaction(EL_Utils.GetPlayerUID(GetOwner()), amount, comment);

		//Send updated account back to client
		AskSetLocalBankAccount(updatedAccount);
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void Rpc_DoLoadBankAccount()
	{
		m_BankManager.LoadPlayerBankAccount(GetOwner());
	}

	//------------------------------------------------------------------------------------------------
	//! Init after Replication
	void LateInit(IEntity owner)
	{
		m_bIsMaster = EL_NetworkUtils.IsMaster(owner);
		if (m_bIsMaster)
			Rpc_DoLoadBankAccount();
		else
			Rpc(Rpc_DoLoadBankAccount);
	}

	//------------------------------------------------------------------------------------------------
	override void EOnInit(IEntity owner)
	{
		GetGame().GetCallqueue().CallLater(LateInit, 100, false, owner);
		m_BankManager = EL_GlobalBankAccountManager.GetInstance();
	}

	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{
		SetEventMask(owner, EntityEvent.INIT);
	}
}
