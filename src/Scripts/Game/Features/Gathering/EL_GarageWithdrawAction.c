class EL_GarageWithdrawAction : ScriptedUserAction
{
	ResourceName vehicleRes = "{259EE7B78C51B624}Prefabs/Vehicles/Wheeled/UAZ469/UAZ469.et";
	
	EL_GarageManagerComponent m_Garage;
	//------------------------------------------------------------------------------------------------
	IEntity FindFreeSpawnPoint()
	{
		IEntity child = GetOwner().GetChildren();
		while (child)
		{
			EL_VehicleSpawnPoint vehicleSpawnPoint = EL_VehicleSpawnPoint.Cast(child.FindComponent(EL_VehicleSpawnPoint));
			if (!vehicleSpawnPoint.GetIsOccupied())
			{
				return child;			
			}
			child = child.GetSibling();
		}
		return null;
	}
	
	//------------------------------------------------------------------------------------------------
	void SpawnVehicle(ResourceName prefabToSpawn)
	{
		IEntity freeSpawnPoint = FindFreeSpawnPoint();
		EL_Utils.SpawnEntityPrefab(prefabToSpawn, freeSpawnPoint.GetOrigin(), freeSpawnPoint.GetYawPitchRoll());
	}
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		SpawnVehicle(vehicleRes);
		EL_GarageManagerComponent garage = EL_GarageManagerComponent.Cast(pOwnerEntity.FindComponent(EL_GarageManagerComponent));
		garage.RemoveVehicle();
	}
	
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		outName = "Withdraw Vehicle";

		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
 	{
		//Check if spawnpoint is empty..
		if (m_Garage.GetOwnedVehicles().Count() == 0)
		{
			SetCannotPerformReason("No vehicles parked");
			return false;
		}
		if (!FindFreeSpawnPoint())
		{
			SetCannotPerformReason("No free spawn points");
			return false;
		}

		return true;
	}
	
	//------------------------------------------------------------------------------------------------	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		m_Garage = EL_GarageManagerComponent.Cast(pOwnerEntity.FindComponent(EL_GarageManagerComponent));
		FindFreeSpawnPoint();
	}

}
