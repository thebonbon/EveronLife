[EntityEditorProps(category: "EL/Misc")]
class EL_ConstantRotateComponentClass : ScriptComponentClass
{
};

class EL_ConstantRotateComponent : ScriptComponent
{
	[Attribute("0 0 0", UIWidgets.EditBox, "Rotation Speed")]
	protected vector m_vRotationSpeed;
	
	//------------------------------------------------------------------------------------------------
	override void EOnFrame(IEntity owner, float timeSlice)
	{
		owner.SetAngles(owner.GetAngles() + m_vRotationSpeed * timeSlice);
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{
		SetEventMask(owner, EntityEvent.FRAME);
		owner.SetFlags(EntityFlags.ACTIVE, true);
	}
}