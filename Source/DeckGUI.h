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
#include "WaveformDisplay.h"

//==============================================================================
/*
*/
class DeckGUI  : public juce::Component, public juce::Button::Listener, public juce::Slider::Listener, public juce::Timer
{
public:
    DeckGUI(DJAudioPlayer& player, juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse);
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void paintOverChildren(juce::Graphics& g) override;
    void resized() override;
    
    void buttonClicked(juce::Button * button) override;
    void sliderValueChanged(juce::Slider * slider) override;
    
    void loadWaveform();
    void timerCallback() override;
    
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;

private:
    WaveformDisplay waveformDisplay;
    CustomGUIDial customDial;
    CustomGUISlider customSlider;
    CustomGUIButton customButton;
    juce::TextButton playPauseButton;
    juce::TextButton stopButton;
    juce::Slider gainDial;
    juce::Slider posSlider;
    juce::Slider speedSlider;
    juce::Label speedLabel;
    DJAudioPlayer& djAudioPlayer;
    juce::Path path;
    juce::DropShadow shadow;
    juce::Rectangle<int> waveformWindow;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
