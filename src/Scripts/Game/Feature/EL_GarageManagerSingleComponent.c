[EntityEditorProps(category: "EL/Garage")]
class EL_GarageManagerComponentClass : ScriptComponentClass
{
};

class EL_GarageManagerComponent : ScriptComponent
{	
	// Point info?
	[Attribute("0.777 3.069 12.971", UIWidgets.EditBox, "Camera offset used for this PIP")]
	protected vector m_vCameraPoint;

	[Attribute("0 0 0", UIWidgets.EditBox, "Camera offset used for this PIP")]
	protected vector m_vCameraAngels;
	
	[Attribute("0 0 0", UIWidgets.EditBox, "Preview spawnpoint", params: "inf inf 0 purposeCoords spaceEntity")]
	protected vector m_vPreviewSpawnPoint;


	protected bool m_bIsEnabled;
	protected SCR_ManualCamera m_GarageCamera;

	protected ref map<string, ref array<string>> m_mSavedVehicles = new ref map<string, ref array<string>>;

	InputManager m_InputManager;
	IEntity m_GarageEntity;
	IEntity m_RealGarageEntity;

	ResourceName m_GarageCameraPrefab = "{FAE60B62153B7058}Prefabs/Camera/Garage_Camera.et";
	IEntity m_aPreviewVehicle;
	IEntity m_UserEntity;

	ref array<ref EL_GarageData> m_aGarageSaveDataList = new ref array<ref EL_GarageData>();
	int m_iCurPreviewVehicleIndex;


	//------------------------------------------------------------------------------------------------
	void DisableCam()
	{
		EnableGarageCamera(false);
	}

	void AskRPC(IEntity pUserEntity)
	{
		RplComponent rplC = RplComponent.Cast(pUserEntity.FindComponent(RplComponent));
		Rpc(RPC_AskPopulate, rplC.Id());
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void RPC_AskPopulate(RplId playerId)
	{
		Print("Server getting garage data..");
		RplComponent rplC = RplComponent.Cast(Replication.FindItem(playerId));

		IEntity pUserEntity = rplC.GetEntity();
		PopulateLocalGarage(pUserEntity);
	}

	//------------------------------------------------------------------------------------------------
	//! Do this on server
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void Rpc_AskWithdrawVehicle()
	{
		IEntity freeSpawnPoint = EL_SpawnUtils.FindFreeSpawnPoint(m_RealGarageEntity);
		if (!freeSpawnPoint)
			return;
		string ownerId = EL_Utils.GetPlayerUID(m_UserEntity);

		array<string> storedVehicleIds = m_mSavedVehicles.Get(ownerId);
		

		string withdrawnVehicleId = storedVehicleIds.Get(m_iCurPreviewVehicleIndex);
		storedVehicleIds.Remove(m_iCurPreviewVehicleIndex);
		m_mSavedVehicles.Set(ownerId, storedVehicleIds);

		//Load and spawn parked vehicle
		IEntity withdrawnVehicle = EL_PersistentWorldEntityLoader.Load(EL_VehicleSaveData, withdrawnVehicleId);

		//Teleport to free spawn point
		withdrawnVehicle.SetOrigin(freeSpawnPoint.GetOrigin());
		withdrawnVehicle.SetAngles(freeSpawnPoint.GetAngles());

		EL_PersistenceComponent persistence = EL_PersistenceComponent.Cast(withdrawnVehicle.FindComponent(EL_PersistenceComponent));
		persistence.Save();

		DisableCam();
	}

	//------------------------------------------------------------------------------------------------
	void SetUserEntity(IEntity user)
	{
		m_UserEntity = user;
	}

	//------------------------------------------------------------------------------------------------
	void WithdrawVehicle()
	{
		Rpc(Rpc_AskWithdrawVehicle);
	}
	
	//------------------------------------------------------------------------------------------------	
	void NextPreviewVehicle()
	{
		int lastPreviewVehicleIndex = m_iCurPreviewVehicleIndex;
		m_iCurPreviewVehicleIndex ++;
		
		if (m_iCurPreviewVehicleIndex > (m_aGarageSaveDataList.Count() - 1))
			m_iCurPreviewVehicleIndex = 0;
		
		if (lastPreviewVehicleIndex == m_iCurPreviewVehicleIndex)
			return;
		
		SpawnParkedVehiclePreview(m_iCurPreviewVehicleIndex);
	}
	
	//------------------------------------------------------------------------------------------------
	void PreviousPreviewVehicle()
	{
		int lastPreviewVehicleIndex = m_iCurPreviewVehicleIndex;
		m_iCurPreviewVehicleIndex --;
		
		if (m_iCurPreviewVehicleIndex < 0)
			m_iCurPreviewVehicleIndex = m_aGarageSaveDataList.Count() - 1;
		
		if (lastPreviewVehicleIndex == m_iCurPreviewVehicleIndex)
			return;
		
		SpawnParkedVehiclePreview(m_iCurPreviewVehicleIndex);
	}
	
	//------------------------------------------------------------------------------------------------
	void EnableGarageCamera(bool enabled)
	{

		if (enabled && !m_bIsEnabled)
		{
			m_InputManager.AddActionListener("MouseRight", EActionTrigger.DOWN, DisableCam);
			m_InputManager.AddActionListener("MouseLeft", EActionTrigger.DOWN, WithdrawVehicle);
			m_InputManager.AddActionListener("ManualCameraSnapLeft", EActionTrigger.DOWN, PreviousPreviewVehicle);
			m_InputManager.AddActionListener("ManualCameraSnapRight", EActionTrigger.DOWN, NextPreviewVehicle);
			// Create Garage camera
			if (!m_GarageCamera)
				m_GarageCamera = EL_CameraUtils.CreateAndSetCamera(m_GarageCameraPrefab, m_GarageEntity, m_vCameraPoint, m_vCameraAngels);

			m_bIsEnabled = true;
			return;
		}

		if (!enabled && m_bIsEnabled)
		{
			m_InputManager.RemoveActionListener("MouseRight", EActionTrigger.DOWN, DisableCam);
			m_InputManager.RemoveActionListener("MouseLeft", EActionTrigger.DOWN, WithdrawVehicle);
			m_InputManager.RemoveActionListener("ManualCameraSnapLeft", EActionTrigger.DOWN, PreviousPreviewVehicle);
			m_InputManager.RemoveActionListener("ManualCameraSnapRight", EActionTrigger.DOWN, NextPreviewVehicle);

			delete m_aPreviewVehicle;
			
			EL_CameraUtils.DestroyCamera(m_GarageCamera);
			m_bIsEnabled = false;
			return;
		}
	}

	//------------------------------------------------------------------------------------------------
	//! Garage Stuff
	//------------------------------------------------------------------------------------------------


	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	void RPC_SetVehicleSaveData(RplId playerId, array<ref EL_GarageData> garageSaveData)
	{
		Print("Client revieced a RPC Broadcast :) ");
		RplComponent rplC = RplComponent.Cast(Replication.FindItem(playerId));

		IEntity pUserEntity = rplC.GetEntity();
		IEntity localPlayer = SCR_PlayerController.GetLocalControlledEntity();
		Print("Is for me?.. " + pUserEntity != localPlayer);
		if (pUserEntity != localPlayer)
			return;

		Print("Client Recieved Data: " + garageSaveData);
		
		//Cache saved vehicles
		m_aGarageSaveDataList = garageSaveData;
		
		//Spawn first vehicle
		SpawnParkedVehiclePreview(m_iCurPreviewVehicleIndex);

	}

	//------------------------------------------------------------------------------------------------
	void SpawnParkedVehiclePreview(int vehicleIndex)
	{
		EntitySpawnParams params();
		params.TransformMode = ETransformMode.WORLD;

		Print("Spawning preview veh: " + m_aGarageSaveDataList[vehicleIndex].m_rPrefab);
		EL_SpawnUtils.FindSpawnPoint(m_GarageEntity).GetTransform(params.Transform);
		if (m_aPreviewVehicle)
			delete m_aPreviewVehicle;
		m_aPreviewVehicle = GetGame().SpawnEntityPrefabLocal(Resource.Load(m_aGarageSaveDataList[vehicleIndex].m_rPrefab), GetGame().GetWorld(), params);
		ApplyVehicleSaveData(m_aPreviewVehicle, m_aGarageSaveDataList[vehicleIndex].m_iVehicleColor);
		
	}

	//------------------------------------------------------------------------------------------------
	//! Mabye move to utils? Or always set m_aPreviewVehicle
	void ApplyVehicleSaveData(IEntity vehicle, int color)
	{
		ParametricMaterialInstanceComponent carMaterialComponent = ParametricMaterialInstanceComponent.Cast(vehicle.FindComponent(ParametricMaterialInstanceComponent));
        carMaterialComponent.SetColor(color);
	}

	//------------------------------------------------------------------------------------------------
	array<ref EL_GarageData> GetGarageSaveDataList(IEntity pUserEntity)
	{

		EL_DbRepository<EL_VehicleSaveData> vehicleRepo = EL_PersistenceEntityHelper<EL_VehicleSaveData>.GetRepository();
		array<string> allVehiclesInGarage = GetOwnedVehicles(EL_Utils.GetPlayerUID(pUserEntity));
		array<ref EL_GarageData> garageVehicleList = new array<ref EL_GarageData>();
		if (!allVehiclesInGarage)
			return garageVehicleList;
		foreach (string vehicleId : allVehiclesInGarage)
		{
			Print("Loading " + vehicleId);
			EL_VehicleSaveData vehSaveData = vehicleRepo.Find(vehicleId).GetEntity();

			array<ref EL_ComponentSaveDataBase> colorSaveData = vehSaveData.m_mComponentsSaveData.Get(EL_CarColorSaveData);
			EL_CarColorSaveData colorComp = EL_CarColorSaveData.Cast(colorSaveData.Get(0));

			EL_GarageData garageVehicleData = new EL_GarageData();
			garageVehicleData.m_rPrefab = vehSaveData.m_rPrefab;
			garageVehicleData.m_iVehicleColor = colorComp.m_iVehicleColor;
			garageVehicleList.Insert(garageVehicleData);

		}
		return garageVehicleList;

	}

	//------------------------------------------------------------------------------------------------
	//! Called from Server
	void PopulateLocalGarage(IEntity pUserEntity, bool isHostAndPlay = false)
	{
		m_iCurPreviewVehicleIndex = 0;
		
		Print("Server spawning preview Vehicle..");
		RplComponent rplC = RplComponent.Cast(pUserEntity.FindComponent(RplComponent));
		m_aGarageSaveDataList = GetGarageSaveDataList(pUserEntity);

		if (isHostAndPlay)
			SpawnParkedVehiclePreview(m_iCurPreviewVehicleIndex);
		else
			Rpc(RPC_SetVehicleSaveData, rplC.Id(), m_aGarageSaveDataList);

	}

	//------------------------------------------------------------------------------------------------
	void AddVehicle(string vehicleId, string ownerId)
	{
		Print("Added vehicle: " + vehicleId + " for " + ownerId);

		if (m_mSavedVehicles.Get(ownerId))
		{
			//Owner already has stuff in this garage
			ref array<string> storedVehicleIds = m_mSavedVehicles.Get(ownerId);
			storedVehicleIds.Insert(vehicleId);
			m_mSavedVehicles.Set(ownerId, storedVehicleIds);
		}
		else
		{
			//New owner for this garage
			m_mSavedVehicles.Insert(ownerId, {vehicleId});
		}
	}

	//------------------------------------------------------------------------------------------------
	void SetVehicles(map<string, ref array<string>> savedVehicles)
	{
		m_mSavedVehicles = savedVehicles;
	}

	//------------------------------------------------------------------------------------------------
	map<string, ref array<string>> GetAllVehicles()
	{
		return m_mSavedVehicles;
	}

	//------------------------------------------------------------------------------------------------
	//! Return null if no vehicles saved or no ownerID present
	array<string> GetOwnedVehicles(string ownerId)
	{
		array<string> storedVehicleIds = m_mSavedVehicles.Get(ownerId);
		return storedVehicleIds;
	}

	//------------------------------------------------------------------------------------------------
	override void EOnInit(IEntity owner)
	{
		m_InputManager = GetGame().GetInputManager();
		m_GarageEntity = owner.GetWorld().FindEntityByName("PREVIEW_GARAGE");
		m_RealGarageEntity = owner.GetWorld().FindEntityByName("REAL_GARAGE");
	}

	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{
		SetEventMask(owner, EntityEvent.INIT);
		owner.SetFlags(EntityFlags.ACTIVE, false);
	}

};