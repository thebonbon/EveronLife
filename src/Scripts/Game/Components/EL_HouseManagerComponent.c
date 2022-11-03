class EL_HouseManagerComponentClass : ScriptComponentClass
{
}

class EL_HouseManagerComponent : ScriptComponent
{
	//------------------------------------------------------------------------------------------------
	void LockAllDoors()
	{
		LockDoors(GetOwner());
	}
	
	//------------------------------------------------------------------------------------------------
	void LockDoors(IEntity parent)
	{
		IEntity child = parent.GetChildren();
		while (child)
		{	
			LockDoors(child);
			EL_LockComponent doorLockComp = EL_LockComponent.Cast(child.FindComponent(EL_LockComponent));
			if (doorLockComp)
				doorLockComp.Lock();	
			
			child = child.GetSibling();
		}
	}
	
}