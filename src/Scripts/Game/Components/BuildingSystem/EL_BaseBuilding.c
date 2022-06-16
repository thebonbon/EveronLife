[EntityEditorProps(category: "EveronLife/Building", description:"Basic staged crop entity")]
class EL_BaseBuildingClass : GenericEntityClass 
{
};	

enum EBuildingType
{
	CROP
};

class EL_BaseBuilding : GenericEntity 
{
	[Attribute("", UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(EBuildingType), desc: "Placeable Type", category: "Building")]
	EBuildingType m_buildingType;
};