modded enum ChimeraMenuPreset {
    EL_VehicleColorPicker
}

class EL_VehicleColorPickerClass: ChimeraMenuBase
{
    Widget m_wRoot;
	SliderWidget m_RedSlider, m_GreenSlider, m_BlueSlider;
	ImageWidget m_wImageWidget;
	ref Color m_NewColor = new Color();
	
    //------------------------------------------------------------------------------------------------
    override void OnMenuOpen()
    {
        m_wRoot = GetRootWidget();

		
		m_wImageWidget = ImageWidget.Cast(m_wRoot.FindAnyWidget("ColorImage"));
		m_RedSlider = SliderWidget.Cast(m_wRoot.FindAnyWidget("RedSlider"));
		m_GreenSlider = SliderWidget.Cast(m_wRoot.FindAnyWidget("GreenSlider"));
		m_BlueSlider = SliderWidget.Cast(m_wRoot.FindAnyWidget("BlueSlider"));

        SCR_ButtonTextComponent confirmButton = SCR_ButtonTextComponent.GetButtonText("ButtonConfirm", m_wRoot);
		SCR_ButtonTextComponent cancelButton = SCR_ButtonTextComponent.GetButtonText("ButtonCancel", m_wRoot);
				
		confirmButton.m_OnClicked.Insert(Close);
        cancelButton.m_OnClicked.Insert(Close);
    }

	//------------------------------------------------------------------------------------------------
	override void OnMenuUpdate(float tDelta)
	{
		if (m_RedSlider && m_GreenSlider && m_BlueSlider && m_wImageWidget)
		{
			m_NewColor = Color.FromRGBA(m_RedSlider.GetCurrent(), m_GreenSlider.GetCurrent(), m_BlueSlider.GetCurrent(), 255);
			m_wImageWidget.SetColor(m_NewColor);
		}
		// Invoke OnUpdate
		m_OnUpdate.Invoke(tDelta);
	}

}