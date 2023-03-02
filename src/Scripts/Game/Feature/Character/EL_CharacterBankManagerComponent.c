[EntityEditorProps(category: "EveronLife/Bank", description: "Player Bank Manager", color: "0 0 255 255")]
class EL_CharacterBankManagerComponentClass: ScriptGameComponentClass
{
}

class EL_CharacterBankManagerComponent : ScriptGameComponent
{
	[Attribute(defvalue:"1000", UIWidgets.EditBox, desc: "Starting account balance")]
	protected int m_iStartAccountBalance;

	ref EL_BankAccount m_LocalBankAccount;

	//------------------------------------------------------------------------------------------------
	void Ask_NewTransaction(int amount, string comment)
	{
		if (Replication.IsServer())
			Do_NewTransaction(amount, comment);
		else
			Rpc(Do_NewTransaction, amount, comment);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void Do_NewTransaction(int amount, string comment)
	{
		bool success;
		if (amount == 0)
			return;
		if (amount > 0)
			success = m_LocalBankAccount.TryDeposit(amount, comment);
		else
			success = m_LocalBankAccount.TryWithdraw(-amount, comment);
		if (success)
		{
			Print("[EL-Bank] Transaction Success!");
			Ask_UpdateProxyAccount();
		}
		else
			Print("[EL-Bank] Transaction Error!");
	}

	//------------------------------------------------------------------------------------------------
	EL_BankAccount GetAccount()
	{
		return m_LocalBankAccount;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Called from Authority
	void SetAccount(EL_BankAccount newAccount)
	{
		m_LocalBankAccount = newAccount;
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void Rpc_DoSetLocalBankAccount(EL_BankAccount account, array<string> transactionComments)
	{
		//Update account
		m_LocalBankAccount = account;
		
		//Update transactions comments
		foreach (int i, string comment : transactionComments)
		{
			m_LocalBankAccount.m_aTransactions[i].m_sComment = comment;
		}
		
		Print("[EL-Bank] Received bank account update");
		//Update open bank ui
		EL_BankMenu bankMenu = EL_BankMenu.Cast(GetGame().GetMenuManager().FindMenuByPreset(ChimeraMenuPreset.EL_BankMenu));
		if (bankMenu)
			bankMenu.OnMenuFocusGained();
	}
	
	//------------------------------------------------------------------------------------------------
	void Ask_UpdateProxyAccount()
	{
		//Workaround to send strings
		array<string> transactionComments = {};
		foreach (EL_BankTransaction transaction : m_LocalBankAccount.m_aTransactions)
		{
			transactionComments.Insert(transaction.m_sComment);
		}
		Rpc(Rpc_DoSetLocalBankAccount, m_LocalBankAccount, transactionComments);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Called from Authority
	void LoadOrCreateAccount()
	{
		//If Account was already loaded by persistency update proxy
		if (m_LocalBankAccount)
		{
			Ask_UpdateProxyAccount();
			return;
		}
		
		Print("[EL-Bank] Server created new Bank Account for " + EL_Utils.GetPlayerName(GetOwner()));
		SetAccount(EL_BankAccount.Create(EL_Utils.GetPlayerUID(GetOwner()), m_iStartAccountBalance));
	}
	
	//------------------------------------------------------------------------------------------------
	override void EOnInit(IEntity owner)
	{
		if (Replication.IsServer())
			GetGame().GetCallqueue().CallLater(LoadOrCreateAccount, 10000, false);
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{
		SetEventMask(owner, EntityEvent.INIT);
	}
}