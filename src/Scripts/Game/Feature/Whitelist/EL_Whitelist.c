[BaseContainerProps()]
class EL_Whitelist
{
	[Attribute("", UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(EL_WhitelistType))]
	EL_WhitelistType m_eType;

	[Attribute("", UIWidgets.FileNamePicker)]
	string m_sWhitelistFilePath;

	ref array<string> m_aWhitelistUuids = new array<string>();

	//------------------------------------------------------------------------------------------------
	bool VerifyPlayer(string uuid)
	{
		PrintFormat("[%1-WHITELIST] Verifing UUID: %2 -> %3!", SCR_Enum.GetEnumName(EL_WhitelistType, m_eType), uuid, m_aWhitelistUuids.Contains(uuid).ToString());
		return m_aWhitelistUuids.Contains(uuid);
	}

	//------------------------------------------------------------------------------------------------
	void LoadUuidsFromFile()
	{
		if (!Replication.IsServer())
			return;

		if(!FileIO.FileExist(m_sWhitelistFilePath))
		{
			PrintFormat("[%1-WHITELIST] Whitelist File $2 does not exist!", SCR_Enum.GetEnumName(EL_WhitelistType, m_eType), m_sWhitelistFilePath);
			return;
		}
		
		m_aWhitelistUuids.Clear();
		FileHandle whitelistFile = FileIO.OpenFile(m_sWhitelistFilePath, FileMode.READ);
		
		if(whitelistFile == 0)
		{
			PrintFormat("[%1-WHITELIST] Whitelist File is empty!", SCR_Enum.GetEnumName(EL_WhitelistType, m_eType));
			return;
		}

		string uuid;
		while(whitelistFile.FGets(uuid) > 0)
		{
			m_aWhitelistUuids.Insert(uuid);
			PrintFormat("[%1-WHITELIST] Loaded UUID: " + uuid, SCR_Enum.GetEnumName(EL_WhitelistType, m_eType));
		}

		PrintFormat("[%1-WHITELIST] Loaded " + m_aWhitelistUuids.Count() + " UUIDs from file.", SCR_Enum.GetEnumName(EL_WhitelistType, m_eType));
		whitelistFile.Close();
	}

	//------------------------------------------------------------------------------------------------
	void AddNewUuidToFile(string uuid)
	{
		if (!Replication.IsServer())
			return;

		if(!FileIO.FileExist(m_sWhitelistFilePath))
			return;

		FileHandle whitelistFile = FileIO.OpenFile(m_sWhitelistFilePath, FileMode.APPEND);
		whitelistFile.FPrintln(uuid);
		whitelistFile.Close();

		PrintFormat("[%1-WHITELIST] Added " + uuid + " to file.", SCR_Enum.GetEnumName(EL_WhitelistType, m_eType));

		m_aWhitelistUuids.Insert(uuid);
	}

	//------------------------------------------------------------------------------------------------
	void RemoveUUIDFromFile(string uuid)
	{
		if (!Replication.IsServer())
			return;

		if (!m_aWhitelistUuids.Contains(uuid))
			return;
		m_aWhitelistUuids.RemoveItem(uuid);
		
		//Clear and create again
		FileIO.DeleteFile(m_sWhitelistFilePath);
		FileHandle whitelistFile = FileIO.OpenFile(m_sWhitelistFilePath, FileMode.WRITE);
		foreach(string uid : m_aWhitelistUuids)
		{
			whitelistFile.FPrintln(uid);
		}
		whitelistFile.Close();
		
		PrintFormat("[%1-WHITELIST] Removed %2 from file.", SCR_Enum.GetEnumName(EL_WhitelistType, m_eType), uuid);
	}
	
	void EL_Whitelist()
	{
		if (GetGame().InPlayMode())
			LoadUuidsFromFile();
	}
}

enum EL_WhitelistType
{
	NONE,
	CONNECT = 1 << 0,
	POLICE = 1 << 1,
	MEDIC = 1 << 2
}