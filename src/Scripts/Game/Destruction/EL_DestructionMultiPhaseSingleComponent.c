[ComponentEditorProps(category: "GameScripted/Destruction", description: "Multi-Phase destruction component, for objects that go through several damage phases")]
class EL_DestructionMultiPhaseSingleComponentClass: SCR_DestructionMultiPhaseComponentClass
{
};

class EL_DestructionMultiPhaseSingleComponent : SCR_DestructionMultiPhaseComponent
{
	//------------------------------------------------------------------------------------------------
	override void ApplyDamagePhaseData(SCR_DamagePhaseData pDamagePhaseData)
	{
		if (!pDamagePhaseData)
		{
			SetHitZoneHealth(m_iOriginalHealth);
			GetDestructionBaseData().SetPreviousHealth(GetHealth());
			SetModel(GetOriginalResourceName());
			return;
		}
		
		GetDestructionBaseData().SetPreviousHealth(pDamagePhaseData.m_fPhaseHealth);
		SetHitZoneHealth(pDamagePhaseData.m_fPhaseHealth);
		//Don't set model if none specified
		if (pDamagePhaseData.m_PhaseModel)
			SetModel(pDamagePhaseData.m_PhaseModel);	
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

//------------------------------------------------------------------------------------------------
class EL_Spawnable_HitParticleTitle : BaseContainerCustomTitle
{
	override bool _WB_GetCustomTitle(BaseContainer source, out string title)
	{
		title = "Hit Particle Effect";
		return true;
	}
};

//------------------------------------------------------------------------------------------------
[BaseContainerProps(), EL_Spawnable_HitParticleTitle()]
class EL_HitParticleSpawnable : SCR_BaseSpawnable
{
	[Attribute(ResourceName.Empty, UIWidgets.ResourceNamePicker, "Particle effect to spawn", "ptc")]
	ResourceName m_Particle;
	
	//------------------------------------------------------------------------------------------------
	//! Spawns the object
	override void Spawn(IEntity owner, Physics parentPhysics, SCR_HitInfo hitInfo)
	{
		if (!hitInfo || m_Particle == ResourceName.Empty)
			return;
		
		vector spawnMat[4];
		GetSpawnTransform(owner, spawnMat);

		vector newRight, newUp, newForward;
			
		newUp = -hitInfo.m_HitDirection;
		newRight = newUp * spawnMat[2];
		newForward = newUp * newRight;
			
		spawnMat[0] = newRight;
		spawnMat[1] = newUp;
		spawnMat[2] = newForward;
		spawnMat[3] = hitInfo.m_HitPosition;
		
		
		SCR_DestructionCommon.PlayParticleEffect_Transform(m_Particle, hitInfo.m_DamageType, spawnMat);
	}
};

//------------------------------------------------------------------------------------------------
class EL_ChangeSizeTitle : BaseContainerCustomTitle
{
	override bool _WB_GetCustomTitle(BaseContainer source, out string title)
	{
		title = "Size Change";
		return true;
	}
};

//------------------------------------------------------------------------------------------------
[BaseContainerProps(), EL_ChangeSizeTitle()]
class EL_SizeChange : SCR_BaseSpawnable
{
	[Attribute("1", UIWidgets.EditBox, desc: "New size")]
	protected float m_fScale;
	
	//------------------------------------------------------------------------------------------------
	override void Spawn(IEntity owner, Physics parentPhysics, SCR_HitInfo hitInfo)
	{
		owner.SetScale(m_fScale);
	}
};