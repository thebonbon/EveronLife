[EntityEditorProps(category: "EveronLife/Spawning", description: "Spawn Zone", color: "251 91 0 255", dynamicBox: true)]
class EL_SpawnZoneClass: GenericEntityClass
{
};

class EL_SpawnZone : GenericEntity
{
	[Attribute("10", desc: "Area to prefabs in", category: "Spawning Zone")]
	private float m_fSpawnRadius;
	
	[Attribute("", UIWidgets.ResourcePickerThumbnail, desc: "Prefab to spawn", category: "Spawning Zone", params: "et")]
	private ResourceName m_PrefabToSpawn;
	
	[Attribute("5", UIWidgets.EditBox, desc: "Amount of preabs to spawn", category: "Spawning Zone")]
	private int m_iAmount;	
	
	[Attribute("5", UIWidgets.EditBox, desc: "Time to respawn prefabs (min)", category: "Spawning Zone")]
	private float m_fRespawnTime;	

	float timer;
	ref RandomGenerator m_RandomGenerator = new RandomGenerator();
	ref array<IEntity> m_aSpawnedEnts = new ref array<IEntity>();
	ref array<IEntity> m_aSpawnedEntsCopy = new ref array<IEntity>();
	
	//------------------------------------------------------------------------------------------------	
	void SpawnPrefabs()
	{
		m_aSpawnedEntsCopy.Copy(m_aSpawnedEnts);
		
		foreach (IEntity ent : m_aSpawnedEntsCopy)
		{
			InventoryItemComponent invComp = InventoryItemComponent.Cast(ent.FindComponent(InventoryItemComponent));
			if (!invComp.GetParentSlot()) //Is on Ground?
					continue;
			Print("Removing ent..");
			m_aSpawnedEnts.RemoveItem(ent);
			vector rndPoint = m_RandomGenerator.GenerateRandomPointInRadius(0, m_fSpawnRadius, GetOrigin());
			
			IEntity newEnt = EL_Utils.SpawnEntityPrefab(m_PrefabToSpawn, rndPoint);
			Print("new ent" + newEnt);
			m_aSpawnedEnts.Insert(newEnt);
		}
		m_aSpawnedEntsCopy.Clear();
	}

	//------------------------------------------------------------------------------------------------	
	void FirstSpawn()
	{
		for (int i = 0; i < m_iAmount; i++)
		{
			vector rndPoint = m_RandomGenerator.GenerateRandomPointInRadius(0, m_fSpawnRadius, GetOrigin());
			rndPoint[1] = GetOrigin()[1];
			IEntity ent = EL_Utils.SpawnEntityPrefab(m_PrefabToSpawn, rndPoint);
			m_aSpawnedEnts.Insert(ent);
		}
	}

	//------------------------------------------------------------------------------------------------	
	override void EOnFrame(IEntity owner, float timeSlice)
	{
		
		timer -= timeSlice;
		if (timer <= 0)
		{
			SpawnPrefabs();
			timer = m_fRespawnTime * 1000;
		}
	}
	
	//------------------------------------------------------------------------------------------------	
	override void EOnInit(IEntity owner)
	{
		if (!GetGame().InPlayMode())
			return;
		FirstSpawn();
		timer = m_fRespawnTime * 1000;
		
	}
	
	//------------------------------------------------------------------------------------------------	
	void EL_SpawnZone(IEntitySource src, IEntity parent) 
	{
		SetEventMask(EntityEvent.INIT | EntityEvent.FRAME);
		SetFlags(EntityFlags.ACTIVE, true);
	}
};