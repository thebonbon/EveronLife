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
		if (!EL_NetworkUtils.IsOwner(pOwnerEntity)) return;

		if (processParticles)
			processParticles.Stop();
		
		InventoryStorageManagerComponent inventoryManager = EL_ComponentFinder<InventoryStorageManagerComponent>.Find(pUserEntity);

		foreach (EL_ProcessingInput processingInput : m_aProcessingInputs)
		{
			EL_InventoryUtils.RemoveAmount(inventoryManager, processingInput.m_InputPrefab, processingInput.m_iInputAmount);
		}

		foreach (EL_ProcessingOutput processingOutput : m_aProcessingOutputs)
		{
			if (m_bForceDropOutput)
			{
				for (int i = 0; i < processingOutput.m_iOutputAmount; i++)
				{
					EL_Utils.SpawnEntityPrefab(processingOutput.m_OutputPrefab, pOwnerEntity.GetOrigin() + m_vDropOffset, m_vRotation);
				}
			}
			else
			{
				EL_InventoryUtils.AddAmount(inventoryManager, processingOutput.m_OutputPrefab, processingOutput.m_iOutputAmount);
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
	override bool CanBePerformedScript(IEntity user)
 	{
		InventoryStorageManagerComponent inventoryManager = EL_ComponentFinder<InventoryStorageManagerComponent>.Find(user);
		foreach (EL_ProcessingInput processingInput : m_aProcessingInputs)
		{
			int inputPrefabsInInv = EL_InventoryUtils.GetAmount(inventoryManager, processingInput.m_InputPrefab);
			if (inputPrefabsInInv < processingInput.m_iInputAmount) return false;
		}

		return true;
	}

	//------------------------------------------------------------------------------------------------
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		SetCannotPerformReason("Can't find items");
	}
}

