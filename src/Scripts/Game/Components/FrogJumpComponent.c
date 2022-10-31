[EntityEditorProps(category: "Frog/Jump")]
class FrogJumpComponentClass : ScriptComponentClass
{
};

class FrogJumpComponent : ScriptComponent
{

	[Attribute("2", UIWidgets.EditBox, "JUMP")]
	protected float m_fJumpVelocity;
	
	[Attribute("5", UIWidgets.EditBox, "JUMP")]
	protected float m_fJumpHeight;	
	
	[Attribute("5", UIWidgets.EditBox, "JUMP")]
	protected float m_fMaxJumpDistance;
	
	
	[Attribute("5", UIWidgets.EditBox, "Sec to jump")]
	protected float m_fMinJumpDelay;
	[Attribute("10", UIWidgets.EditBox, "Sec to jump")]
	protected float m_fMaxJumpDelay;
	
	Physics phys;
	private ref RandomGenerator m_RandomGenerator = new RandomGenerator;
	
	float timer;
	vector m_vStartPos;
	
	//------------------------------------------------------------------------------------------------
	void Jump()
	{
		float rndVelX = m_RandomGenerator.RandFloatXY(-m_fJumpVelocity, m_fJumpVelocity);
		float rndVelZ = m_RandomGenerator.RandFloatXY(-m_fJumpVelocity, m_fJumpVelocity);
		
		vector rndVel = Vector(rndVelX, m_fJumpHeight, rndVelZ);
		vector velRot = Vector(rndVelX, 0, rndVelZ);
		
		float nextJumpDistToCenter = (GetOwner().GetOrigin() + velRot - m_vStartPos).Length();
		float curDistToCenter = (GetOwner().GetOrigin() - m_vStartPos).Length();
		
		if (nextJumpDistToCenter > curDistToCenter && nextJumpDistToCenter > m_fMaxJumpDistance)
		{
			//Jump would be outside or even further away from area, negate it
			rndVel[0] = -rndVel[0];
			rndVel[2] = -rndVel[2];
			velRot *= -1;
		}
				
		velRot = velRot.Normalized().VectorToAngles();
		GetOwner().SetYawPitchRoll(velRot);
		
		phys.SetVelocity(rndVel);
	}
	
	//------------------------------------------------------------------------------------------------
	override void EOnFixedFrame(IEntity owner, float timeSlice)
	{
		if (!phys)
			return;
		
		timer -= timeSlice;
		if (timer <= 0)
		{
			timer = m_RandomGenerator.RandFloatXY(m_fMinJumpDelay, m_fMaxJumpDelay);
			Jump();
		}			
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{
		m_vStartPos = owner.GetOrigin();
		phys = owner.GetPhysics();
		timer = m_RandomGenerator.RandFloatXY(m_fMinJumpDelay, m_fMaxJumpDelay);
		
		SetEventMask(owner, EntityEvent.FIXEDFRAME);
		owner.SetFlags(EntityFlags.ACTIVE, true);
	}
};