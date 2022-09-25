[ComponentEditorProps(category: "GameScripted/Destruction", description: "Multi-Phase destruction component, for objects that go through several damage phases")]
class EL_DestructionMultiPhaseSingleComponentClass: SCR_DestructionMultiPhaseComponentClass
{
};

class EL_DestructionMultiPhaseSingleComponent : SCR_DestructionMultiPhaseComponent
{
	string m_sRemap;
	
	//------------------------------------------------------------------------------------------------
	override void InitDestruction()
	{
		/*
		//Save main materials
		ResourceName name = SCR_BaseContainerTools.GetPrefabResourceName(GetOwner().GetPrefabData().GetPrefab());
		ResourceName modelPath;
		SCR_Global.GetModelAndRemapFromResource(name, modelPath, m_sRemap);
		Print(m_sRemap);
		*/
	}
	
	//------------------------------------------------------------------------------------------------
	override void ApplyDamagePhaseData(SCR_DamagePhaseData pDamagePhaseData)
	{
		
		ResourceName name = SCR_BaseContainerTools.GetPrefabResourceName(GetOwner().GetPrefabData().GetPrefab());
		ResourceName modelPath;
		SCR_Global.GetModelAndRemapFromResource(name, modelPath, m_sRemap);
		
		super.ApplyDamagePhaseData(pDamagePhaseData);
		
		//Apply saved material
		VObject mesh = GetOwner().GetVObject();
		GetOwner().SetObject(mesh, m_sRemap);
		Print(m_sRemap);
	}
};


//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_Spawnable_PrefabTitle()]
class EL_RandomPrefabSpawnable : SCR_BaseSpawnable
{
	[Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, "Prefabs to spawn (spawns ALL of them)", "et")]
	ref array<ResourceName> m_Prefabs;
	[Attribute("0", UIWidgets.CheckBox, "Whether the spawned prefabs should be set as children (sets auto-transform)")]
	bool m_bSpawnAsChildren;
	
	[Attribute("1", UIWidgets.EditBox, desc: "")]
	protected int m_iCount;
	[Attribute("0 0 0", UIWidgets.Coords, desc: "")]
	protected vector m_vRandomOffsetPosition;
	[Attribute("0 0 0", UIWidgets.Coords, desc: "")]
	protected vector m_vRandomOffsetRotation;
	
	//------------------------------------------------------------------------------------------------
	//! Spawns the object
	override void Spawn(IEntity owner, Physics parentPhysics, SCR_HitInfo hitInfo)
	{
		if (!hitInfo)
			return;
		
		int numModelPrefabs = 0;
		if (m_Prefabs)
			numModelPrefabs = m_Prefabs.Count();
		
		for (int i = 0; i < numModelPrefabs; i++)
		{
			ResourceName prefabPath = m_Prefabs[i];
			
			bool isPrefab;
			if (SCR_Global.GetResourceContainsComponent(prefabPath, "RplComponent", isPrefab) && RplSession.Mode() == RplMode.Client)
				continue;
			
			if (!isPrefab)
				continue;
			
			for (int j = 0; j < m_iCount; j++)
			{
				vector spawnMat[4];
				GetSpawnTransform(owner, spawnMat, m_bSpawnAsChildren);
				
				vector rndPos;
				rndPos[0] = Math.RandomFloatInclusive(-m_vRandomOffsetPosition[0], m_vRandomOffsetPosition[0]);
				rndPos[1] = Math.RandomFloatInclusive(-m_vRandomOffsetPosition[1], m_vRandomOffsetPosition[1]);
				rndPos[2] = Math.RandomFloatInclusive(-m_vRandomOffsetPosition[2], m_vRandomOffsetPosition[2]);
				spawnMat[3] = spawnMat[3] + rndPos;
				
				EntitySpawnParams prefabSpawnParams = EntitySpawnParams();
				prefabSpawnParams.Transform = spawnMat;
				
				IEntity spawnedPrefab = GetGame().SpawnEntityPrefab(Resource.Load(prefabPath), null, prefabSpawnParams);
				
				if (!spawnedPrefab)
					continue;
				
				if (m_bSpawnAsChildren)
				{
					owner.AddChild(spawnedPrefab, -1, EAddChildFlags.AUTO_TRANSFORM);
					continue;
				}
			}
		}
	}
};