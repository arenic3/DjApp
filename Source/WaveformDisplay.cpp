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
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse, DJAudioPlayer& player, juce::Colour theme) : djAudioPlayer(player), col(theme), audioThumbnail(2000, formatManagerToUse, cacheToUse)
{
    setBufferedToImage(true);
    audioThumbnail.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    if(fileLoaded){

        //Draw the waveform when the deck gets a file to play
        g.setColour(col);
        auto b = getLocalBounds().toNearestInt().reduced(5.0f);
        b.setBounds(0, 10, getWidth(), getHeight()/1.3);
        audioThumbnail.drawChannel(g, b.reduced(1.0f, 5.0f), 0, audioThumbnail.getTotalLength(), 0, 1.0f);
        
        g.setColour(juce::Colours::green);
        int w = 4;
        g.drawRect(position * getWidth() - w/2, 1, w, getHeight()-2);
        
        //Store current Minutes, seconds and milliseconds in variables
        auto tl = audioThumbnail.getTotalLength();
        
        int tcS = static_cast<int>(tl * position) % 60;
        int tcM = static_cast<int>((tl * position) / 60);
        int tcMs = static_cast<int>(position*(tl * 1000))%1000;
        
        // Display the timecode on the waveform display
        g.setColour(juce::Colours::white);
        g.setFont(15.0f);
        g.drawText(juce::String(tcM).paddedLeft('0', 2) + ":" + juce::String(tcS).paddedLeft('0', 2) + ":" + juce::String(tcMs).paddedLeft('0', 3),
        2, getHeight() - 80, 100, 20, juce::Justification::left);
        
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
