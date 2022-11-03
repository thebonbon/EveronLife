class EL_HouseBuyAction : ScriptedUserAction
{
	IEntity m_ParentHouse;
	EL_CharacterOwnerComponent m_CharacterOwner;
	ResourceName m_ParentHousePrefab;
	EL_LockComponent m_DoorLock;
	ResourceName m_KeyPrefab = "{4895654A61D2F41A}Prefabs/Items/HouseKeyItem.et";
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		m_CharacterOwner.SetCharacterOwner(EL_Utils.GetPlayerUID(pUserEntity));
		//Spawn Key
		InventoryStorageManagerComponent playerInv = InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(InventoryStorageManagerComponent));
		IEntity keyItem = GetGame().SpawnEntityPrefab(Resource.Load(m_KeyPrefab));
		EL_KeyComponent keyComp = EL_KeyComponent.Cast(keyItem.FindComponent(EL_KeyComponent));
		keyComp.m_HouseEntity = m_ParentHouse;
		
		playerInv.TryInsertItem(keyItem);
		EL_HouseManagerComponent houseManager = EL_HouseManagerComponent.Cast(m_ParentHouse.FindComponent(EL_HouseManagerComponent));
		houseManager.LockAllDoors();
	}
	
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		outName = "Buy house";
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
 	{
		//Is a house?
		if (!m_ParentHousePrefab.Contains("House"))
		{
			SetCannotPerformReason("Not buyable");
			return false;
		}
		//Is already owned?
		if (m_CharacterOwner && m_CharacterOwner.GetCharacterOwner())
		{
			SetCannotPerformReason("Already owned");
			return false;
		}
		//Inv space for the key?
		//InventoryStorageManagerComponent playerInv = InventoryStorageManagerComponent.Cast(user.FindComponent(InventoryStorageManagerComponent));
		//if (!playerInv || !EL_InventoryUtils.CanSpawnPrefabToPlayerInv(user, m_KeyPrefab))
		//	return false;
		
		return true;
		
	}
		
	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
 	{
		return true;
	}
	
	//------------------------------------------------------------------------------------------------	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		m_DoorLock = EL_LockComponent.Cast(pOwnerEntity.FindComponent(EL_LockComponent));
		m_ParentHouse = SCR_EntityHelper.GetMainParent(pOwnerEntity);
		if (m_ParentHouse)
		{
			m_ParentHousePrefab = SCR_BaseContainerTools.GetPrefabResourceName(m_ParentHouse.GetPrefabData().GetPrefab());
			m_CharacterOwner = EL_CharacterOwnerComponent.Cast(m_ParentHouse.FindComponent(EL_CharacterOwnerComponent));
		}
		
	}

}