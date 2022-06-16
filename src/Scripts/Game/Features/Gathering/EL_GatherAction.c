class EL_GatherAction : ScriptedUserAction
{
	[Attribute("", UIWidgets.EditBox, desc: "Display name of what is being gathered")]
	private string m_GatherItemDisplayName;
	
	[Attribute("", UIWidgets.ResourceNamePicker, desc: "Prefab what item is gathered", "et")]
	private ResourceName m_GatherItemPrefab;
	
	[Attribute("false", UIWidgets.ResourceNamePicker, desc: "Delete the object after gathering?")]
	private bool m_bDeleteAfterUse;
	
	[Attribute("true", UIWidgets.ResourceNamePicker, desc: "Gather Action always active?")]
	private bool m_bAlwaysActive;
	
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		// Play sound
		RplComponent replication = RplComponent.Cast(pOwnerEntity.FindComponent(RplComponent));
		
		//Todo: Replace SCR_InventoryStorageManagerComponent RPL setup with own function or better synced sound alternative
		SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
		inventoryManager.PlayItemSound(replication.Id(), "SOUND_PICK_UP");
		
		//Play grab animation
		CharacterControllerComponent controller = CharacterControllerComponent.Cast(pUserEntity.FindComponent(CharacterControllerComponent));
		if (controller)
			controller.TryPlayItemGesture(EItemGesture.EItemGesturePickUp, this, "Character_ActionGrab");
		
		//Spawn item
		inventoryManager.TrySpawnPrefabToStorage(m_GatherItemPrefab);
		
		//Show hint what to do with the gathered item
		EL_GameModeRoleplay.GetInstance().ShowInitalTraderHint();
		
		//Delete entity after use?
		if (m_bDeleteAfterUse)
			delete(pOwnerEntity);
			
	}		
	
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		outName = string.Format("Gather %1", m_GatherItemDisplayName);
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
 	{
		EL_BaseCrop baseCrop = EL_BaseCrop.Cast(GetOwner());
		if (baseCrop)
			return baseCrop.CanGather();
		
		//Return for everything but crops
		return m_bAlwaysActive;
 	}
	
	//------------------------------------------------------------------------------------------------
	//! Hide action until CanBePerformed to not show "Action [Unavailable]"
	override bool CanBeShownScript(IEntity user)
	{
		return CanBePerformedScript(user);
	}
}
