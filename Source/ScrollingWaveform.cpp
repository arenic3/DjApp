/*
  ==============================================================================

    ScrollingWaveform.cpp
    Created: 15 Apr 2025 11:01:10am
    Author:  Nicolas Arellano

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ScrollingWaveform.h"

//==============================================================================
ScrollingWaveform::ScrollingWaveform(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse, DJAudioPlayer& player, juce::Colour theme) : WaveformDisplay(formatManagerToUse, cacheToUse, player, theme), col(theme)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
}

ScrollingWaveform::~ScrollingWaveform()
{
}

void ScrollingWaveform::paint (juce::Graphics& g)
{
    auto innerBounds = getLocalBounds().toFloat(); //.reduced(5.0f, 5.0f);
    auto innterBounds = getLocalBounds().toNearestInt();//.reduced(5.0f, 0);
    /*
    auto outerBounds = getLocalBounds().toFloat().reduced(8.0f);
    
    path.addRectangle(outerBounds);
    
    shadow = juce::DropShadow(juce::Colours::black.withAlpha(0.5f), 10, juce::Point<int> (0, 0));
    shadow.drawForPath (g, path);
    */
    
    g.setColour(juce::Colour(40, 40, 40));
    g.fillRect(innerBounds);
    
    g.setColour(juce::Colour(60, 60, 60).withAlpha(0.5f));
    g.drawRect(innerBounds);
    
    if (fileLoaded) {
        double thisPos = position * audioThumbnail.getTotalLength();
        double half = audioThumbnail.getTotalLength() / 80;
        double left = thisPos - half;
        double right = thisPos + half;
        g.setColour(col);
        audioThumbnail.drawChannel(g, innterBounds, left, right, 0, .7);
        
        auto b = innterBounds.reduced(0, 50);
        if(col == juce::Colours::hotpink){
            g.setColour(juce::Colours::green);
            audioThumbnail.drawChannel(g, b, left, right, 0, .7);
        } else {
            g.setColour(juce::Colours::yellowgreen);
            audioThumbnail.drawChannel(g, b, left, right, 0, .7);
        }
        
        if(left <= 0){
            double empty = juce::jmap(fabs(left), (double)0, half * 2, (double)0, (double)getWidth());
            g.setColour(juce::Colour(40, 40, 40));
            g.fillRect(0, 6, empty, getHeight()-11);
        }
    }
    
    //PlayHead
    g.setColour(juce::Colours::green);
    g.drawRect((getWidth()/2)-1, 1, 2, getHeight()-2);
}

void ScrollingWaveform::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
