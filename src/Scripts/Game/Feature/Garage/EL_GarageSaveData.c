[BaseContainerProps()]
class EL_GarageSaveDataClass : EPF_EntitySaveDataClass
{
};

[EDF_DbName.Automatic()]
class EL_GarageSaveData : EPF_EntitySaveData
{
};

[EPF_ComponentSaveDataType(EL_GarageManagerComponent), BaseContainerProps()]
class EL_GarageManagerSaveDataClass : EPF_ComponentSaveDataClass
{
};

[EDF_DbName.Automatic()]
class EL_GarageManagerSaveData : EPF_ComponentSaveData
{
	protected ref map<string, ref array<string>> m_mSavedVehicles = new map<string, ref array<string>>;

	//------------------------------------------------------------------------------------------------
	override EPF_EReadResult ReadFrom(IEntity owner, GenericComponent component, EPF_ComponentSaveDataClass attributes)
	{
		EL_GarageManagerComponent garage = EL_GarageManagerComponent.Cast(component);
		m_mSavedVehicles = garage.GetAllVehicles();
		
		PrintFormat("[EL-Garage] Persi saved %1 vehicles", m_mSavedVehicles.Count());
		return EPF_EReadResult.OK;
	}

	//------------------------------------------------------------------------------------------------
	override EPF_EApplyResult ApplyTo(IEntity owner, GenericComponent component, EPF_ComponentSaveDataClass attributes)
	{		
		EL_GarageManagerComponent garage = EL_GarageManagerComponent.Cast(component);
		garage.SetVehicles(m_mSavedVehicles);
		
		PrintFormat("[EL-Garage] Persi loaded %1 vehicles.", m_mSavedVehicles.Count());
		
		return EPF_EReadResult.OK;
	}
}
