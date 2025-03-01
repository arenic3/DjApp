#pragma once

#include <JuceHeader.h>
#include "DeckGUI.h"
#include "FileManager.h"

using namespace juce;

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/



class MainComponent  : public juce::AudioAppComponent
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
    
private:
    //==============================================================================
    // Your private member variables go here...
    DJAudioPlayer player1;
    DeckGUI deck1{player1};
    DJAudioPlayer player2;
    DeckGUI deck2{player2};
    MixerAudioSource mixerSource;
    juce::Image background;
    FileManager fileManager{player1, player2};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
