[EL_ComponentSaveDataType(EL_GarageSaveData, EL_GarageManagerComponent, "GarageManager"), BaseContainerProps()]
class EL_GarageSaveData : EL_ComponentSaveDataBase
{
	protected ref array<Vehicle> m_aVehicles = new ref array<Vehicle>;
	
	//------------------------------------------------------------------------------------------------
	override bool ReadFrom(notnull GenericComponent worldEntityComponent)
	{
		EL_GarageManagerComponent garage = EL_GarageManagerComponent.Cast(worldEntityComponent.FindComponent(EL_GarageManagerComponent));
		m_aVehicles = garage.GetAllVehicles();
		
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override bool ApplyTo(notnull GenericComponent worldEntityComponent)
	{
		EL_GarageManagerComponent garage = EL_GarageManagerComponent.Cast(worldEntityComponent.FindComponent(EL_GarageManagerComponent));	
		garage.AddVehicles(m_aVehicles);
		
		return true;
	}
}
