class EL_ToolHitZone : ScriptedHitZone
{

	[Attribute(ResourceName.Empty, UIWidgets.ResourceNamePicker, "Particle to spawn on hit", "ptc")]
	ResourceName m_HitParticle;
	[Attribute("", UIWidgets.Slider, "Effective tool damage", "0.0 100.0 0.01")]
	float m_fEffectiveDamage;
	[Attribute("0.1", UIWidgets.Slider, "Shake intensity", "0.0 1 0.1")]
	float m_fShakeIntensity;
	[Attribute("3", UIWidgets.Slider, "Shake amount", "0.0 100 1.0")]
	int m_iShakeAmount;

	vector m_vOriginalPos;
	
	//------------------------------------------------------------------------------------------------
	override float ComputeEffectiveDamage(EDamageType damageType, float rawDamage, IEntity hitEntity, HitZone struckHitZone, IEntity damageSource, IEntity damageSourceGunner, IEntity damageSourceParent, const GameMaterial hitMaterial, int colliderID, const inout vector hitTransform[3], const vector impactVelocity, int nodeID, bool isDOT)
	{
		//Check for tool
		if (rawDamage != m_fEffectiveDamage) return 0;

		SCR_ParticleEmitter.Create(m_HitParticle, hitTransform[0], hitTransform[1]);
		
		//Shake "Animation"
		m_vOriginalPos = GetOwner().GetOrigin();
		Shake();
		
		return super.ComputeEffectiveDamage(damageType, rawDamage, hitEntity, struckHitZone, damageSource, damageSourceGunner, damageSourceParent, hitMaterial, colliderID, hitTransform, impactVelocity, nodeID, isDOT);

	}
	
	//------------------------------------------------------------------------------------------------
	void Shake()
	{
		bool negativeOffset;
		for (int i=0; i < m_iShakeAmount; i++)
		{
			vector offset = vector.Right * m_fShakeIntensity;
			if (negativeOffset)
				offset *= -1;
			negativeOffset = !negativeOffset;
			GetGame().GetCallqueue().CallLater(SetNewPos, i * 50, false, m_vOriginalPos + offset);
		}
		GetGame().GetCallqueue().CallLater(SetNewPos, m_iShakeAmount * 50, false, m_vOriginalPos);
	}
	
	//------------------------------------------------------------------------------------------------
	void SetNewPos(vector pos)
	{
		GetOwner().SetOrigin(pos);
	}
};