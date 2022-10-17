class EL_ParkVehicleAction : ScriptedUserAction
{
	[Attribute(defvalue:"10", UIWidgets.EditBox, desc: "Garage Search Radius")]
	private float m_fGarageSearchRadius;

	private EL_GarageManagerComponent m_GarageManager;
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		GetGame().GetWorld().QueryEntitiesBySphere(pOwnerEntity.GetOrigin(), m_fGarageSearchRadius, FindFirstGarage, FilterGarage);
		
		if (!m_GarageManager)
			Print("Garage not longer found!", LogLevel.WARNING);
		
		m_GarageManager.AddVehicle(Vehicle.Cast(pOwnerEntity));
		delete GetOwner();	
	}
	

	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
 	{
		GetGame().GetWorld().QueryEntitiesBySphere(GetOwner().GetOrigin(), m_fGarageSearchRadius, FindFirstGarage, FilterGarage);
		return (m_GarageManager);
 	}

	//------------------------------------------------------------------------------------------------
	bool FilterGarage(IEntity ent) 
	{
		return (ent.FindComponent(EL_GarageManagerComponent));
	}
	
	//------------------------------------------------------------------------------------------------
	bool FindFirstGarage(IEntity ent) 
	{
		m_GarageManager = EL_GarageManagerComponent.Cast(ent.FindComponent(EL_GarageManagerComponent));
		if (!m_GarageManager)
			return true; //Continue search
		
		return false; //Stop search
	}
	

	
}
