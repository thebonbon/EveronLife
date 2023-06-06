[BaseContainerProps()]
class EL_GarageSaveDataClass : EPF_EntitySaveDataClass
{
};

[EDF_DbName.Automatic()]
class EL_GarageSaveData : EPF_EntitySaveData
{
	protected ref map<string, ref array<string>> m_mSavedVehicles = new ref map<string, ref array<string>>;

	//------------------------------------------------------------------------------------------------
	override EPF_EReadResult ReadFrom(IEntity entity, EPF_EntitySaveDataClass attributes)
	{
		 EPF_EReadResult readResult = super.ReadFrom(entity, attributes);

		EL_GarageManagerComponent garage = EL_GarageManagerComponent.Cast(entity.FindComponent(EL_GarageManagerComponent));
		m_mSavedVehicles = garage.GetAllVehicles();

		return readResult;
	}

	//------------------------------------------------------------------------------------------------
	override EPF_EApplyResult ApplyTo(IEntity entity, EPF_EntitySaveDataClass attributes)
	{
		EPF_EApplyResult applyResult = super.ApplyTo(entity, attributes);
		
		EL_GarageManagerComponent garage = EL_GarageManagerComponent.Cast(entity.FindComponent(EL_GarageManagerComponent));
		garage.SetVehicles(m_mSavedVehicles);

		return applyResult;
	}

	//------------------------------------------------------------------------------------------------
	override protected bool SerializationSave(BaseSerializationSaveContext saveContext)
	{
		if (!super.SerializationSave(saveContext))
            return false;

		saveContext.WriteValue("m_mSavedVehicles", m_mSavedVehicles);

		return true;
	}

	//------------------------------------------------------------------------------------------------
	override protected bool SerializationLoad(BaseSerializationLoadContext loadContext)
	{
        if (!super.SerializationLoad(loadContext))
            return false;

		loadContext.ReadValue("m_mSavedVehicles", m_mSavedVehicles);

		return true;
	}
}
