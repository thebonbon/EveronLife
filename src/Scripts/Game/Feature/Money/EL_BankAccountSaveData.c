[EL_DbName(EL_BankAccountSaveData, "BankAccount"), BaseContainerProps()]
class EL_BankAccountSaveData : EL_EntitySaveDataBase
{
	ref array<ref EL_BankAccount> m_aBankAccounts = new array<ref EL_BankAccount>();

	//------------------------------------------------------------------------------------------------
	override bool ReadFrom(notnull IEntity worldEntity)
	{
		//Meta data
		EL_PersistenceComponent persistenceComponent = EL_PersistenceComponent.Cast(worldEntity.FindComponent(EL_PersistenceComponent));
		SetId(persistenceComponent.GetPersistentId());
		m_iLastSaved = persistenceComponent.GetLastSaved();

		//Actual Data
		EL_GlobalBankAccountManager bankManager = EL_GlobalBankAccountManager.Cast(worldEntity);
		m_aBankAccounts = bankManager.GetBankAccounts();
		EL_Logger.Log("EL-Bank", string.Format("Saved %1 banks to DB.", m_aBankAccounts.Count()));
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override bool ApplyTo(notnull IEntity worldEntity)
	{
		EL_GlobalBankAccountManager bankManager = EL_GlobalBankAccountManager.Cast(worldEntity);
		bankManager.SetBankAccounts(m_aBankAccounts);
		EL_Logger.Log("EL-Bank", string.Format("Loaded %1 banks from DB.", m_aBankAccounts.Count()));
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override protected bool SerializationSave(BaseSerializationSaveContext saveContext)
	{
		//Meta data
		saveContext.WriteValue("m_iDataLayoutVersion", m_iDataLayoutVersion);
		saveContext.WriteValue("m_sId", GetId());
		saveContext.WriteValue("m_iLastSaved", m_iLastSaved);

		//Actual Data
		saveContext.WriteValue("m_aBankAccounts", m_aBankAccounts);		
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override protected bool SerializationLoad(BaseSerializationLoadContext loadContext)
	{
		//Meta data
		loadContext.ReadValue("m_iDataLayoutVersion", m_iDataLayoutVersion);
		string id;
		loadContext.ReadValue("m_sId", id);
		SetId(id);
		loadContext.ReadValue("m_iLastSaved", m_iLastSaved);

		//Actual Data
		loadContext.ReadValue("m_aBankAccounts", m_aBankAccounts);

		return true;
	}
}