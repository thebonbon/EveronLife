[EntityEditorProps(category: "EveronLife/Items")]
class EL_SpikeTriggerComponentClass : ScriptComponentClass
{
};

class EL_SpikeTriggerComponent : ScriptComponent
{
	[Attribute("0 0 0", UIWidgets.EditBox, "Box")]
	protected vector m_vMins;
	[Attribute("0 0 0", UIWidgets.EditBox, "Box")]
	protected vector m_vMaxs;
	
	
	SCR_BaseTriggerEntity m_SpikeTrigger;
	ref array<SCR_WheelHitZone> m_aWheelHitZones = {};
	
	
	//------------------------------------------------------------------------------------------------
	void GetAllWheelHitZones(IEntity ent)
	{
		VehicleWheeledSimulation simulation = VehicleWheeledSimulation.Cast(ent.FindComponent(VehicleWheeledSimulation));
		SCR_VehicleDamageManagerComponent vehicleDamageManager = SCR_VehicleDamageManagerComponent.Cast(ent.FindComponent(SCR_VehicleDamageManagerComponent));
		array<HitZone> hitZones = {};
		vehicleDamageManager.GetAllHitZones(hitZones);
		m_aWheelHitZones.Resize(simulation.WheelCount());
		
		foreach (HitZone hitZone : hitZones)
		{
			if (hitZone && hitZone.Type() == SCR_WheelHitZone)
			{
				SCR_WheelHitZone wheelHitZone = SCR_WheelHitZone.Cast(hitZone);
				
				int wheelIndex = simulation.WheelGetIndex(hitZone.GetName());
				m_aWheelHitZones.InsertAt(wheelHitZone, wheelIndex);
			}
		}
		
	}

	//------------------------------------------------------------------------------------------------
	void VehicleEnterTrigger(IEntity ent)
	{
		VehicleWheeledSimulation simulation = VehicleWheeledSimulation.Cast(ent.FindComponent(VehicleWheeledSimulation));

		GetAllWheelHitZones(ent);
		
		for (int i; i < m_aWheelHitZones.Count(); i++)
		{
			vector wheelPos = simulation.WheelGetPosition(i);
			wheelPos = ent.CoordToParent(wheelPos);
			
			bool inSpikes = Math3D.IntersectionBoxBox(m_vMins, m_vMaxs, wheelPos, wheelPos);
			
			if (inSpikes)
			{
				m_aWheelHitZones[i].SetHealth(0);
				Print("Wheel index spiked: " + i);
			}
		}
	
	}
	
	//------------------------------------------------------------------------------------------------
	override void EOnInit(IEntity owner)
	{
		m_SpikeTrigger = SCR_BaseTriggerEntity.Cast(owner);
		
		m_vMins = m_SpikeTrigger.CoordToParent(m_vMins);
		m_vMaxs = m_SpikeTrigger.CoordToParent(m_vMaxs);
		m_SpikeTrigger.GetOnActivate().Insert(VehicleEnterTrigger);
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{
		SetEventMask(owner, EntityEvent.INIT);
		owner.SetFlags(EntityFlags.ACTIVE, false);
	}
}
