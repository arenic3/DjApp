#pragma once

#include <JuceHeader.h>
#include "DeckGUI.h"
#include "FileManager.h"

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
    AudioFormatManager formatManager1;
    AudioThumbnailCache thumbCache{20};
    DJAudioPlayer player1{formatManager1};
    DeckGUI deck1{player1, formatManager1, thumbCache};
    DJAudioPlayer player2{formatManager1};
    DeckGUI deck2{player2, formatManager1, thumbCache};
    MixerAudioSource mixerSource;
    Image background;
    FileManager fileManager{player1, player2, deck1, deck2};
    Slider xfadeSlider;
    CustomGUISlider customXFadeSlider;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
