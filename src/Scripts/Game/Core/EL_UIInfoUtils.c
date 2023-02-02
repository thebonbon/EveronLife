class EL_UIInfoUtils
{
	protected static ref map<ResourceName, ref UIInfo> s_mCache = new map<ResourceName, ref UIInfo>();

	//------------------------------------------------------------------------------------------------
	static string GetUIInfoName(ResourceName prefab)
	{
		return GetInfo(prefab).GetName();
	}
	
	//------------------------------------------------------------------------------------------------
	static string GetUIInfoName(IEntity entity)
	{
		string name;
		if (entity)
			name = GetInfo(entity).GetName();
		return name;
	}
	
	//------------------------------------------------------------------------------------------------
	static UIInfo GetInfo(IEntity entity)
	{
		if (!entity) return null;

		InventoryItemComponent item = EL_ComponentFinder<InventoryItemComponent>.Find(entity);
		if (!item) return null;

		UIInfo resultInfo = item.GetAttributes().GetUIInfo();
		string prefab = EL_Utils.GetPrefabName(entity);
		if (prefab) s_mCache.Set(prefab, resultInfo);

		return resultInfo;
	}
	
	//------------------------------------------------------------------------------------------------
	static UIInfo GetInfo(ResourceName prefab)
	{
		UIInfo resultInfo = s_mCache.Get(prefab);
		
		if (!resultInfo)
		{
			BaseContainer inventoryItemContainer = SCR_BaseContainerTools.FindComponentSource(Resource.Load(prefab), InventoryItemComponent);
			if (!inventoryItemContainer)
				return null;
			BaseContainer attributesContainer = inventoryItemContainer.GetObject("Attributes");
			if (attributesContainer)
			{
				BaseContainer itemDisplayNameContainer = attributesContainer.GetObject("ItemDisplayName");
			    if (itemDisplayNameContainer)
			    	resultInfo = UIInfo.Cast(BaseContainerTools.CreateInstanceFromContainer(itemDisplayNameContainer));
			}

			s_mCache.Set(prefab, resultInfo);
		}

		return resultInfo;
	}

	//------------------------------------------------------------------------------------------------
	static SCR_EditableVehicleUIInfo GetVehicleInfo(ResourceName prefab)
	{
		BaseContainer vehicleUIInfoComponent = SCR_BaseContainerTools.FindComponentSource(Resource.Load(prefab), "SCR_EditableVehicleComponent");
		if (!vehicleUIInfoComponent)
			return null;

		SCR_EditableVehicleUIInfo vehicleUIInfo;
		vehicleUIInfoComponent.Get("m_UIInfo", vehicleUIInfo);
		return vehicleUIInfo;
	}

}
