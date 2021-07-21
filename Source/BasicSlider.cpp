#include "BasicSlider.h"

void BasicSlider::drawFrame(juce::Graphics& g, int x, int y, int width, int height, Slider& slider)
{
    juce::Image image = juce::ImageCache::getFromMemory(BinaryData::knobImage_png, BinaryData::knobImage_pngSize);

	const double div = slider.getMaximum() / frameCount;
	double pos = (int)(slider.getValue() / div);

	if (pos > 0)
		pos = pos - 1;

	if (width != height) /* scale */
	{
		x = (width / 2) - (height / 2);
		width = height;
	}

	if (isVerticalStrip)
	{
		g.drawImage(image, x, y, width, height, 0, (int)(pos * frameSize), frameSize, frameSize, false);
	}
	else
	{
		g.drawImage(image, x, y, width, height, (int)(pos * frameSize), 0, frameSize, frameSize, false);
	}
}

BasicSlider::BasicSlider()
{
	juce::Image knobStrip = juce::ImageCache::getFromMemory(BinaryData::knobImage_png, BinaryData::knobImage_pngSize);

	if (knobStrip.getWidth() > knobStrip.getHeight())
	{
		frameCount = knobStrip.getWidth() / knobStrip.getHeight();
		frameSize = knobStrip.getHeight();
		isVerticalStrip = false;
	}
	else
	{
		frameCount = knobStrip.getHeight() / knobStrip.getWidth();
		frameSize = knobStrip.getWidth();
		isVerticalStrip = true;
	}
}
#pragma once

BasicSlider::~BasicSlider()
{

}

void BasicSlider::setYourValues( int x_pos, int y_pos, int length, int height, bool isThereText,
                                juce::Slider::SliderStyle style, juce::Slider::TextEntryBoxPosition textPos,
                                int text_l, int text_h, float min, float max, 
                                float interval, float precis)
{

    this->setBounds(x_pos, y_pos, length, height);
    this->setSliderStyle(style);
    this->setTextBoxStyle(textPos, isThereText, text_l, text_h);
    this->setRange(min, max, interval);
    this->setNumDecimalPlacesToDisplay(precis);
}

void BasicSlider::setBasicValues(int x, int y)
{
    this->setBounds(x, y, 100, 100);
    this->setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    this->setTextBoxStyle(juce::Slider::TextBoxBelow, true, 150, 50);
    this->setRange(-48.0f, 0.0f, 0.1f);
    this->setNumDecimalPlacesToDisplay(2);
}
