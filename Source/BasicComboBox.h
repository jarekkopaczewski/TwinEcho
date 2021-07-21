#include <JuceHeader.h>

#pragma once

class BasicComboBox : public juce::ComboBox
{
public:
	BasicComboBox();
	~BasicComboBox();
	void setWavesCombo(int x_pos, int y_pos, int length, int height);
	void setPresetCombo(int x_pos, int y_pos, int length, int height);
	void setEqualizationCombo(int x_pos, int y_pos, int length, int height);
	void setSteps(int x_pos, int y_pos, int length, int height);
};
