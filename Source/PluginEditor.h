#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PluginProcessor.h"
#include "BasicSlider.h"
#include "BasicComboBox.h"
#include "BasicLabel.h"
#include "MyLookAndFeel.h"
#include "MyLookAndFeelVertical.h"

class DelayAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                   public juce::Button::Listener,
                                   public juce::ComboBox::Listener
{
public:
    DelayAudioProcessorEditor (DelayAudioProcessor&);
    ~DelayAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void buttonClicked(juce::Button* button) override;
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
    void initializePresets();
    BasicSlider slidersArray[10];
    BasicComboBox comboBoxArray[4];
    int step;
    int step2;
    int bpm;
    float presets[10][13];

private:

    juce::TextButton buttonBpm;

    CustomLookAndFeel lookAndFeel;
    CustomLookAndFeelVertical verticalLookAndFeel;

    DelayAudioProcessor& audioProcessor;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> slidersAttachment[10];
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> comboBoxAttachment[4];


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayAudioProcessorEditor)
};
