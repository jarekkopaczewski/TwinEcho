#include "BasicLabel.h"

BasicLabel::BasicLabel()
{
}

BasicLabel::~BasicLabel()
{
}

void BasicLabel::setBasicValue(std::string name, bool isOnleft, BasicSlider& slider)
{
    this->setText(name, juce::dontSendNotification);
    this->setJustificationType(juce::Justification::centred);
    this->attachToComponent(&slider, isOnleft);
}
