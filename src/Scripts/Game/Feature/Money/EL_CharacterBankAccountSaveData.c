[EL_ComponentSaveDataType(EL_CharacterBankAccountComponentSaveData, EL_CharacterBankManagerComponent, "BankAccount"), BaseContainerProps()]
class EL_CharacterBankAccountComponentSaveData : EL_ComponentSaveDataBase
{
	ref EL_BankAccount m_BankAccount;

	//------------------------------------------------------------------------------------------------
	override bool ReadFrom(notnull GenericComponent worldEntityComponent)
	{
		EL_CharacterBankManagerComponent bankManager = EL_CharacterBankManagerComponent.Cast(worldEntityComponent);
		m_BankAccount = bankManager.GetAccount();
		if (!m_BankAccount)
			m_BankAccount = EL_BankAccount.Create(string.Empty, 0);
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override bool ApplyTo(notnull GenericComponent worldEntityComponent)
	{
		if (m_BankAccount.GetAccountOwnerUid() != string.Empty)
		{
			EL_CharacterBankManagerComponent bankManager = EL_CharacterBankManagerComponent.Cast(worldEntityComponent);
			bankManager.SetAccount(m_BankAccount);
		}
		return true;
	}
}