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
		
		VObject mesh = GetOwner().GetVObject();
		if (mesh)
		{
			string remap;
			string materials[256];
			int numMats = mesh.GetMaterials(materials);
			
			super.ApplyDamagePhaseData(pDamagePhaseData);
			
			for (int i = 0; i < numMats; i++)
			{
				Print(materials[i]);
				//remap += string.Format("$remap '%1' '%2';", materials[i], material);
			}
			GetOwner().SetObject(mesh, remap);
		}
	}
#endif
};
