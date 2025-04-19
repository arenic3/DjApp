/*
  ==============================================================================

    ScrollingWaveform.h
    Created: 15 Apr 2025 11:01:10am
    Author:  Nicolas Arellano

  ==============================================================================
*/

#pragma once

#include "WaveformDisplay.h"

//==============================================================================
/*
*/
class ScrollingWaveform  : public WaveformDisplay
{
public:
    ScrollingWaveform(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse, DJAudioPlayer& player, juce::Colour theme);
    ~ScrollingWaveform() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    const juce::Colour col;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScrollingWaveform)
};
