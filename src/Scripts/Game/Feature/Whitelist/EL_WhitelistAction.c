class EL_WhitelistAction : ScriptedUserAction
{
	[Attribute("0", UIWidgets.Flags, "Action can only be used with this whitelist", "", ParamEnumArray.FromEnum(EL_WhitelistType) )]
	protected EL_WhitelistType m_eWhitelistOnlyType;

	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
 	{
		if (!super.CanBePerformedScript(user))
			return false;
		
		//No Whitelist specified
		if (m_eWhitelistOnlyType == 0)
			return true;
		
		//Check if user has at least one whitelist flag set
		EL_PlayerWhitelistComponent whitelistComponent = EL_PlayerWhitelistComponent.Cast(user.FindComponent(EL_PlayerWhitelistComponent));
		if(!whitelistComponent.HasWhitelist(m_eWhitelistOnlyType))
		{
			SetCannotPerformReason("Not allowed");
			return false;
		}
		return true;
	}
}