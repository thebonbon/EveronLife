class EL_LockComponentClass : ScriptComponentClass
{
}

class EL_LockComponent : ScriptComponent
{
	protected bool m_bLocked;
	
	bool IsLocked()
	{
		return m_bLocked;
	}	
	
	void Lock()
	{
		m_bLocked = true;
	}
		
	void Unlock()
	{
		m_bLocked = false;
	}	
		
	void ToggleLock()
	{
		m_bLocked = !m_bLocked;
	}
}