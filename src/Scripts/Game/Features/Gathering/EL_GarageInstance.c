class EL_GarageInstance
{
	ResourceName m_GaragePrefab = "{8504F81F14656B33}Prefabs/Structures/Garage/PlayerGarage.et";
	bool isLoaded;
	string m_sGarageId;
	string m_sOwnerId;
	ref array<string> m_aSavedVehicles = {};
	ref array<string> m_aActivePlayers = {};
	

	IEntity m_GarageEntity;
	IEntity m_RealGarageEntity;

	void EL_GarageInstance(string ownerId, IEntity realGarage)
	{
		Print("[EL-GarageInstance] Constructing new instance...");
		m_sGarageId = "garage_instance_" + ownerId;
		m_sOwnerId = ownerId;
		m_RealGarageEntity = realGarage;

	}

	void ~EL_GarageInstance()
	{
		Print("[EL-GarageInstance] Destructing instance...");
		//TODO: Cleanup here
		//Delete spawned previews
		SCR_EntityHelper.DeleteEntityAndChildren(m_GarageEntity);
	};

	//------------------------------------------------------------------------------------------------
	void WithdrawVehicle(IEntity vehicle)
	{
		IEntity freeSpawnPoint = EL_SpawnUtils.FindFreeSpawnPoint(m_RealGarageEntity);
		if (!freeSpawnPoint)
		{
			Print("[EL-Garage] No real garage spawn points free!", LogLevel.WARNING);
			return;
		}
		
		EL_PersistenceComponent persistence = EL_PersistenceComponent.Cast(vehicle.FindComponent(EL_PersistenceComponent));
		m_aSavedVehicles.RemoveItem(persistence.GetPersistentId());

		Print("[EL-Garage] Teleporting entered vehicle: " + persistence.GetPersistentId());
		EL_Utils.Teleport(vehicle, freeSpawnPoint.GetOrigin(), freeSpawnPoint.GetYawPitchRoll());

	}
	
	//------------------------------------------------------------------------------------------------
	array<string> GetAllVehicles()
	{
		return m_aSavedVehicles;
	}
	//------------------------------------------------------------------------------------------------
	void SavedVehicle(string vehicleId)
	{
		m_aSavedVehicles.Insert(vehicleId);
	}
	
	//------------------------------------------------------------------------------------------------
	void Populate()
	{
		if (m_aSavedVehicles.Count() == 0)
			return;
		
		foreach (string vehicleId : m_aSavedVehicles)
		{
			IEntity freeSpawnPoint = EL_SpawnUtils.FindSpawnPoint(m_GarageEntity);
			if (!freeSpawnPoint)
			{
				Print("[EL-GarageInstance] To many vehicles stored in this garage! Not spawning veh: " + vehicleId, LogLevel.WARNING);
				continue;
			}
			
			Print("[EL-GarageInstance] Populating with vehicle: " + vehicleId);
			IEntity previewVehicle = EL_PersistentWorldEntityLoader.Load(EL_VehicleSaveData, vehicleId);
			previewVehicle.SetOrigin(freeSpawnPoint.GetOrigin());
			previewVehicle.SetAngles(freeSpawnPoint.GetAngles());
		}
	}


	//------------------------------------------------------------------------------------------------
	void Create(vector origin)
	{
		Print("[EL-GarageInstance] Creating new instance with prefab: " + m_GaragePrefab);
		m_GarageEntity = EL_Utils.SpawnEntityPrefab(m_GaragePrefab, "0 20 0");
		if (!m_GarageEntity)
		{
			Print("[EL-GarageInstance] Error spawning instance prefab!", LogLevel.WARNING);
			return;
		}
		isLoaded = true;
		
		Populate();
	}

	//------------------------------------------------------------------------------------------------
	void SaveVehicle(string vehicleId)
	{
		m_aSavedVehicles.Insert(vehicleId);
	}

	//------------------------------------------------------------------------------------------------
	void OnPreviewVehicleEntered( IEntity vehicle, BaseCompartmentManagerComponent manager, int mgrID, int slotID )
	{	
		BaseCompartmentSlot compSlot = manager.FindCompartment(slotID, mgrID);
		if (!compSlot)
			return;
	
		IEntity playerEntity = compSlot.GetOccupant();
		if (!playerEntity)
			return;
		
		Print("EL-Garage] Player entered vehicle: " + vehicle.GetName());
		
		WithdrawVehicle(vehicle);
	}
	
	//------------------------------------------------------------------------------------------------
	void Enter(IEntity user)
	{
		m_aActivePlayers.Insert(EL_Utils.GetPlayerUID(user));
		EL_Utils.Teleport(user, m_GarageEntity.GetOrigin() + vector.Right);
		
		SCR_CompartmentAccessComponent accessComp = SCR_CompartmentAccessComponent.Cast(user.FindComponent(SCR_CompartmentAccessComponent));
		if (accessComp)
		{
			accessComp.GetOnCompartmentEntered().Insert(OnPreviewVehicleEntered);
		}
	}

	//------------------------------------------------------------------------------------------------
	void Exit(IEntity user, IEntity garageExit)
	{
		m_aActivePlayers.RemoveItem(EL_Utils.GetPlayerUID(user));
		EL_Utils.Teleport(user, garageExit.GetOrigin(), garageExit.GetYawPitchRoll());
	}
	
	//------------------------------------------------------------------------------------------------
	bool IsEmpty()
	{
		return (m_aActivePlayers.Count() == 0);
	}

	//------------------------------------------------------------------------------------------------
	string GetId()
	{
		return m_sGarageId;
	}


}