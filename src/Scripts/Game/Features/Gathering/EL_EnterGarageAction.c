class EL_EnterGarageAction : ScriptedUserAction
{	
	EL_RealGarageManagerComponent m_GarageManager;
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		if (Replication.IsServer())
		{
			m_GarageManager.EnterGarage(pUserEntity, GetGarageId(pUserEntity));
		}
	}

	//------------------------------------------------------------------------------------------------
	string GetGarageId(IEntity pUserEntity)
	{
		return "garage_instance_" + EL_Utils.GetPlayerUID(pUserEntity);
		
	}
	
	//------------------------------------------------------------------------------------------------	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		m_GarageManager = EL_RealGarageManagerComponent.Cast(pOwnerEntity.FindComponent(EL_RealGarageManagerComponent));
	}

}
