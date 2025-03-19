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

        //Draw the waveform when the deck gets a file to play
        g.setGradientFill(juce::ColourGradient(juce::Colours::orange, 0, 0, juce::Colours::red, getWidth(), getHeight(), false));
        auto b = getBoundsInParent();
        b.setBounds(0, 10, getWidth(), getHeight()/1.3);
        audioThumbnail.drawChannel(g, b, 0, audioThumbnail.getTotalLength(), 0, 1.0f);
        g.setColour(juce::Colours::green);
        int w = 4;
        g.drawRect(position * getWidth() - w/2, 0, w, getHeight());
        
        //Store current Minutes, seconds and milliseconds in variables
        auto tl = audioThumbnail.getTotalLength();
        
        int tcS = static_cast<int>(tl * position) % 60;
        int tcM = static_cast<int>((tl * position) / 60);
        int tcMs = static_cast<int>(position*(tl * 1000))%1000;
        
        // Display the timecode on the waveform display
        g.setColour(juce::Colours::white);
        g.setFont(15.0f);
        g.drawText(juce::String(tcM) + ":" + juce::String(tcS).paddedLeft('0', 2) + ":" + juce::String(tcMs).paddedLeft('0', 3),
        0, getHeight() - 68, 100, 20, juce::Justification::left);
        
    } else {
        g.setColour(juce::Colours::darkorange);
        g.drawText("-", 0, 0, getWidth(), getHeight(), juce::Justification::centred);
    }
}

void WaveformDisplay::resized()
{

}

bool WaveformDisplay::loadURL(const juce::URL& url)
{
    //Retrieve loaded file URL from the audioPlayer
    audioThumbnail.clear();
    fileLoaded = audioThumbnail.setSource(new URLInputSource(djAudioPlayer.lurl));
    return fileLoaded;
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster * source)
{
    //Refresh component
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos){
    //Update position -> to be used for playhead
    if(pos >= 0. && pos != position){
        position = pos;
        repaint();
    }
}
