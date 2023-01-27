class EL_VehiclePriceTitle : BaseContainerCustomTitle
{
	//------------------------------------------------------------------------------------------------
	override bool _WB_GetCustomTitle(BaseContainer source, out string title)
	{
		int iBuyPrice;
		int iSellPrice;
		ResourceName prefab;
		source.Get("m_Prefab", prefab);
		source.Get("m_iBuyPrice", iBuyPrice);
		source.Get("m_iSellPrice", iSellPrice);
		string sName = FilePath.StripExtension(FilePath.StripPath(prefab.GetPath()));

		title = string.Format("%1: %2$ | %3$", sName, iBuyPrice, iSellPrice);

		return true;
	}
}

[BaseContainerProps(), EL_VehiclePriceTitle()]
class EL_VehiclePrice
{
	[Attribute("", UIWidgets.ResourcePickerThumbnail, "", "et")]
	ResourceName m_Prefab;
	[Attribute("0", UIWidgets.EditBox, "")]
	int m_iBuyPrice;
	[Attribute("0", UIWidgets.EditBox, "")]
	int m_iSellPrice;
	[Attribute("1", UIWidgets.CheckBox, "Show color picker in UI?")]
	bool m_bCanRecolor;
}

[BaseContainerProps(configRoot: true)]
class EL_VehiclePriceConfig : ScriptAndConfig
{
	[Attribute("", UIWidgets.Object, "")]
	ref array<ref EL_VehiclePrice> m_aVehiclePriceConfigs;
}
