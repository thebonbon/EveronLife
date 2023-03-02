class EL_OpenBankMenuAction : ScriptedUserAction
{
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.EL_BankMenu);
	}
	
	//------------------------------------------------------------------------------------------------
	override bool HasLocalEffectOnlyScript()
	{
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		outName = "Open Bank";
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
	{
		EL_CharacterBankManagerComponent bankManager = EL_ComponentFinder<EL_CharacterBankManagerComponent>.Find(user);
		return (bankManager.GetAccount());
	}
	
	//------------------------------------------------------------------------------------------------
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		SetCannotPerformReason("Loading Bank Account..")		
	}
	
}