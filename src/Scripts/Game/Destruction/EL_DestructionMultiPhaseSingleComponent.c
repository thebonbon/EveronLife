#define ENABLE_BASE_DESTRUCTION

//------------------------------------------------------------------------------------------------
[ComponentEditorProps(category: "GameScripted/Destruction", description: "Multi-Phase destruction component, for objects that go through several damage phases")]
class EL_DestructionMultiPhaseSingleComponentClass: SCR_DestructionMultiPhaseComponentClass
{
};

//------------------------------------------------------------------------------------------------
//! Multi-Phase destruction component, for objects that go through several destruction states
class EL_DestructionMultiPhaseSingleComponent : SCR_DestructionMultiPhaseComponent
{
#ifdef ENABLE_BASE_DESTRUCTION

	//------------------------------------------------------------------------------------------------
	override void ApplyDamagePhaseData(SCR_DamagePhaseData pDamagePhaseData)
	{
		//--- Remap textures of the current mesh
		ResourceName modelPath;
		string remap;
		ResourceName name = SCR_BaseContainerTools.GetPrefabResourceName(GetOwner().GetPrefabData().GetPrefab());
		SCR_Global.GetModelAndRemapFromResource(name, modelPath, remap);
		
		super.ApplyDamagePhaseData(pDamagePhaseData);
		VObject mesh = GetOwner().GetVObject();
		GetOwner().SetObject(mesh, remap);
		
	}
#endif
};
