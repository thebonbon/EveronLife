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

	
	protected bool m_bIsEnabled;
	protected SCR_ManualCamera m_GarageCamera;
	
	protected ref map<string, ref array<string>> m_mSavedVehicles = new ref map<string, ref array<string>>;
	
	InputManager m_InputManager;
	IEntity m_GarageEntity;
	IEntity m_RealGarageEntity;
	
	ResourceName m_GarageCameraPrefab = "{FAE60B62153B7058}Prefabs/Camera/Garage_Camera.et";
	ref array<IEntity> m_aPreviewVehicles = new ref array<IEntity>();
	IEntity m_UserEntity;
	
	
	//------------------------------------------------------------------------------------------------
	protected void DestroyCamera(SCR_ManualCamera camera)
	{
		if (camera)
		{
			IEntity cameraParent = camera.GetParent();
			if (cameraParent)
				cameraParent.RemoveChild(camera);

			delete camera;
		}
	}
	
	//------------------------------------------------------------------------------------------------
	protected SCR_ManualCamera CreateCamera(IEntity parent, vector position)
	{
		// Spawn camera
		BaseWorld baseWorld = parent.GetWorld();
		SCR_ManualCamera garageCamera = SCR_ManualCamera.Cast(GetGame().SpawnEntityPrefabLocal(Resource.Load(m_GarageCameraPrefab), baseWorld));
		
		vector mat[4];
		Math3D.MatrixIdentity3(mat);
		mat[3] = position;
		
		garageCamera.SetTransform(mat);
		garageCamera.SetAngles(m_vCameraAngels);

		//! TODO: Change this to snap to spawn points and no count
		EL_SnapManualCameraComponent snapCameraComponent = EL_SnapManualCameraComponent.Cast(garageCamera.FindCameraComponent(EL_SnapManualCameraComponent));
		snapCameraComponent.m_iMaxIndex = GetSpawnPointCount(m_GarageEntity);
	
		parent.AddChild(garageCamera, -1, EAddChildFlags.AUTO_TRANSFORM);
		return garageCamera;
	}
	
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
		IEntity freeSpawnPoint = FindFreeSpawnPoint(m_RealGarageEntity);
		if (!freeSpawnPoint)
			return;
		string ownerId = EL_Utils.GetPlayerUID(m_UserEntity);
		
		array<string> storedVehicleIds = m_mSavedVehicles.Get(ownerId);
		EL_SnapManualCameraComponent snapCameraComponent = EL_SnapManualCameraComponent.Cast(m_GarageCamera.FindCameraComponent(EL_SnapManualCameraComponent));
				
		string withdrawnVehicleId = storedVehicleIds.Get(snapCameraComponent.m_iCurIndex);
		storedVehicleIds.Remove(snapCameraComponent.m_iCurIndex);
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
	
	void SetUserEntity(IEntity user)
	{
		m_UserEntity = user;
	}
	
	void WithdrawVehicle()
	{
		Rpc(Rpc_AskWithdrawVehicle);
	}
	
	//------------------------------------------------------------------------------------------------
	void EnableGarageCamera(bool enabled)
	{
		
		if (enabled && !m_bIsEnabled)
		{
			m_InputManager.AddActionListener("MouseRight", EActionTrigger.DOWN, DisableCam);
			m_InputManager.AddActionListener("MouseLeft", EActionTrigger.DOWN, WithdrawVehicle);
			
			// Create Garage camera
			if (!m_GarageCamera)
				m_GarageCamera = CreateCamera(m_GarageEntity, m_vCameraPoint);
			
				
			CameraManager camManager = GetGame().GetCameraManager();
			camManager.SetCamera(m_GarageCamera);
			
			m_bIsEnabled = true;
			return;
		}

		if (!enabled && m_bIsEnabled)
		{
			m_InputManager.RemoveActionListener("MouseRight", EActionTrigger.DOWN, DisableCam);
			m_InputManager.RemoveActionListener("MouseLeft", EActionTrigger.DOWN, WithdrawVehicle);
			foreach (IEntity previewVeh : m_aPreviewVehicles)
			{
				delete previewVeh;
			}
			DestroyCamera(m_GarageCamera);
			m_bIsEnabled = false;
			return;
		}
	}
	
	//------------------------------------------------------------------------------------------------
	IEntity FindFreeSpawnPoint(notnull IEntity parent)
	{
		IEntity child = parent.GetChildren();
		while (child)
		{
			EL_VehicleSpawnPoint vehicleSpawnPoint = EL_VehicleSpawnPoint.Cast(child.FindComponent(EL_VehicleSpawnPoint));

			if (vehicleSpawnPoint && !vehicleSpawnPoint.GetIsOccupied())
			{
				return child;
			}
			child = child.GetSibling();
		}
		return null;
	}
	
	//------------------------------------------------------------------------------------------------
	//! move to utils
	int GetSpawnPointCount(notnull IEntity parent)
	{
		IEntity child = parent.GetChildren();
		int spawnPoints;
		while (child)
		{
			EL_VehicleSpawnPoint vehicleSpawnPoint = EL_VehicleSpawnPoint.Cast(child.FindComponent(EL_VehicleSpawnPoint));
			
			child = child.GetSibling();
			
			if (vehicleSpawnPoint)
			{
				spawnPoints++; 
			}
			
		}
		Print("Found : " + spawnPoints + " spawn points");
		return spawnPoints;
	}
	//------------------------------------------------------------------------------------------------
	//! Garage Stuff
	//------------------------------------------------------------------------------------------------

	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	void RPC_DoSpawnLocalVehiclesInGarage(RplId playerId, array<ref EL_GarageData> garageSaveData)
	{
		Print("Client revieced a RPC Broadcast :) ");
		RplComponent rplC = RplComponent.Cast(Replication.FindItem(playerId));
		
		IEntity pUserEntity = rplC.GetEntity();
		IEntity localPlayer = SCR_PlayerController.GetLocalControlledEntity();
		Print("Is for me?.. " + pUserEntity != localPlayer);
		if (pUserEntity != localPlayer)
			return;
		
		Print("Client Recieved Data: " + garageSaveData);
		
		SpawnParkedVehiclePreviews(garageSaveData);		
		
	}	
	
	//------------------------------------------------------------------------------------------------
	void DoSpawnLocalVehiclesInGarage(IEntity pUserEntity, array<ref EL_GarageData> garageSaveData)
	{
		Print("Host&Play Recieved: " + garageSaveData);	
		SpawnParkedVehiclePreviews(garageSaveData);		
	}
	
	//------------------------------------------------------------------------------------------------
	void SpawnParkedVehiclePreviews(array<ref EL_GarageData> garageSaveData)
	{
		EntitySpawnParams params();
		params.TransformMode = ETransformMode.WORLD;
		
		foreach (EL_GarageData garageVeh : garageSaveData)
		{
			Print("Spawning preview veh: " + garageVeh.m_rPrefab);
			FindFreeSpawnPoint(m_GarageEntity).GetTransform(params.Transform);
			IEntity nextCar = GetGame().SpawnEntityPrefabLocal(Resource.Load(garageVeh.m_rPrefab), GetGame().GetWorld(), params);
			ApplyVehicleSaveData(nextCar, garageVeh.m_iVehicleColor);
			
			m_aPreviewVehicles.Insert(nextCar);
		}	
	}
	
	//------------------------------------------------------------------------------------------------
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
		Print("Server Populating Garage..");
		RplComponent rplC = RplComponent.Cast(pUserEntity.FindComponent(RplComponent));
		array<ref EL_GarageData> garageData = GetGarageSaveDataList(pUserEntity);
		
		if (isHostAndPlay)
			DoSpawnLocalVehiclesInGarage(pUserEntity, garageData);
		else
			Rpc(RPC_DoSpawnLocalVehiclesInGarage, rplC.Id(), garageData);

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

class EL_GarageData
{
	ResourceName m_rPrefab;
	int m_iVehicleColor;
	
	//------------------------------------------------------------------------------------------------
	static void Encode(SSnapSerializerBase snapshot, ScriptCtx ctx, ScriptBitSerializer packet) 
	{

		snapshot.Serialize(packet, 26);
	}
	
	//------------------------------------------------------------------------------------------------
	static bool Decode(ScriptBitSerializer packet, ScriptCtx ctx, SSnapSerializerBase snapshot) 
	{
		return snapshot.Serialize(packet, 26);
	}
	
	//------------------------------------------------------------------------------------------------
	static bool SnapCompare(SSnapSerializerBase lhs, SSnapSerializerBase rhs, ScriptCtx ctx) 
	{	
		return lhs.CompareSnapshots(rhs, 26);
	}
	
	//------------------------------------------------------------------------------------------------
	static bool PropCompare(EL_GarageData prop, SSnapSerializerBase snapshot, ScriptCtx ctx) 
	{
		return snapshot.Compare(prop.m_rPrefab, 22)
			&& snapshot.CompareInt(prop.m_iVehicleColor);
	}
	
	//------------------------------------------------------------------------------------------------
	static bool Extract(EL_GarageData prop, ScriptCtx ctx, SSnapSerializerBase snapshot) 
	{	
		snapshot.SerializeBytes(prop.m_rPrefab, 22);
		snapshot.SerializeInt(prop.m_iVehicleColor);
		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	static bool Inject(SSnapSerializerBase snapshot, ScriptCtx ctx, EL_GarageData prop) 
	{
		snapshot.SerializeBytes(prop.m_rPrefab, 22);
		snapshot.SerializeInt(prop.m_iVehicleColor);
		
		return true;
	}
}
