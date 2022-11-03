[ComponentEditorProps(category: "EveronLife/XY", description: "XY")]
class EL_XYComponentClass : ScriptComponentClass
{
}

class EL_XYComponent : ScriptComponent
{
	[Attribute("",  UIWidgets.EditBox, "")]
	protected string m_sExampleAttrib;
	
	[Attribute("", UIWidgets.ResourcePickerThumbnail, "", "et")]
	protected ResourceName m_ExamplePrefab;
	
	//------------------------------------------------------------------------------------------------
	override void EOnInit(IEntity owner)
	{
		//INIT STUFF HERE
		//FIND COMPONENTS ETC..
	}

	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{
		SetEventMask(owner, EntityEvent.INIT);
		//SetEventMask(owner, EntityEvent.INIT | EntityEvent.FRAME); <- FOR FRAME LOGIC
		owner.SetFlags(EntityFlags.ACTIVE, false);
	}
}
	