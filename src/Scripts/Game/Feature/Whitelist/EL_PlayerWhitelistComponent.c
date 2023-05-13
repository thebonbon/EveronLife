[ComponentEditorProps(category: "EveronLife/Feature/Whitelist", description: "Local copy of whitelists to allow action checks")]
class EL_PlayerWhitelistComponentClass : ScriptComponentClass
{
}

class EL_PlayerWhitelistComponent : ScriptComponent
{
	bool m_bHideChat;
	protected EL_WhitelistType m_ePlayerWhitelists;

	//------------------------------------------------------------------------------------------------
	void AskRpcDisableWhitelist(EL_WhitelistType flags)
	{
		Rpc(Rpc_DoDisableWhitelist, flags);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void Rpc_DoDisableWhitelist(EL_WhitelistType flags)
	{
		m_ePlayerWhitelists &= ~flags
	}	
	
	//------------------------------------------------------------------------------------------------
	void AskRpcEnableWhitelist(EL_WhitelistType flags)
	{
		Rpc(Rpc_DoEnableWhitelist, flags);
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void Rpc_DoEnableWhitelist(EL_WhitelistType flags)
	{
		m_ePlayerWhitelists |= flags
	}
	
	//------------------------------------------------------------------------------------------------
	void AskRpcSetWhitelists(EL_WhitelistType flags)
	{
		Rpc(Rpc_DoSetWhitelists, flags);
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void Rpc_DoSetWhitelists(EL_WhitelistType flags)
	{
		m_ePlayerWhitelists = flags
	}

	//------------------------------------------------------------------------------------------------
	bool HasWhitelist(EL_WhitelistType flags)
	{
		return SCR_Enum.HasPartialFlag(m_ePlayerWhitelists, flags);
	}
}
