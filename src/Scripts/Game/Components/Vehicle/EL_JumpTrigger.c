[EntityEditorProps(category: "EveronLife/Items")]
class EL_JumpTriggerClass : ScriptComponentClass
{
};

class EL_JumpTrigger : ScriptComponent
{
	[Attribute("0 0 0", UIWidgets.EditBox, "Box")]
	protected vector m_vJumpVel;	
	


	//------------------------------------------------------------------------------------------------
	void Jump(IEntity ent)
	{
		Physics phys = ent.GetPhysics();
		if (!phys)
			return;
		phys.SetActive(ActiveState.ACTIVE);
		phys.SetVelocity(m_vJumpVel);
	
	}
	
	//------------------------------------------------------------------------------------------------
	override void EOnInit(IEntity owner)
	{
		SCR_BaseTriggerEntity trigger = SCR_BaseTriggerEntity.Cast(owner);
		
		trigger.GetOnActivate().Insert(Jump);
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{
		SetEventMask(owner, EntityEvent.INIT);
		owner.SetFlags(EntityFlags.ACTIVE, false);
	}
}
