/*
  ==============================================================================

    DeckGUI.h
    Created: 20 Feb 2025 11:19:24am
    Author:  Nicolas Arellano

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "CustomGUIElements.h"

//==============================================================================
/*
*/
class DeckGUI  : public juce::Component, public juce::Button::Listener, public juce::Slider::Listener
{
public:
    DeckGUI(DJAudioPlayer& player);
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void buttonClicked(juce::Button * button) override;
    void sliderValueChanged(juce::Slider * slider) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
    CustomGUIDial customDial;
    CustomGUISlider customSlider;
    juce::TextButton playPauseButton;
    juce::TextButton stopButton;
    juce::TextButton loadButton;
    juce::Slider gainDial;
    juce::Slider posSlider;
    juce::Slider speedSlider;
    juce::Label speedLabel;
    DJAudioPlayer& djAudioPlayer;
};
