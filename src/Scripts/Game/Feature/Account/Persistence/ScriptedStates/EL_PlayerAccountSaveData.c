[EPF_PersistentScriptedStateSettings(EL_BankAccount), EDF_DbName.Automatic()]
class EL_PlayerBankAccountSaveData :  EPF_ScriptedStateSaveData
{
	ref array<string> m_aCharacterIds;
	ref EL_BankAccount m_BankAccount;
}
