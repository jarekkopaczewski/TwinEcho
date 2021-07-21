#pragma once

#include <JuceHeader.h>
#pragma once

class BasicSlider : public juce::Slider
{
public:
    void drawFrame(juce::Graphics& g, int x, int y, int width, int height, Slider& slider);
    BasicSlider();
    ~BasicSlider();
    void setYourValues( int x_pos, int y_pos, int length, int height, bool isThereText,
                       juce::Slider::SliderStyle style, juce::Slider::TextEntryBoxPosition textPos,  
                       int text_l , int text_h , float min, float max, 
                       float interval, float precis);
    void setBasicValues(int x_pos, int y_pos);

private:
    //const juce::Image knobStrip = juce::ImageCache::getFromMemory(BinaryData::crusher_background_png, BinaryData::crusher_background_pngSize);
    int frameCount;
    int frameSize;
    bool isVerticalStrip;

};