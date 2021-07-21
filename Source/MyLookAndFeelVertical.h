#pragma once

#include <JuceHeader.h>

class CustomLookAndFeelVertical : public juce::LookAndFeel_V4
{
public:
    juce::Image imageKnob;

    CustomLookAndFeelVertical();
    ~CustomLookAndFeelVertical();

    void drawLinearSliderBackground(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float minSliderPos, float maxSliderPos, 
                          const juce::Slider::SliderStyle style, juce::Slider& slider);
};

