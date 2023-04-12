class AdminChatChannel : BaseChatChannel
{
	//------------------------------------------------------------------------------------------------
	void RequestClose()
	{
		GetGame().RequestClose();
	}
	
	//------------------------------------------------------------------------------------------------
	override bool ProcessMessage(BaseChatComponent sender, string message, bool isAuthority)
	{
		if (!isAuthority)
			return true;

		//!wadd TYPE UID
		//eg. "!wadd POLICE 544-242-544-242"
		if (message.StartsWith("!wadd"))
		{
			array<string> strs = new array<string>();
			message.Split(" ", strs, true);
			if (strs.Count() != 3)
				return false;

			EL_WhitelistType type = typename.StringToEnum(EL_WhitelistType, strs[1]);
			if (type == -1)
				return false;

			EL_WhitelistRespawnHandlerComponent whitelistComponent = EL_WhitelistRespawnHandlerComponent.Cast(GetGame().GetGameMode().FindComponent(EL_WhitelistRespawnHandlerComponent));
			whitelistComponent.EnableWhitelistOnPlayer(strs[2], type);
		}
		
		//!shutdown TIME (in seconds)
		//eg.: !shutdown 60
		if (message.StartsWith("!shutdown"))
		{
			array<string> strs = new array<string>();
			message.Split(" ", strs, true);
			if (strs.Count() != 2)
				return false;	

			float delay = strs[1].ToFloat();
			GetGame().GetCallqueue().CallLater(RequestClose, delay * 60000);
			SCR_PlayerController senderPC = SCR_PlayerController.Cast(sender.GetOwner());
			EL_RpcSenderComponent rplSender = EL_RpcSenderComponent.Cast(senderPC.GetMainEntity().FindComponent(EL_RpcSenderComponent));
			rplSender.AskSendGlobalHint(delay, "SERVER",  "Server restart in " + delay + " minute(s)");
		}
		
		//!hint TIME TITLE DESCRIPTION
		//eg.: !hint 60 MyTitle Hello World !
		if (message.StartsWith("!hint"))
		{
			array<string> strs = new array<string>();
			message.Split(" ", strs, true);
			if (strs.Count() < 4)
				return false;
			
			SCR_PlayerController senderPC = SCR_PlayerController.Cast(sender.GetOwner());
			EL_RpcSenderComponent rplSender = EL_RpcSenderComponent.Cast(senderPC.GetMainEntity().FindComponent(EL_RpcSenderComponent));
			string desc;
			for (int i; i < strs.Count() - 3; i++)
			{
				desc += strs[i + 3];
				desc += " ";
			}
			rplSender.AskSendGlobalHint(strs[1].ToFloat(), strs[2], desc);
		}
		
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override bool IsDelivering(BaseChatComponent sender, BaseChatComponent receiver)
	{
		return IsAvailable(receiver);
	}

	//------------------------------------------------------------------------------------------------
	override bool IsAvailable(BaseChatComponent sender)
	{
		SCR_PlayerController senderPC = SCR_PlayerController.Cast(sender.GetOwner());
		return (senderPC && GetGame().GetPlayerManager().HasPlayerRole(senderPC.GetPlayerId(), EPlayerRole.ADMINISTRATOR));
	}
}
