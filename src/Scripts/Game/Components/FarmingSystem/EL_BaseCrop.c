[ComponentEditorProps(category: "EveronLife/Game/Farming")]
class EL_BaseCropClass : EL_BaseBuildingClass 
{
};	

class EL_BaseCrop : EL_BaseBuilding 
{
	[Attribute("false", UIWidgets.CheckBox, "Spawn items with area spawner?", category: "Crop")]
	protected bool m_UseAreaSpawner;
	bool m_IsGrown = false;
	private bool m_SpawnedArea = false;
		
	//------------------------------------------------------------------------------------------------
	protected void Grow(IEntity owner, float timeSlice) 
	{
		m_IsGrown = true;
	}
	
	//------------------------------------------------------------------------------------------------
	override void EOnFrame(IEntity owner, float timeSlice)
	{		
		super.EOnFrame(owner, timeSlice);
		if (!m_IsGrown)
			Grow(owner, timeSlice);
		else if (!m_SpawnedArea && m_UseAreaSpawner) 
		{
			EL_AreaSpawnerComponent areaSpawner = EL_AreaSpawnerComponent.Cast(owner.FindComponent(EL_AreaSpawnerComponent));
			areaSpawner.SpawnPrefabs(owner);
			m_SpawnedArea = true;
		}
			
	}
	
	//------------------------------------------------------------------------------------------------
	override void EOnInit(IEntity owner)
	{	
		super.EOnInit(owner);
		
		owner.SetEventMask(EntityEvent.INIT | EntityEvent.FRAME);
		owner.SetFlags(EntityFlags.ACTIVE, true);
	}
	
};