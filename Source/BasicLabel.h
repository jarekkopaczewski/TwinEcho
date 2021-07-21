#include <JuceHeader.h>
#include "BasicSlider.h"

#pragma once

class BasicLabel : public juce::Label
{
public:
    BasicLabel();
    ~BasicLabel();
    void setBasicValue(std::string name, bool isOnleft, BasicSlider& slider );
};
