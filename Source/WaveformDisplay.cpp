/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 10 Mar 2025 12:28:00pm
    Author:  Nicolas Arellano

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse, DJAudioPlayer& player) : audioThumbnail(1000, formatManagerToUse, cacheToUse), djAudioPlayer(player)
{
    audioThumbnail.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    if(fileLoaded)
    {
        g.setColour(juce::Colours::darkorange);
        audioThumbnail.drawChannel(g, getLocalBounds(), 0, audioThumbnail.getTotalLength(), 0, 1.);
    } else
    {
        g.setColour(juce::Colours::darkorange);
        g.drawText("Waveform", 0, 0, getWidth(), getHeight(), juce::Justification::centred);
    }
}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

bool WaveformDisplay::loadURL(const juce::URL& url)
{
    audioThumbnail.clear();
    fileLoaded = audioThumbnail.setSource(new URLInputSource(djAudioPlayer.lurl));
    return fileLoaded;
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster * source)
{
    DBG("change recieved!");
    repaint();
}
