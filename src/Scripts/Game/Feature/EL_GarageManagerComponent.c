[EntityEditorProps(category: "EL/Garage")]
class EL_GarageManagerComponentClass : ScriptComponentClass
{
};

class EL_GarageManagerComponent : ScriptComponent
{
	
	protected ref array<Vehicle> m_aVehicles = new ref array<Vehicle>;
	
	//------------------------------------------------------------------------------------------------
	void AddVehicle(Vehicle vehicle)
	{
		Print("Added Vehicle: " + vehicle);
		m_aVehicles.Insert(vehicle);
	}	
	
	//------------------------------------------------------------------------------------------------
	Vehicle RemoveVehicle()
	{
		Vehicle veh = m_aVehicles.Get(0);
		m_aVehicles.Remove(0);
		return veh;
	}
	
	//------------------------------------------------------------------------------------------------
	void AddVehicles(array<Vehicle> vehicles)
	{
		m_aVehicles.InsertAll(vehicles);
	}
	
	//------------------------------------------------------------------------------------------------
	array<Vehicle> GetAllVehicles()
	{
		return m_aVehicles;
	}	
	
	//------------------------------------------------------------------------------------------------
	array<Vehicle> GetOwnedVehicles()
	{
		//TODO: Add ownerIDs to vehicles
		return m_aVehicles;
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{		
		SetEventMask(owner, EntityEvent.INIT);
		owner.SetFlags(EntityFlags.ACTIVE, false);
	}
	
};
