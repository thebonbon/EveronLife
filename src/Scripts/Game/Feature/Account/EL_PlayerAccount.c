[EL_PersistentScriptedStateSettings(EL_PlayerAccount, EL_PlayerAccountSaveData, autoSave: false, shutDownSave: true, selfDelete: false)]
class EL_PlayerAccount : EL_PersistentScriptedStateBase
{
	ref array<string> m_aCharacterIds;
	ref EL_BankAccount m_BankAccount;
	
	//------------------------------------------------------------------------------------------------
	static EL_PlayerAccount Create(string playerUid)
	{
		EL_PlayerAccount account();
		account.m_sId = playerUid;
		account.m_aCharacterIds = new array<string>();
		account.m_BankAccount = EL_BankAccount.Create(playerUid, 1000);
		return account;
	}
}
