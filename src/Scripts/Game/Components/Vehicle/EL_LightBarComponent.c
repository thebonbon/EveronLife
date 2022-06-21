[EntityEditorProps(category: "EL/LicnesePlateManagerComponent", description: "This renders license plates on your vehicle.", color: "0 0 255 255")]
class EL_LightBarComponentClass: ScriptComponentClass
{
};

class EL_LightBarComponent: ScriptComponent
{
	[Attribute("0.5", UIWidgets.EditBox)]
	float m_fLightSwitchTime;
	
	private float m_fNextSwitchTime = 0;
	LightEntity redLight, blueLight;
	private bool b_RedActive;
	bool b_IsActive;
	
	//------------------------------------------------------------------------------------------------
	void ToggleLights()
	{
		b_IsActive = !b_IsActive;
		if (!b_IsActive)
		{
			redLight.SetEnabled(false);
			blueLight.SetEnabled(false);
		}
		else
			SwitchLights();
	}
	
	//------------------------------------------------------------------------------------------------
	private void SwitchLights()
	{
		b_RedActive = !b_RedActive;
		redLight.SetEnabled(b_RedActive);
		blueLight.SetEnabled(!b_RedActive);
	}
	
	//------------------------------------------------------------------------------------------------
	override void EOnInit(IEntity owner)
	{
		blueLight = LightEntity.Cast(owner.GetChildren());
		redLight = LightEntity.Cast(owner.GetChildren().GetSibling());
		
		redLight.SetEnabled(false);
		blueLight.SetEnabled(false);
	}
	
	//------------------------------------------------------------------------------------------------
	override void EOnFrame(IEntity owner, float timeSlice)
	{
		if (b_IsActive && Replication.Time() >= m_fNextSwitchTime)
		{
			SwitchLights();
			m_fNextSwitchTime = Replication.Time() + m_fLightSwitchTime * 1000;
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{		
		SetEventMask(owner, EntityEvent.INIT | EntityEvent.FRAME);
		owner.SetFlags(EntityFlags.ACTIVE, true);
	}
	
};
