#pragma once

#include "MyLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel()
{
    imageKnob = juce::ImageCache::getFromMemory(BinaryData::knobImage_png, BinaryData::knobImage_pngSize);
}

CustomLookAndFeel::~CustomLookAndFeel()
{
}

void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g,
    int x, int y, int width, int height, float sliderPos,
    float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    width -= 10;
    height -= 10;

    const double rotation = (slider.getValue() - slider.getMinimum())/ (slider.getMaximum()- slider.getMinimum());

            
    /*const float radius = juce::jmin(width / 2.0f, height / 2.0f);
    const float centerX = x + width * 0.5f;
    const float centerY = y + height * 0.5f;
    const float rx = centerX - radius - 1.0f;
    const float ry = centerY - radius;*/

    const int frames = imageKnob.getHeight() / imageKnob.getWidth();
    const int frameId = (int)ceil(rotation * ((double)frames - 1.0));

    float diameter = juce::jmin(width, height);
    float radius = diameter / 2;
    float centreX = x + width / 2;
    float centreY = y + height / 2;
    float rx = centreX - radius;
    float ry = centreY - radius;
    float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));

    juce::Rectangle<float> dialArea(rx, ry, diameter, diameter);

    g.setColour(juce::Colours::black);

    if( height > 100 )
    g.fillEllipse(rx + 3.0f, ry + 1.0f, diameter + 2.0f, diameter + 3.0f);
    else
    g.fillEllipse(rx + 1.5f, ry, diameter + 2.0f, diameter + 3.0f);

    g.setColour(juce::Colours::black);

    g.drawImage(imageKnob, (int)rx, (int)ry, 2 * (int)radius, 2 * (int)radius, 0, frameId * imageKnob.getWidth(), imageKnob.getWidth(), imageKnob.getHeight());

    juce::Path dialTick;

    if( height > 100 )
    dialTick.addRectangle(-1.0f, -radius, 5.0f, radius * 0.43f);
    else
    dialTick.addRectangle(-1.0f, -radius, 3.5f, radius * 0.43f);
        
    g.fillPath(dialTick, juce::AffineTransform::rotation(angle).translated(centreX, centreY));
}


