[BaseContainerProps(), SCR_BaseManualCameraComponentTitle()]
class EL_SnapManualCameraComponent : SCR_BaseManualCameraComponent
{

	vector m_vCamMove;
	int m_iMaxIndex;
	int m_iCurIndex;
	
	//------------------------------------------------------------------------------------------------
	override void EOnCameraFrame(SCR_ManualCameraParam param)
	{
		
		if (!param.isManualInputEnabled || m_vCamMove == vector.Zero) return;
		
		param.transform[3] = param.transform[3] + m_vCamMove;
		param.isManualInput = true;
		param.isDirty = true;
		
		m_vCamMove = vector.Zero;
	}
	
	//------------------------------------------------------------------------------------------------
	void MoveCamRight(float value, EActionTrigger trigger)
	{
		if (m_iCurIndex < m_iMaxIndex)
		{
			m_iCurIndex ++;
			m_vCamMove = -vector.Right * 6.5;
		}
	}			
	
	//------------------------------------------------------------------------------------------------
	void MoveCamLeft(float value, EActionTrigger trigger)
	{
		if (m_iCurIndex > 0)
		{
			m_iCurIndex --;
			m_vCamMove = vector.Right * 6.5;
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override bool EOnCameraInit()
	{
		GetInputManager().AddActionListener("ManualCameraSnapLeft", EActionTrigger.DOWN, MoveCamLeft);
		GetInputManager().AddActionListener("ManualCameraSnapRight", EActionTrigger.DOWN, MoveCamRight);
		return true;
	}	
	
	//------------------------------------------------------------------------------------------------
	override void EOnCameraExit()
	{
		GetInputManager().RemoveActionListener("ManualCameraSnapLeft", EActionTrigger.DOWN, MoveCamLeft);
		GetInputManager().RemoveActionListener("ManualCameraSnapRight", EActionTrigger.DOWN, MoveCamRight);
	}
};






















