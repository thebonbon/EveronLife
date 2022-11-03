modded class SCR_DoorUserAction
{
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
	{
		DoorComponent doorComponent = GetDoorComponent();
		if (!doorComponent)
			return false;
		
		EL_LockComponent keyLock = EL_LockComponent.Cast(GetOwner().FindComponent(EL_LockComponent));
		if (keyLock && keyLock.IsLocked())
		{
			SetCannotPerformReason("Locked");
			return false;
		}
		return true;
	}
};