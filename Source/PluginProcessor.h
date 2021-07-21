#pragma once

#include <JuceHeader.h>

//delay params right
#define DRY_WET_R "dryWetR"
#define FEEDBACK_R_ID "feedbackR"
#define TIME_R_ID "timeR"

//delay params left
#define DRY_WET_L "dryWetL"
#define FEEDBACK_L_ID "feedbackL"
#define TIME_L_ID "timeL"

//effect params 
#define WAVE_BASE_ID "waveBase"
#define RATE_ID "rate"
#define DEPTH_ID "depth"

//effect params 
#define IS_ACTIVE_ID "isActive"
#define TYPE_ID "type"

//addition
#define BPM_ID "bpm"
#define STEP_L_ID "stepL"
#define STEP_R_ID "stepR"
#define PRESET_ID "preset"

class DelayAudioProcessor  : public juce::AudioProcessor
{
public:
    DelayAudioProcessor();
    ~DelayAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    float line_interp(float sample_x, float sample_x1, float inPhase);

    juce::AudioProcessorValueTreeState treeState;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

private:

    double lastSampleRate;
    double mLFOPhase;

    double mDelaySmoothed;
    double mDelaySmoothedLeft;

    double mFeedbackLeft;
    double mFeedbackRight;

    double mDelayReadHead;
    double mDelayReadHeadLeft;
    float mTimeInSamples;
    float mTimeInSamplesLeft;

    int mCirclarBufferLenght;
    int mCircularBufferWriterHead;
    int mCircularBufferWriterHeadLeft;

    float* mCircularBufferLeft;
    float* mCircularBufferRight;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayAudioProcessor)
};
