#pragma once

#include <JuceHeader.h>

using namespace juce;

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent, public juce::Button::Listener, public juce::Slider::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    void buttonClicked(juce::Button *button) override;  //inherit virtual function
    void sliderValueChanged(juce::Slider *slider) override;  //inherit virtual function
    
private:
    //==============================================================================
    // Your private member variables go here...
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    
    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::TextButton loadButton;
    juce::Slider gainSlider;
    juce::AudioFormatManager formatManager;
    juce::AudioTransportSource transportSource;
    juce::Random random;
    
    bool playing = false;
    double gain = 0.5;
    
    //float freq = 0.f;
    //float phase = 0.f;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
