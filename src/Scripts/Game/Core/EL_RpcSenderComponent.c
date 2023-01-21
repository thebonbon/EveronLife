[EntityEditorProps(category: "EveronLife/Core")]
class EL_RpcSenderComponentClass : ScriptComponentClass
{
}

class EL_RpcSenderComponent  : ScriptComponent
{
	//------------------------------------------------------------------------------------------------	
	//! Vehicle Owner
	void AskSetLocalVehicleOwner(RplId vehicleId)
	{
		Rpc(DoSetLocalVehicleOwner, vehicleId);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Vehicle Owner
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void DoSetLocalVehicleOwner(RplId vehicleId)
	{
		IEntity vehicle = IEntity.Cast(Replication.FindItem(vehicleId));
		if (!vehicle)
			Print("[EL-VehicleShop] Error finding replicated vehicle! RplId: " + vehicleId, LogLevel.ERROR);
		EL_CharacterOwnerComponent charOwnerComp = EL_CharacterOwnerComponent.Cast(vehicle.FindComponent(EL_CharacterOwnerComponent));
		charOwnerComp.m_IsLocalOwner = true;
		
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
}
