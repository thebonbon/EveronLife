[EntityEditorProps(category: "EL/Garage")]
class EL_RealGarageManagerComponentClass : ScriptComponentClass
{
};

class EL_RealGarageManagerComponent : ScriptComponent
{
	//[PERSISTENT]
	//< garageId, array<vehicleIds> >
	protected ref map<string, ref array<string>> m_mSavedVehicles = new ref map<string, ref array<string>>;
	//[PERSISTENT]
	//< garageId, array<playerIds> >
	protected ref map<string, ref array<string>> m_mPlayersInGarage = new ref map<string, ref array<string>>;
	
	protected ref array<ref EL_GarageInstance> m_aGarageInstances = {};
	
	InputManager m_InputManager;
	ResourceName m_GaragePrefab = "{8504F81F14656B33}Prefabs/Structures/Garage/PlayerGarage.et";
	IEntity m_UserEntity;
	IEntity m_RealGarageEntity, m_GarageEntity, m_RealGarageExit;



	//------------------------------------------------------------------------------------------------
	//Enter new = yes create
	//Park only = no create
	EL_GarageInstance CreateGarageInstance(IEntity user, bool create = false)
	{
		EL_GarageInstance instance = new EL_GarageInstance(EL_Utils.GetPlayerUID(user), m_RealGarageEntity);
		m_aGarageInstances.Insert(instance);
		
		if (create)
			instance.Create("0 20 0");
		
		return instance;
	}
	
	//------------------------------------------------------------------------------------------------
	void DeleteGarageInstance(string garageId)
	{
		delete FindGarageInstance(garageId);
		
	}
	
	//------------------------------------------------------------------------------------------------
	void LeaveGarage(IEntity user, IEntity garageInstanceEntity)
	{
		EL_GarageInstance instance = FindGarageInstanceFromGarageEntity(garageInstanceEntity);
		
		//3. Teleport player to new garage
		instance.Exit(user, m_RealGarageExit); 
	}
	
	//------------------------------------------------------------------------------------------------
	//! TODO: Check if garage exists and add code to enter, for other players to enter aswell
	void EnterGarage(IEntity user, string garageId)
	{
		//Check if garage exists
		//Spawn new garage prefab
		//Check if we are owner of this instance -> Yes + not spawned --> create
		//if (IsOwnerOfGarageId) --- else ring ring at other garage
		EL_GarageInstance instance = FindGarageInstance(garageId);
		if (!instance)
			instance = CreateGarageInstance(user, true);
		
		//3. Teleport player to new garage
		instance.Enter(user); 
		
	}
	
	//------------------------------------------------------------------------------------------------
	void AddVehicleToGarage(IEntity owner, IEntity vehicle)
	{
		EL_GarageInstance instance = FindGarageInstance(owner);
		if (!instance)
			instance = CreateGarageInstance(owner);
		EL_PersistenceComponent persistence = EL_PersistenceComponent.Cast(vehicle.FindComponent(EL_PersistenceComponent));
		EL_EntitySaveDataBase saveData = persistence.Save();
		
		Print(saveData);
		Print(instance);
		
		instance.SavedVehicle(saveData.GetId());
		
		SCR_EntityHelper.DeleteEntityAndChildren(vehicle);
	}
	
	//------------------------------------------------------------------------------------------------
	EL_GarageInstance FindGarageInstance(string garageId)
	{
		foreach (EL_GarageInstance instance : m_aGarageInstances)
		{
			if (instance.m_sGarageId == garageId)
				return instance;
		}
		return null;
	}	
	
	//------------------------------------------------------------------------------------------------
	EL_GarageInstance FindGarageInstance(IEntity owner)
	{
		foreach (EL_GarageInstance instance : m_aGarageInstances)
		{
			if (instance.m_sOwnerId == EL_Utils.GetPlayerUID(owner))
				return instance;
		}
		return null;
	}
		
	//------------------------------------------------------------------------------------------------
	EL_GarageInstance FindGarageInstanceFromGarageEntity(IEntity garageEnt)
	{
		foreach (EL_GarageInstance instance : m_aGarageInstances)
		{
			if (instance.m_GarageEntity == garageEnt)
				return instance;
		}
		return null;
	}

	//------------------------------------------------------------------------------------------------
	override void EOnInit(IEntity owner)
	{
		m_RealGarageEntity = GetGame().FindEntity("REAL_GARAGE");
		m_RealGarageExit = GetGame().FindEntity("REAL_GARAGE_EXIT");
		m_InputManager = GetGame().GetInputManager();
	}

	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{
		SetEventMask(owner, EntityEvent.INIT);
		owner.SetFlags(EntityFlags.ACTIVE, false);
	}
	
	

};