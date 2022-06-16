[EntityEditorProps(category: "EveronLife/Crop", description:"Basic staged crop entity")]
class EL_BaseCropClass : EL_BaseBuildingClass 
{
};	

//------------------------------------------------------------------------------------------------
class EL_StagePhaseTitle : BaseContainerCustomTitle
{
	override bool _WB_GetCustomTitle(BaseContainer source, out string title)
	{
		float time = 0;
		string name = "";
		source.Get("m_fStageTime", time);
		source.Get("m_sStageName", name);
		title = string.Format("%1 Stage | %2min", name, time);
		return true;
	}
};

//------------------------------------------------------------------------------------------------
[BaseContainerProps(), EL_StagePhaseTitle()]
class EL_CropStage
{
	[Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, "Model to use for the stage. Leave empty to use current model", "xob")]
	ResourceName m_NewStageModel;
	
	[Attribute("1", UIWidgets.Range, "How long this stage lasts (m)")]
	float m_fStageTime;
	
	[Attribute("", UIWidgets.Auto, "Name of this stage")]
	string m_sStageName;
	
	[Attribute("0 0 0", UIWidgets.Auto, "Offset in this stage")]
	vector m_StageOffset;
	
	[Attribute("1", UIWidgets.Auto, "Scale change in this stage")]
	float m_fStageScale;
	
	[Attribute("0", UIWidgets.Auto, "Can crop be gathered in this stage? (Directly)")]
	bool m_bCanGather;
}

//------------------------------------------------------------------------------------------------
class EL_BaseCrop : EL_BaseBuilding 
{
	[Attribute("-1", UIWidgets.CheckBox, "Spawn items afer last stage with area spawner? -1 to disable", category: "Crop")]
	protected int m_bSpawnItemsAtStage;
	
	[Attribute("0", UIWidgets.CheckBox, "Delete the crop after final stage?", category: "Crop")]
	protected bool m_bDeleteAfterFinalStage;
	
	[Attribute("", UIWidgets.Object, "List of stages", category: "Crop")]
	ref array<ref EL_CropStage> m_aCropStages;
	
	[RplProp(onRplName: "OnCropStageChanged")]
	protected int m_iCropStage = -1;
 
	
	protected bool m_IsInit = false;
	private float m_fStageStartTime = 0;

	//------------------------------------------------------------------------------------------------
	//! Change model, size etc..
	protected void OnCropStageChanged()
	{
		//End of last stage
		if (m_iCropStage > m_aCropStages.Count() - 1)
		{
			if (m_bDeleteAfterFinalStage)
			{
				RplComponent rplComponent = RplComponent.Cast(FindComponent(RplComponent));
				rplComponent.DeleteRplEntity(this, false);
			}
			return;
		}

		SetOrigin(GetOrigin() + m_aCropStages[m_iCropStage].m_StageOffset);
		SetScale(m_aCropStages[m_iCropStage].m_fStageScale);
		
		if (m_aCropStages[m_iCropStage].m_NewStageModel)
			SetObject(Resource.Load(m_aCropStages[m_iCropStage].m_NewStageModel).GetResource().ToVObject(), "");
		
		if (m_bSpawnItemsAtStage == m_iCropStage)
		{	
			//Only start area spawner on Server
			RplComponent rplComponent = RplComponent.Cast(FindComponent(RplComponent));
			EL_AreaSpawnerComponent areaSpawner = EL_AreaSpawnerComponent.Cast(FindComponent(EL_AreaSpawnerComponent));
			if (areaSpawner && rplComponent && rplComponent.IsMaster())
				areaSpawner.SpawnPrefabs(this);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	bool CanGather()
	{
		return m_aCropStages[m_iCropStage].m_bCanGather;
	}
	
	//------------------------------------------------------------------------------------------------
	protected void InitCrop(IEntity owner) 
	{
		RplComponent rplComponent = RplComponent.Cast(FindComponent(RplComponent));
		//Remove frame mask on other proxys
		if (rplComponent && !rplComponent.IsOwner())
		{
			ClearEventMask(EntityEvent.FRAME);
			return;
		}
		m_IsInit = true;
		//Authority specific behavior 

		//Set crop stage to first stage (0)
		m_iCropStage ++;
		Replication.BumpMe();
		
		//Dont grow on gui only server
		if (!System.IsConsoleApp())
			OnCropStageChanged();

		m_fStageStartTime = Replication.Time();
	}	
	
	//------------------------------------------------------------------------------------------------
	override void EOnFrame(IEntity owner, float timeSlice)
	{
		if (!m_IsInit)
		{
			InitCrop(owner);
			return;
		}
		// Authority specific behavior
		if (!m_aCropStages.Count())
			return; 
		//Time*60000 for replication time in minutes
		float stageEndTime = m_fStageStartTime + m_aCropStages[m_iCropStage].m_fStageTime * 60000;
		if (Replication.Time() >= stageEndTime)
		{
			m_iCropStage ++;
			Replication.BumpMe();

			//Dont grow on gui only server
			if (!System.IsConsoleApp())
				OnCropStageChanged();
			
			m_fStageStartTime = Replication.Time();
		}
	}

	void EL_BaseCrop(IEntitySource src, IEntity parent)
	{
		SetEventMask(EntityEvent.INIT | EntityEvent.FRAME);
		SetFlags(EntityFlags.ACTIVE, true);
    }
	
};