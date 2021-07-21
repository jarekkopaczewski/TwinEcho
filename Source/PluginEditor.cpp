#pragma once

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "BasicSlider.h"
#include "BasicComboBox.h"
#include "MyLookAndFeel.h"

DelayAudioProcessorEditor::DelayAudioProcessorEditor (DelayAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (422, 360);
    auto& params = processor.getParameters();

    std::string idSliderBase[10] =
    {
        //left
        DRY_WET_L, FEEDBACK_L_ID, TIME_L_ID,
        //right
        DRY_WET_R, FEEDBACK_R_ID, TIME_R_ID,
        //effect
        WAVE_BASE_ID, RATE_ID, DEPTH_ID,
        //addition
        BPM_ID
    };

    slidersArray[0].setYourValues(43, 35, 100, 100, true, juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Slider::TextBoxBelow, 97, 20, 0.0f, 1.0f, 0.02f, 2);
    slidersArray[1].setYourValues(43, 145, 100, 100, true, juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Slider::TextBoxBelow, 97, 20, 0.0f, 1.0f, 0.02f, 2);
    slidersArray[2].setYourValues(43, 255, 100, 100, true, juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Slider::TextBoxBelow, 97, 20, 0.0f, 10.0f, 0.02f, 2);

    slidersArray[3].setYourValues(143, 35, 100, 100, true, juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Slider::TextBoxBelow, 97, 20, 0.0f, 1.0f, 0.02f, 2);
    slidersArray[4].setYourValues(143, 145, 100, 100, true, juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Slider::TextBoxBelow, 97, 20, 0.0f, 1.0f, 0.02f, 2);
    slidersArray[5].setYourValues(143, 255, 100, 100, true, juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Slider::TextBoxBelow, 97, 20, 0.0f, 10.0f, 0.02f, 2);

    slidersArray[6].setYourValues(318, 35, 100, 100, true, juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Slider::TextBoxBelow, 97, 20, 0.1f, 10.0f, 0.1f, 2);
    slidersArray[7].setYourValues(318, 145, 100, 100, true, juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Slider::TextBoxBelow, 97, 20, 0.0f, 1.0f, 0.51f, 2);
    slidersArray[8].setYourValues(318, 255, 100, 100, true, juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Slider::TextBoxBelow, 97, 20, 0.0f, 5000.0f, 0.1f, 2);

    slidersArray[9].setYourValues(243, 50, 75, 305, true, juce::Slider::SliderStyle::LinearVertical, juce::Slider::TextBoxBelow, 70, 20, 50, 300, 1, 0);

    comboBoxArray[0].setWavesCombo(318, 7, 100, 19);
    comboBoxArray[1].setSteps(43, 7, 100, 19);//left
    comboBoxArray[2].setPresetCombo(3, 7, 40 , 19);
    comboBoxArray[3].setSteps(143, 7, 100, 19);//right

    buttonBpm.setBounds(243, 7, 75, 19);

    comboBoxAttachment[0] = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.treeState, TYPE_ID, comboBoxArray[0]);
    comboBoxAttachment[1] = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.treeState, STEP_L_ID, comboBoxArray[1]);
    comboBoxAttachment[3] = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.treeState, STEP_R_ID, comboBoxArray[3]);

    addAndMakeVisible(comboBoxArray[0]);
    addAndMakeVisible(comboBoxArray[1]);
    addAndMakeVisible(comboBoxArray[2]);
    addAndMakeVisible(comboBoxArray[3]);

    buttonBpm.setButtonText("update time");
    addAndMakeVisible(buttonBpm);
    buttonBpm.addListener(this);

    //conection with processor
    for (int x = 0; x < 10; x++)
    {
        slidersAttachment[x] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, idSliderBase[x], slidersArray[x]);
    }

    for (int i = 0; i < 9; i++)
    {
        slidersArray[i].setLookAndFeel(&lookAndFeel);
    }

    //add to window

    for (int i = 0; i < 10; i++)
    {
       addAndMakeVisible(slidersArray[i]);
    }

    initializePresets();
    comboBoxArray[2].addListener(this);
}

DelayAudioProcessorEditor::~DelayAudioProcessorEditor()
{
    for (int i = 0; i < 9; i++)
    {
        slidersArray[i].setLookAndFeel(nullptr);
    }
}

void DelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    juce::Image background;

    background = juce::ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);

    g.drawImage(background, 0, 0, 450, 660, 0, 0, 450, 660);

    
    g.setColour(juce::Colours::white);

    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::lightgrey);

    g.setFont(13.0f);
    g.drawFittedText("drywet", 2, 53, 43, 30, juce::Justification::centred, 1);
    g.drawFittedText("feedback", 2, 163, 43, 30, juce::Justification::centred, 1);
    g.drawFittedText("time", 2, 271, 43, 30, juce::Justification::centred, 1);

    g.setFont(12.0f);
    g.drawFittedText("L", 125, 37, 10, 10, juce::Justification::centred, 1);
    g.drawFittedText("L", 125, 147, 10, 10, juce::Justification::centred, 1);
    g.drawFittedText("L", 125, 257, 10, 10, juce::Justification::centred, 1);


    g.drawFittedText("R", 225, 37, 10, 10, juce::Justification::centred, 1);
    g.drawFittedText("R", 225, 147, 10, 10, juce::Justification::centred, 1);
    g.drawFittedText("R", 225, 257, 10, 10, juce::Justification::centred, 1);

    g.drawFittedText("BPM", 267, 37, 30, 10, juce::Justification::centred, 1);

    g.setFont(11.5f);
    g.drawFittedText("base", 394, 37, 25, 10, juce::Justification::centred, 1);
    g.drawFittedText("rate", 394, 147, 25, 10, juce::Justification::centred, 1);
    g.drawFittedText("depth", 394, 257, 25, 10, juce::Justification::centred, 1);
}

void DelayAudioProcessorEditor::resized()
{
}

void DelayAudioProcessorEditor::buttonClicked(juce::Button* button)
{

    step = comboBoxArray[1].getSelectedId();
    step2 = comboBoxArray[3].getSelectedId();
    bpm = (int)slidersArray[9].getValue();

    double value;

    if (button == &buttonBpm)
    {
        //left
        if (step < 7)
        {
            value = (float)(60.0f / bpm) * (double)std::pow(2.0, (2 - step + 1));
            slidersArray[2].setValue(value);
        }
        else if (step < 13)
        {
            step -= 6;
            value = (float)(60.0f / bpm) * (double)std::pow(2.0, (2 - step + 1));
            slidersArray[2].setValue(value * 1.5f);
        }
        else
        {
            step -= 12;
            value = (float)(60.0f / bpm) * (double)std::pow(2.0, (2 - step + 1));
            slidersArray[2].setValue(value * 0.667f);
        }

        //right
        if (step2 < 7)
        {
            value = (float)(60.0f / bpm) * (double)std::pow(2.0, (2 - step2 + 1));
            slidersArray[5].setValue(value);
        }
        else if (step2 < 13)
        {
            step2 -= 6;
            value = (float)(60.0f / bpm) * (double)std::pow(2.0, (2 - step2 + 1));
            slidersArray[5].setValue(value * 1.5f);
        }
        else
        {
            step2 -= 12;
            value = (float)(60.0f / bpm) * (double)std::pow(2.0, (2 - step2 + 1));
            slidersArray[5].setValue(value * 0.667f);
        }
    }
}

void DelayAudioProcessorEditor::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &comboBoxArray[2] )
    {
        int id = comboBoxArray[2].getSelectedId()-1;
        //Left
        slidersArray[0].setValue(presets[id][0]);
        slidersArray[1].setValue(presets[id][1]);
        slidersArray[2].setValue(presets[id][2]);
        //Right
        slidersArray[3].setValue(presets[id][3]);
        slidersArray[4].setValue(presets[id][4]);
        slidersArray[5].setValue(presets[id][5]);
        //Effect
        slidersArray[6].setValue(presets[id][6]);
        slidersArray[7].setValue(presets[id][7]);
        slidersArray[8].setValue(presets[id][8]);
        //Bpm
        slidersArray[9].setValue((int)presets[id][9]);
        //Combo
        comboBoxArray[0].setSelectedId((int)presets[id][10]);
        comboBoxArray[1].setSelectedId((int)presets[id][11]);
        comboBoxArray[3].setSelectedId((int)presets[id][12]);
    }

}

void DelayAudioProcessorEditor::initializePresets()
{
    float tab[10][13] = {
    {0.5f,0.1f,0.5f,0.5f,0.1f,0.5f,2.0f,1.0f,50.0f,120,1,3,3}, //default
    {0.1f,0.15f,1.00f,0.1f,0.11f,0.67f,3.89f,12.85f,1780.0f,120,2,2,14},//jungle fade
    {0.3f,0.1f,0.07f,0.6f,0.1f,0.07f,2.0f,8.0f,2050.0f,113,2,6,6},//wide joe
    {1.00f,0.01f,0.08f,1.00f,0.01f,0.13f,0.3f,0.20f,40.0f,120,2,17,5},//shadow
    {0.4f,0.25f,0.08f,0.6f,0.25f,0.08f,3.00f,50.0f,1950.0f,120,3,17,17},//repeter
    {0.82f,0.3f,0.5f,0.83f,0.46f,0.33f,3.3f,24.5f,2140.0f,120,2,3,15},//psycho
    {0.38f,0.28f,3.0f,0.55f,0.66f,1.33f,3.0f,50.0f,5000.0f,120,2,7,13},//mist
    {0.2f,0.1f,0.5f,0.36f,0.08f,0.67f,2.0f,1.0f,50.0f,120,1,3,14},//chord
    {0.9f,0.15f,0.5f,0.75f,0.1f,0.33f,2.0f,1.0f,50.0f,120,1,3,15},//disorder
    {0.57f,0.31f,1.86f,0.78f,0.36f,0.33f,4.1f,24.5f,2140.0f,120,1,10,15}//fighters
    };

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            presets[i][j] = tab[i][j];
        }
    }
}
