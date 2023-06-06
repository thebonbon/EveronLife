[EPF_ComponentSaveDataType(EL_CharacterBankManagerComponent), BaseContainerProps()]
class EL_CharacterBankAccountComponentSaveDataClass : EPF_ComponentSaveDataClass
{
};

[EDF_DbName.Automatic()]
class EL_CharacterBankAccountComponentSaveData : EPF_ComponentSaveData
{
	ref EL_BankAccount m_BankAccount;

    //------------------------------------------------------------------------------------------------
    override EPF_EReadResult ReadFrom(IEntity owner, GenericComponent component, EPF_ComponentSaveDataClass attributes)
    {		
		EL_CharacterBankManagerComponent bankManager = EL_CharacterBankManagerComponent.Cast(component);
		m_BankAccount = bankManager.GetAccount();
		return EPF_EReadResult.OK;
    }

    //------------------------------------------------------------------------------------------------
    override EPF_EApplyResult ApplyTo(IEntity owner, GenericComponent component, EPF_ComponentSaveDataClass attributes)
    {	
		EL_CharacterBankManagerComponent bankManager = EL_CharacterBankManagerComponent.Cast(component);
		bankManager.SetAccount(m_BankAccount);
		return EPF_EApplyResult.OK;
    }		
}
