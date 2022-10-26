class EL_GarageAction : ScriptedUserAction
{
	EL_GarageManagerComponent m_GarageManager;
	
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{

		if (Replication.IsServer())
		{
			bool isHostAndPlay = pUserEntity == SCR_PlayerController.GetLocalControlledEntity();
			m_GarageManager.PopulateLocalGarage(pUserEntity, isHostAndPlay);
			//Check if host & play
		}
		
		RplComponent rplC = RplComponent.Cast(pUserEntity.FindComponent(RplComponent));
		
		//Only on the local player that used the action
		if (pUserEntity == SCR_PlayerController.GetLocalControlledEntity())
		{
			Print("My client garage cam activated!");
			m_GarageManager.EnableGarageCamera(true);
			m_GarageManager.SetUserEntity(pUserEntity);
		}
	}
		
	//------------------------------------------------------------------------------------------------	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		m_GarageManager = EL_GarageManagerComponent.Cast(pOwnerEntity.FindComponent(EL_GarageManagerComponent));
	}	

}
