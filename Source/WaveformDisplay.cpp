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
    if(fileLoaded){
        g.setGradientFill(juce::ColourGradient(juce::Colours::orange, 0, 0, juce::Colours::red, getWidth(), getHeight(), false));
        audioThumbnail.drawChannel(g, getLocalBounds(), 0, audioThumbnail.getTotalLength(), 0, 1.0f);
        g.setColour(juce::Colours::green);
        int w = 4;
        g.drawRect(position * getWidth() - w/2, 0, w, getHeight());
        
    } else {
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
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos){
    if(pos >= 0. && pos != position){
        position = pos;
        repaint();
    }
}
