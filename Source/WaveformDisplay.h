/*
  ==============================================================================

    WaveformDisplay.h
    Created: 10 Mar 2025 12:28:00pm
    Author:  Nicolas Arellano

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class WaveformDisplay  : public juce::Component, public juce::ChangeListener
{
public:
    WaveformDisplay(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse, DJAudioPlayer& player);
    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    bool loadURL(const juce::URL& url);
    void changeListenerCallback(juce::ChangeBroadcaster * source) override;
    void setPositionRelative(double pos);

private:
    juce::AudioThumbnail audioThumbnail;
    DJAudioPlayer& djAudioPlayer;
    
    bool fileLoaded = false;
    double position = 0.;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
