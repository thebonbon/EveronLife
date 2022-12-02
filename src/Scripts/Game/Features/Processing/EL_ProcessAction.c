[BaseContainerProps()]
class EL_ProcessingInput
{
	[Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, "Prefab to Input", "et")]
	ResourceName m_InputPrefab;

	[Attribute(defvalue: "1", uiwidget: UIWidgets.Auto, desc: "Input/s amount per process")]
	int m_iInputAmount;
}

[BaseContainerProps()]
class EL_ProcessingOutput
{
	[Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, "Prefab to Output", "et")]
	ResourceName m_OutputPrefab;

	[Attribute(defvalue: "1", uiwidget: UIWidgets.Auto, desc: "Output amount per process")]
	int m_iOutputAmount;
}

class EL_ProcessAction : ScriptedUserAction
{
	[Attribute("", UIWidgets.Object, "List of inputs")]
	ref array<ref EL_ProcessingInput> m_aProcessingInputs;

	[Attribute("", UIWidgets.Object, "List of outputs")]
	ref array<ref EL_ProcessingOutput> m_aProcessingOutputs;
	
	[Attribute(ResourceName.Empty, UIWidgets.ResourceNamePicker, "Particle to spawn on process", "ptc")]
	ResourceName m_ProcessParticle;
	
	[Attribute("0 0 0", UIWidgets.EditBox, "Particle Offset", params: "inf inf 0 purposeCoords spaceEntity")]
	vector m_vParticleOffset;
	
	[Attribute("0 0 0", UIWidgets.EditBox, "Particle Rotation")]
	vector m_vParticleRotation;
	
	[Attribute("0", UIWidgets.EditBox, "Time to process (s)")]
	float m_fProcessTime;

	[Attribute("", UIWidgets.CheckBox, "Force drop output? (Not spawning in inventory)")]
	bool m_bForceDropOutput;


	
	[Attribute("0 0 0", UIWidgets.EditBox, "Drop Offset", params: "inf inf 0 purposeCoords spaceEntity")]
	vector m_vDropOffset;
	
	[Attribute("0 0 0", UIWidgets.EditBox, "Drop Rotation")]
	vector m_vRotation;
	
	ref SCR_PrefabNamePredicate m_pPrefabNamePredicate = new SCR_PrefabNamePredicate();

	
	//------------------------------------------------------------------------------------------------
	private void ProcessAction(IEntity pOwnerEntity, IEntity pUserEntity, SCR_ParticleEmitter processParticles)
	{
		if (processParticles)
			processParticles.Stop();
		InventoryStorageManagerComponent inventoryManager = EL_ComponentFinder<InventoryStorageManagerComponent>.Find(pUserEntity);

		SCR_PrefabNamePredicate prefabNamePredicate();

		foreach (EL_ProcessingInput processingInput : m_aProcessingInputs)
		{
			//Set search to new input prefab
			prefabNamePredicate.prefabName = processingInput.m_InputPrefab;

			for (int i = 0; i < processingInput.m_iInputAmount; i++)
			{
				ConsumeInput(inventoryManager.FindItem(prefabNamePredicate), pUserEntity, inventoryManager);
			}
		}

		foreach (EL_ProcessingOutput processingOutput : m_aProcessingOutputs)
		{
			for (int i = 0; i < processingOutput.m_iOutputAmount; i++)
			{
				if (m_bForceDropOutput || !inventoryManager.TrySpawnPrefabToStorage(processingOutput.m_OutputPrefab))
				{
					EL_Utils.SpawnEntityPrefab(processingOutput.m_OutputPrefab, pOwnerEntity.GetOrigin() + m_vDropOffset, m_vRotation);
				}
			}
		}
	}
		
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		SCR_ParticleEmitter processParticles;
		if (m_ProcessParticle)
			processParticles = SCR_ParticleEmitter.Create(m_ProcessParticle, m_vParticleOffset, m_vParticleRotation, pOwnerEntity);
		GetGame().GetCallqueue().CallLater(ProcessAction, m_fProcessTime * 1000, false, pOwnerEntity, pUserEntity, processParticles);
	}

	//------------------------------------------------------------------------------------------------
	void ConsumeInput(IEntity input, IEntity pUserEntity, InventoryStorageManagerComponent inventoryManager)
	{
		inventoryManager.TryDeleteItem(input);
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
 	{
		//Check player inventory
		InventoryStorageManagerComponent inventoryManager = InventoryStorageManagerComponent.Cast(user.FindComponent(SCR_InventoryStorageManagerComponent));

		foreach (EL_ProcessingInput processingInput : m_aProcessingInputs)
		{
			int inputPrefabsInInv = inventoryManager.GetDepositItemCountByResource(processingInput.m_InputPrefab);
			if (inputPrefabsInInv < processingInput.m_iInputAmount)
				return false;
		}

		return true;
	}

	//------------------------------------------------------------------------------------------------
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		SetCannotPerformReason("Can't find items");
	}
}

