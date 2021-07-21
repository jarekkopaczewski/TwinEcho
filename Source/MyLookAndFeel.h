#pragma once

#include <JuceHeader.h>

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    juce::Image imageKnob;

    CustomLookAndFeel();
    ~CustomLookAndFeel();

    void drawRotarySlider(juce::Graphics& g,
                          int x, int y, int width, int height, float sliderPos,
                          float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider);
};

