class EL_ChangeTimeAction : ScriptedUserAction
{
	[Attribute(defvalue:"12", UIWidgets.Slider, desc: "Target Time", "0 23 1")]
	private int m_fTargetTime;

	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		RplComponent replication = RplComponent.Cast(pOwnerEntity.FindComponent(RplComponent));
		if (replication && !replication.IsOwner()) return;
		
		TimeAndWeatherManagerEntity weatherManager = GetGame().GetTimeAndWeatherManager();
		if (!weatherManager) return;
		
		weatherManager.SetTimeOfTheDay(m_fTargetTime, true);
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
 	{
		return true;
 	}
}

class AdminChatChannel : BaseChatChannel
{
	//------------------------------------------------------------------------------------------------
	void DelayedShutdown()
	{
		GetGame().RequestClose();
	}	
	
	//------------------------------------------------------------------------------------------------
	override bool ProcessMessage(BaseChatComponent sender, string message, bool isAuthority)
	{
		if (!isAuthority)
			return true;
		
		//!shutdown TIME
		if (message.StartsWith("!shutdown"))
		{
			array<string> strs = new array<string>();
			message.Split(" ", strs, true);
			if (strs.Count() != 2)
				return false;	

			float delay = strs[1].ToFloat();
			GetGame().GetCallqueue().CallLater(DelayedShutdown, delay * 60000);
			SCR_PlayerController senderPC = SCR_PlayerController.Cast(sender.GetOwner());
			EL_RpcSenderComponent rplSender = EL_RpcSenderComponent.Cast(senderPC.GetMainEntity().FindComponent(EL_RpcSenderComponent));
			rplSender.AskSendGlobalHint(delay, "SERVER",  "Server restart in " + delay + " minute(s)");
		}
		//!hint TIME TITLE DESCRIPTION 
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
		SCR_PlayerController receiverPC = SCR_PlayerController.Cast(receiver.GetOwner());
		if (!receiverPC)
			return false;

		return GetGame().GetPlayerManager().HasPlayerRole(receiverPC.GetPlayerId(), EPlayerRole.ADMINISTRATOR);
	}

	//------------------------------------------------------------------------------------------------
	override bool IsAvailable(BaseChatComponent sender)
	{
		SCR_PlayerController senderPC = SCR_PlayerController.Cast(sender.GetOwner());
		if (!senderPC)
			return false;

		return GetGame().GetPlayerManager().HasPlayerRole(senderPC.GetPlayerId(), EPlayerRole.ADMINISTRATOR);
	}
}