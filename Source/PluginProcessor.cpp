#pragma once

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define MAX_DELAY_TIME 10

DelayAudioProcessor::DelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
        treeState(*this, nullptr, juce::Identifier("TwinEcho"), createParameters())
#endif
{
    mLFOPhase = 0;

    mDelaySmoothed = 0;
    mDelaySmoothedLeft = 0;

    mFeedbackLeft = 0;
    mFeedbackRight = 0;

    mDelayReadHead = 0;
    mDelayReadHeadLeft = 0;
    mTimeInSamples = 0;
    mTimeInSamplesLeft = 0;

    mCirclarBufferLenght = 0;
    mCircularBufferWriterHead = 0;
    mCircularBufferWriterHeadLeft = 0;

    mCircularBufferLeft = nullptr;
    mCircularBufferRight = nullptr;

    lastSampleRate = 0.0;
}

DelayAudioProcessor::~DelayAudioProcessor()
{

    if (mCircularBufferLeft != nullptr)
    {
        delete[] mCircularBufferLeft;
        mCircularBufferLeft = nullptr;
    }

    if (mCircularBufferRight != nullptr)
    {
        delete[] mCircularBufferRight;
        mCircularBufferRight = nullptr;
    }
}

const juce::String DelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayAudioProcessor::getNumPrograms()
{
    return 1;
}

int DelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void DelayAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void DelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    lastSampleRate = sampleRate;

    mCirclarBufferLenght = MAX_DELAY_TIME * sampleRate;

    if (mCircularBufferLeft == nullptr)
    {
        mCircularBufferLeft = new float[mCirclarBufferLenght];
    }

    juce::zeromem(mCircularBufferLeft, mCirclarBufferLenght * sizeof(float));

    if (mCircularBufferRight == nullptr)
    {
        mCircularBufferRight = new float[mCirclarBufferLenght];
    }

    juce::zeromem(mCircularBufferRight, mCirclarBufferLenght * sizeof(float));

    mCircularBufferWriterHead = 0;
    mCircularBufferWriterHeadLeft = 0;

    mDelaySmoothed = *treeState.getRawParameterValue(TIME_R_ID);
    mDelaySmoothedLeft = *treeState.getRawParameterValue(TIME_L_ID);
}

void DelayAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DelayAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{

    juce::ScopedNoDenormals noDenormals;
    const int totalNumInputChannels = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    auto EffectType = treeState.getRawParameterValue(TYPE_ID);

    auto RateParameter = treeState.getRawParameterValue(RATE_ID);
    auto EffectDepth = treeState.getRawParameterValue(DEPTH_ID);

    auto RightTimeParameter = treeState.getRawParameterValue(TIME_R_ID);
    auto LeftTimeParameter = treeState.getRawParameterValue(TIME_L_ID);

    auto WaveBaseParameter = treeState.getRawParameterValue(WAVE_BASE_ID);

    auto RightFeedbackParameter = treeState.getRawParameterValue(FEEDBACK_R_ID);
    auto LeftFeedbackParameter = treeState.getRawParameterValue(FEEDBACK_L_ID);
    auto RightDryWetParameter = treeState.getRawParameterValue(DRY_WET_R);
    auto LeftDryWetParameter = treeState.getRawParameterValue(DRY_WET_L);

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    float* leftChannel = buffer.getWritePointer(0);
    float* rightChannel = buffer.getWritePointer(1);


    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        if (*EffectType != 0)
        {
            mLFOPhase = *RateParameter / getSampleRate();

            if (mLFOPhase > 1)
            {
                mLFOPhase -= 1;
            }

            float lfoOut = sin((double)(*WaveBaseParameter * 3.1415f * mLFOPhase));

            lfoOut *= *EffectDepth;

            float lfoOutMapped = juce::jmap(lfoOut, -1.f, 1.f, 0.002f, 0.005f);

            if (*EffectType == 1)
            {
                lfoOutMapped = juce::jmap(lfoOut, -1.f, 1.f, 0.01f, 0.07f);
            }
            else if (*EffectDepth == 2)
            {
                lfoOutMapped = juce::jmap(lfoOut, -1.f, 1.f, 0.001f, 0.005f);
            }


            mDelaySmoothed = mDelaySmoothed - 0.001 * (mDelaySmoothed - lfoOutMapped);
            mTimeInSamples = getSampleRate() * mDelaySmoothed;

            mDelaySmoothedLeft = mDelaySmoothedLeft - 0.001 * (mDelaySmoothedLeft - lfoOutMapped);
            mTimeInSamplesLeft = getSampleRate() * mDelaySmoothedLeft;
        }

        mDelaySmoothed = mDelaySmoothed - 0.001 * (mDelaySmoothed - *RightTimeParameter);
        mTimeInSamples = getSampleRate() * mDelaySmoothed;

        mDelaySmoothedLeft = mDelaySmoothedLeft - 0.001 * (mDelaySmoothedLeft - *LeftTimeParameter);
        mTimeInSamplesLeft = getSampleRate() * mDelaySmoothedLeft;

        mCircularBufferLeft[mCircularBufferWriterHeadLeft] = leftChannel[i] + mFeedbackLeft;
        mCircularBufferRight[mCircularBufferWriterHead] = rightChannel[i] + mFeedbackRight;

        //right
        mDelayReadHead = mCircularBufferWriterHead - mTimeInSamples;

        if (mDelayReadHead < 0)
        {
            mDelayReadHead += mCirclarBufferLenght;
        }

        //left
        mDelayReadHeadLeft = mCircularBufferWriterHeadLeft - mTimeInSamplesLeft;

        if (mDelayReadHeadLeft < 0)
        {
            mDelayReadHeadLeft += mCirclarBufferLenght;
        }

        //right
        int readHeadInt_x0 = (int)mDelayReadHead;
        int readHeadInt_x1 = readHeadInt_x0 + 1;

        float readHeadFloat = mDelayReadHead - readHeadInt_x0;

        if (readHeadInt_x1 >= mCirclarBufferLenght)
        {
            readHeadInt_x1 -= mCirclarBufferLenght;
        }

        if (readHeadInt_x1 < 0)
        {
            readHeadInt_x1 == 0;
        }

        //left
        int readHeadIntLeft_x0 = (int)mDelayReadHeadLeft;
        int readHeadIntLeft_x1 = readHeadIntLeft_x0 + 1;

        float readHeadFloatLeft = mDelayReadHeadLeft - readHeadIntLeft_x0;

        if (readHeadIntLeft_x1 >= mCirclarBufferLenght)
        {
            readHeadIntLeft_x1 -= mCirclarBufferLenght;
        }

        if (readHeadIntLeft_x1 < 0)
        {
            readHeadIntLeft_x1 == 0;
        }

        float delay_sample_left = line_interp(mCircularBufferLeft[readHeadIntLeft_x0], mCircularBufferLeft[readHeadIntLeft_x1], readHeadFloatLeft);
        float delay_sample_right = line_interp(mCircularBufferRight[readHeadInt_x0], mCircularBufferRight[readHeadInt_x1], readHeadFloat);

        mFeedbackLeft = delay_sample_left * *LeftFeedbackParameter;
        mFeedbackRight = delay_sample_right * *RightFeedbackParameter;

        buffer.setSample(0, i, (buffer.getSample(0, i) * (1 - *LeftDryWetParameter) + delay_sample_left * *LeftDryWetParameter));
        buffer.setSample(1, i, (buffer.getSample(1, i) * (1 - *RightDryWetParameter) + delay_sample_right * *RightDryWetParameter));

        mCircularBufferWriterHead++;
        mCircularBufferWriterHeadLeft++;

        if (mCircularBufferWriterHead >= mCirclarBufferLenght)
            mCircularBufferWriterHead = 0;

        if (mCircularBufferWriterHeadLeft >= mCirclarBufferLenght)
            mCircularBufferWriterHeadLeft = 0;
    }
}

bool DelayAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* DelayAudioProcessor::createEditor()
{
    return new DelayAudioProcessorEditor (*this);
}

//==============================================================================
void DelayAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = treeState.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void DelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(treeState.state.getType()))
            treeState.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessorValueTreeState::ParameterLayout DelayAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;

    //right delay params
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(DRY_WET_R, "DryWetR", 0.0f, 1.0f, 0.5f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(FEEDBACK_R_ID, "FeedbackR", 0.0f, 1.0f, 0.1f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(TIME_R_ID, "TimeR", 0.0f, MAX_DELAY_TIME, 0.5f));

    //left delay params
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(DRY_WET_L, "DryWetL", 0.0f, 1.0f, 0.5f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(FEEDBACK_L_ID, "FeedbackL", 0.0f, 1.0f, 0.1f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(TIME_L_ID, "TimeL", 0.0f, MAX_DELAY_TIME, 0.5f));

    //effect params
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(WAVE_BASE_ID, "WaveBase", 0.1f, 10.0f, 2.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(RATE_ID, "Rate", 0.0f, 1.00f, 0.05f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(DEPTH_ID, "Depth", 0.0f, 5000.0f, 50.0f));

    //comboboxes params
    parameters.push_back(std::make_unique<juce::AudioParameterInt>(TYPE_ID, "Type", 1, 4, 1));

    //addition
    parameters.push_back(std::make_unique<juce::AudioParameterInt>(BPM_ID, "Bpm", 50, 300, 120));
    parameters.push_back(std::make_unique<juce::AudioParameterInt>(STEP_L_ID, "StepL", 1, 18, 3));
    parameters.push_back(std::make_unique<juce::AudioParameterInt>(STEP_R_ID, "StepR", 1, 18, 3));
    //parameters.push_back(std::make_unique<juce::AudioParameterInt>(PRESET_ID, "Preset", 1, 10, 1));

    return { parameters.begin(), parameters.end() };
}

float DelayAudioProcessor::line_interp(float sample_x, float sample_x1, float inPhase)
{
    return ((1 - inPhase) * sample_x + inPhase * sample_x1);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayAudioProcessor();
}
