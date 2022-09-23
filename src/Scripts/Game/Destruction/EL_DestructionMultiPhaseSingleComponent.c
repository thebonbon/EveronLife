[ComponentEditorProps(category: "GameScripted/Destruction", description: "Multi-Phase destruction component, for objects that go through several damage phases")]
class EL_DestructionMultiPhaseSingleComponentClass: SCR_DestructionMultiPhaseComponentClass
{
};

class EL_DestructionMultiPhaseSingleComponent : SCR_DestructionMultiPhaseComponent
{
	//------------------------------------------------------------------------------------------------
	override void ApplyDamagePhaseData(SCR_DamagePhaseData pDamagePhaseData)
	{
		Print(pDamagePhaseData.m_fPhaseHealth);
		Print(pDamagePhaseData.m_PhaseModel);
		//Save current phase material
		ResourceName modelPath;
		string remap;
		ResourceName name = SCR_BaseContainerTools.GetPrefabResourceName(GetOwner().GetPrefabData().GetPrefab());
		SCR_Global.GetModelAndRemapFromResource(name, modelPath, remap);
		//Change to new mesh
		super.ApplyDamagePhaseData(pDamagePhaseData);
		//Apply saved material
		VObject mesh = GetOwner().GetVObject();
		GetOwner().SetObject(mesh, remap);
		
	}
};
