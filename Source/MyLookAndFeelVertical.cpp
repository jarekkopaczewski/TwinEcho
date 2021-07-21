#pragma once

#include "MyLookAndFeelVertical.h"

CustomLookAndFeelVertical::CustomLookAndFeelVertical()
{
}

CustomLookAndFeelVertical::~CustomLookAndFeelVertical()
{
}

void CustomLookAndFeelVertical::drawLinearSliderBackground(juce::Graphics& g, int x, int y, int width, int height,
                                                      float sliderPos, float minSliderPos, float maxSliderPos,
                                                      const juce::Slider::SliderStyle style, juce::Slider& slider)
{

    g.drawImage(imageKnob, x, y, width, height, x, y, imageKnob.getWidth(), imageKnob.getHeight());
    g.fillEllipse(x, y, 5.0f, 5.0f);

}


