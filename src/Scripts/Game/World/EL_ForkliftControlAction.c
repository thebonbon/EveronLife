class EL_ForkliftControlAction : ScriptedUserAction
{
	
	bool isUp;
	
	private SignalsManagerComponent m_SignalManager;
	int m_iSignalLift;
	Physics rb;
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		rb.SetActive(ActiveState.ACTIVE);
		if (!isUp)
		{
			m_SignalManager.SetSignalValue(m_iSignalLift, 1.250);
			isUp = true;
		}	
		else
		{
			m_SignalManager.SetSignalValue(m_iSignalLift, 0);
			isUp = false;
		}
	}
	
	override bool CanBeShownScript(IEntity user) { return true; };
	override bool CanBePerformedScript(IEntity user) { return true; };
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		if (isUp)
			outName = "Lift down";
		else
			outName = "Lift up";
		return true;
	}
	
	
	//------------------------------------------------------------------------------------------------	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		m_SignalManager = SignalsManagerComponent.Cast(pOwnerEntity.FindComponent(SignalsManagerComponent));
		rb = pOwnerEntity.GetPhysics();
		m_iSignalLift = m_SignalManager.AddOrFindSignal("LiftSignal", 0);
		
		
	}
}
