[EntityEditorProps(category: "EveronLife/Logging", description: "Logging Area", color: "251 91 0 255", dynamicBox: true)]
class EL_MiningAreaClass: GenericEntityClass
{
};

[BaseContainerProps()]
class EL_WeightedSpawn
{
    [Attribute(defvalue: "50", UIWidgets.Auto, "% Chance to spawn")]
    int m_iChance;
	[Attribute("", UIWidgets.ResourcePickerThumbnail, "Prefab", params: "et")]
	ResourceName m_Prefab;
	
}

class EL_MiningArea : GenericEntity
{
	
	[Attribute("10 10 10", desc: "Area to spawn ores in", category: "Mining Area")]
	private vector m_vSize;
	
	[Attribute("5", UIWidgets.EditBox, desc: "Amount of ores to spawn", category: "Mining Area")]
	private int m_iAmount;	
	
	[Attribute("5", UIWidgets.EditBox, desc: "Time to respawn ores (min)", category: "Mining Area")]
	private float m_fRespawnTime;
	
	[Attribute("", UIWidgets.Object, desc: "Prefab List", category: "Mining Area")]
	private ref array<ref EL_WeightedSpawn> m_PrefabsToSpawn;

	private ref array<IEntity> m_aSpawnedOres;
	private float m_fNextSpawnTime = 0;
	private const int REPL_TO_MIN = 60000;
	
	//------------------------------------------------------------------------------------------------	
	private vector GetRandomPoint()
	{
		vector origin = GetOrigin();
		float rndX = Math.RandomFloatInclusive(-m_vSize[0], m_vSize[0]);
		float rndZ = Math.RandomFloatInclusive(-m_vSize[2], m_vSize[2]);
		float newY = 0;

		return Vector(rndX, newY, rndZ);
	}
	
	//------------------------------------------------------------------------------------------------	
	private int GetTotalWeight()
	{
		int totalWeight = 0;
		foreach (EL_WeightedSpawn weightedSpawnObj : m_PrefabsToSpawn)
		{
			totalWeight += weightedSpawnObj.m_iChance;
		}
		return totalWeight;
	}
	
	//------------------------------------------------------------------------------------------------	
	void SpawnOres(int amount)
	{
		for (int i = 0; i < amount; i++)
		{
			int rndWeight = Math.RandomInt(0,  GetTotalWeight());	
			if (rndWeight == 0)
				return;
			foreach (EL_WeightedSpawn weightedSpawnObj : m_PrefabsToSpawn)
			{
				if (rndWeight < weightedSpawnObj.m_iChance)
				{
					float rndRot = Math.RandomFloat(-180, 180);
					IEntity nextOre = EL_Utils.SpawnEntityPrefab(weightedSpawnObj.m_Prefab, GetRandomPoint(), Vector(0, rndRot, 0), this);
					if (!nextOre.GetParent())
					{
						Print(string.Format("Prefab %1 is missing Hierarchy component!", weightedSpawnObj.m_Prefab.GetPath()), LogLevel.ERROR);
						delete nextOre;
					}
					
					//Snap to ground
					vector transform[4];
					nextOre.GetWorldTransform(transform);
					if(SCR_TerrainHelper.SnapAndOrientToTerrain(transform))
						nextOre.SetOrigin(transform[3]);
		
					break;
				}
				rndWeight -= weightedSpawnObj.m_iChance;
			}
			
		}
	}
	
	//------------------------------------------------------------------------------------------------	
	override void EOnFrame(IEntity owner, float timeSlice)
	{
		if (Replication.Time() >= m_fNextSpawnTime)
		{
			SpawnOres(m_iAmount - SCR_EntityHelper.CountChildren(this));
			m_fNextSpawnTime = Replication.Time() + m_fRespawnTime * REPL_TO_MIN;
		}
	}
	
	//------------------------------------------------------------------------------------------------	
	override void EOnInit(IEntity owner) 
	{
		m_aSpawnedOres = new array<IEntity>();
	}
	
	//------------------------------------------------------------------------------------------------	
	void EL_MiningArea(IEntitySource src, IEntity parent) 
	{
		SetEventMask(EntityEvent.INIT | EntityEvent.FRAME);
		SetFlags(EntityFlags.ACTIVE, true);
	}

	
	#ifdef WORKBENCH
	//------------------------------------------------------------------------------------------------	
	override void _WB_GetBoundBox(inout vector min, inout vector max, IEntitySource src)
	{
		min = -m_vSize;
		max = m_vSize;
	}
	#endif
	
};