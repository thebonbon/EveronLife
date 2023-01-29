[EntityEditorProps(category: "EveronLife/Core", description: "Core gamemode")]
class EL_GameModeRoleplayClass: SCR_BaseGameModeClass
{
}

class EL_GameModeRoleplay: SCR_BaseGameMode
{
	ref map<string, ref array<IEntity>> m_mVehicleOwnerList = new map<string, ref array<IEntity>>();

	//------------------------------------------------------------------------------------------------
	void AddVehicleOwnerCache(string owner, IEntity ent)
	{
		ref array<IEntity> ownedVehicles();
		if (!m_mVehicleOwnerList.Find(owner, ownedVehicles))
			ownedVehicles = new array<IEntity>();
		ownedVehicles.Insert(ent);
		m_mVehicleOwnerList.Set(owner, ownedVehicles);
	}

	//------------------------------------------------------------------------------------------------
	override void OnPlayerSpawned(int playerId, IEntity controlledEntity)
	{
		super.OnPlayerSpawned(playerId, controlledEntity);

		ref array<IEntity> ownedVehicles();
		if (m_mVehicleOwnerList.Find(EL_Utils.GetPlayerUID(playerId), ownedVehicles))
		{
			foreach (IEntity vehicle : ownedVehicles)
			{
				//Set local owner on client vehicle instance
				EL_RpcSenderComponent rpcSender = EL_RpcSenderComponent.Cast(controlledEntity.FindComponent(EL_RpcSenderComponent));
				rpcSender.AskSetLocalVehicleOwner(Replication.FindId(vehicle));
				
				//Set uid in server vehicle instance
				EL_CharacterOwnerComponent charOwnerComp = EL_CharacterOwnerComponent.Cast(vehicle.FindComponent(EL_CharacterOwnerComponent));
				charOwnerComp.SetCharacterOwner(EL_Utils.GetPlayerUID(playerId));
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void ~EL_GameModeRoleplay()
	{
		EL_PlayerAccountManager.Reset();
	}
}
