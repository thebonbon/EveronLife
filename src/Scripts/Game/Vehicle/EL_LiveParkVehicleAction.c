class EL_LiveParkVehicleAction : ScriptedUserAction
{
	[Attribute(defvalue:"10", UIWidgets.EditBox, desc: "Garage Search Radius")]
	private float m_fGarageSearchRadius;

	private EL_RealGarageManagerComponent m_GarageManager;

	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		
		
		GetGame().GetWorld().QueryEntitiesBySphere(pOwnerEntity.GetOrigin(), m_fGarageSearchRadius, FindFirstGarage, FilterGarage);

		if (!m_GarageManager)
			Print("Garage not longer found!", LogLevel.WARNING);

		//Stop engine and eject passengers
		//TODO: Put this in EL_VehicleUtils
		VehicleControllerComponent vehicleController = VehicleControllerComponent.Cast(pOwnerEntity.FindComponent(VehicleControllerComponent));
		vehicleController.StopEngine();
		
		SCR_BaseCompartmentManagerComponent vehicleComparmentManager = SCR_BaseCompartmentManagerComponent.Cast(pOwnerEntity.FindComponent(SCR_BaseCompartmentManagerComponent));
		array<IEntity> occupants = {};
		vehicleComparmentManager.GetOccupants(occupants);
		
		foreach(IEntity occupant : occupants)
		{
			ChimeraCharacter character = ChimeraCharacter.Cast(occupant);
			if (!character)
				return;
			
			CharacterControllerComponent controller = character.GetCharacterController();
			if (!controller)
				continue;
			
			CompartmentAccessComponent access = character.GetCompartmentAccessComponent();
			if (!access)
				continue;

			access.EjectOutOfVehicle();
			//controller.GetInputContext().SetVehicleCompartment(null);
		}


		//Add to garage
		m_GarageManager.AddVehicleToGarage(pUserEntity, pOwnerEntity);
	}


	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
 	{
		//Check if user is owner of this vehicle
		EL_CharacterOwnerComponent charOwnerComp = EL_CharacterOwnerComponent.Cast(GetOwner().FindComponent(EL_CharacterOwnerComponent));
		if (EL_Utils.GetPlayerUID(user) != charOwnerComp.GetCharacterOwner())
		{
			SetCannotPerformReason("NOT OWNER: " + EL_Utils.GetPlayerUID(user) + " != " + charOwnerComp.GetCharacterOwner());
			//return false;
		}
		//Check if garage is nearby
		GetGame().GetWorld().QueryEntitiesBySphere(GetOwner().GetOrigin(), m_fGarageSearchRadius, FindFirstGarage, FilterGarage);
		return (m_GarageManager);
 	}

	//------------------------------------------------------------------------------------------------
	bool FilterGarage(IEntity ent)
	{
		return (ent.FindComponent(EL_RealGarageManagerComponent));
	}

	//------------------------------------------------------------------------------------------------
	bool FindFirstGarage(IEntity ent)
	{
		m_GarageManager = EL_RealGarageManagerComponent.Cast(ent.FindComponent(EL_RealGarageManagerComponent));
		if (!m_GarageManager)
			return true; //Continue search

		return false; //Stop search
	}
}
