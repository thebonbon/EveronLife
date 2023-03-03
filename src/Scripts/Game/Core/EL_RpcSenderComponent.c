[EntityEditorProps(category: "EveronLife/Core")]
class EL_RpcSenderComponentClass : ScriptComponentClass
{
}

class EL_RpcSenderComponent  : ScriptComponent
{
	//------------------------------------------------------------------------------------------------	
	void AskSendGlobalHint(float time, string title, string description)
	{
		Rpc(Rpc_AskGlobalHint, title, description, time);
		SCR_HintManagerComponent.ShowCustomHint(description, title, time * 60, false, EFieldManualEntryId.NONE, true);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void Rpc_AskGlobalHint(string title, string description, float time)
	{
		Rpc(Rpc_DoGlobalHint, title, description, time);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	void Rpc_DoGlobalHint(string title, string description, float time)
	{
		SCR_HintManagerComponent.ShowCustomHint(description, title, time * 60, false, EFieldManualEntryId.NONE, true);
	}
		
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
	protected bool m_bIsMaster;

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
		//EL_BankAccount updatedAccount = m_BankManager.NewAccountTransaction(EL_Utils.GetPlayerUID(GetOwner()), amount, comment);

		//Send updated account back to client
		//AskSetLocalBankAccount(updatedAccount);
	}
}
