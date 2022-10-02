class EL_ToolHitZone : ScriptedHitZone
{

	[Attribute("", UIWidgets.ResourceNamePicker, "Particle to spawn on hit", "et")]
	ResourceName m_HitVFX;
	[Attribute("", UIWidgets.Slider, "Effective tool damage", "0.0 100.0 0.01")]
	float m_fEffectiveDamage;

	//------------------------------------------------------------------------------------------------
	override float ComputeEffectiveDamage(EDamageType damageType, float rawDamage, IEntity hitEntity, HitZone struckHitZone, IEntity damageSource, IEntity damageSourceGunner, IEntity damageSourceParent, const GameMaterial hitMaterial, int colliderID, const inout vector hitTransform[3], const vector impactVelocity, int nodeID, bool isDOT)
	{
		//Wrong Tool
		if (rawDamage !=  m_fEffectiveDamage) return 0;
		
		return super.ComputeEffectiveDamage(damageType, rawDamage, hitEntity, struckHitZone, damageSource, damageSourceGunner, damageSourceParent, hitMaterial, colliderID, hitTransform, impactVelocity, nodeID, isDOT);
	}

	//------------------------------------------------------------------------------------------------
	override void OnLocalDamage(EDamageType type, float damage, HitZone pOriginalHitzone, IEntity damageSource, IEntity instigator, inout vector hitTransform[3], float speed, int colliderID, int nodeID)
	{
		EL_Utils.SpawnEntityPrefab(m_HitVFX, hitTransform[0], hitTransform[1]);
	}

};