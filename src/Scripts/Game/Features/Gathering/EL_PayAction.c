class EL_PayAction : ScriptedUserAction
{
	private const ResourceName MONEY_PREFAB = "{FDEE11D818A4C675}Prefabs/Items/Money/DollarBill.et";

	ref SCR_PrefabNamePredicate m_pPrefabNamePredicate = new SCR_PrefabNamePredicate();
	IEntity m_SpawnPoint;
	ref array<EL_VehicleSpawnPoint> m_aVehicleSpawnPoints = new ref array<EL_VehicleSpawnPoint>();
	EL_ShopCartManager m_ShopCartManager;
	
	
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
	void SpawnVehicle(IEntity userEntity, ResourceName prefabToSpawn, Color color)
	{
		IEntity freeSpawnPoint = FindFreeSpawnPoint();
		IEntity newCar = EL_Utils.SpawnEntityPrefab(prefabToSpawn, freeSpawnPoint.GetOrigin(), freeSpawnPoint.GetYawPitchRoll());
		
		ParametricMaterialInstanceComponent materialOverride = ParametricMaterialInstanceComponent.Cast(newCar.FindComponent(ParametricMaterialInstanceComponent));
		if (!materialOverride)
		{
			Print("[EL-Shop] Vehicle has no ParametricMaterialInstanceComponent!", LogLevel.ERROR);
			return;
		}
		materialOverride.SetColor(ARGB(color.A() * COLOR_1_TO_255, color.R() * COLOR_1_TO_255, color.G() * COLOR_1_TO_255, color.B() * COLOR_1_TO_255));
		
		EL_CharacterOwnerComponent charOwnerComp = EL_CharacterOwnerComponent.Cast(newCar.FindComponent(EL_CharacterOwnerComponent));
		
		//Account ID for now. Later characterID
		string vehOwnerId = EL_Utils.GetPlayerUID(userEntity);
		charOwnerComp.SetCharacterOwner(vehOwnerId);
		Print("User with ID: " + vehOwnerId + " bought vehicle: " + newCar);
	}
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
		EL_ShopCartManager shopCartManager = EL_ShopCartManager.Cast(pUserEntity.FindComponent(EL_ShopCartManager));
		array<IEntity> moneyItems = new array<IEntity>();
		inventoryManager.FindItems(moneyItems, m_pPrefabNamePredicate);
		
		for (int i=0; i < shopCartManager.GetTotalCartCost(); i++)
		{
			inventoryManager.TryDeleteItem(moneyItems[i]);
		}
		
		foreach (EL_ShopCartItem cartItem : shopCartManager.m_aShopCartItems)
		{
			if (cartItem.type == Vehicle)
				SpawnVehicle(pUserEntity, cartItem.prefab, cartItem.color);
			if (cartItem.type == GenericEntity)
				inventoryManager.TrySpawnPrefabToStorage(cartItem.prefab);
		}
		shopCartManager.EmptyCart();
	}
	
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		if (!m_ShopCartManager)
			return false;
		outName = string.Format("Pay $%1\n", m_ShopCartManager.GetTotalCartCost());
		foreach (EL_ShopCartItem cartItem : m_ShopCartManager.m_aShopCartItems)
		{
			outName += string.Format("%1: $%2\n", cartItem.name, cartItem.buyPrice);
		}
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
 	{
		SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(user.FindComponent(SCR_InventoryStorageManagerComponent));
		m_ShopCartManager = EL_ShopCartManager.Cast(user.FindComponent(EL_ShopCartManager));
		if (!inventoryManager || !m_ShopCartManager)
			return false;
		array<IEntity> moneyItems = new array<IEntity>();
		int moneyCount = inventoryManager.FindItems(moneyItems, m_pPrefabNamePredicate);
		
		//Check if spawnpoint is empty..
		if (m_ShopCartManager.GetCartType() == Vehicle && !FindFreeSpawnPoint())
		{
			SetCannotPerformReason("No free spawn points");
			return false;
		}
		if (m_ShopCartManager.GetCartCount() == 0)
		{
			SetCannotPerformReason("No items in cart");
			return false;
		}
		
		if (moneyCount < m_ShopCartManager.GetTotalCartCost())
		{
			SetCannotPerformReason("Not enough money");
			return false;
		}
		return true;
	}
	
	//------------------------------------------------------------------------------------------------	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		m_pPrefabNamePredicate.prefabName = MONEY_PREFAB;
		FindFreeSpawnPoint();
	}

}
