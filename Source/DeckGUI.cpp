/*
  ==============================================================================

    DeckGUI.cpp
    Created: 20 Feb 2025 11:19:24am
    Author:  Nicolas Arellano

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer& player, juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse)
: waveformDisplay(formatManagerToUse, cacheToUse, player), djAudioPlayer(player)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    //Play / Pause button
    playPauseButton.setLookAndFeel(&customButton);
    addAndMakeVisible(playPauseButton);
    playPauseButton.setButtonText("Play/Pause");
    playPauseButton.addListener(this);
    
    //Stop button
    stopButton.setLookAndFeel(&customButton);
    addAndMakeVisible(stopButton);
    stopButton.setButtonText("Stop");
    stopButton.addListener(this);
    
    //Gain dial
    gainDial.setLookAndFeel(&customDial);
    addAndMakeVisible(gainDial);
    gainDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);  //Change slider into Dial
    gainDial.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);    //Remove gain slider value box
    gainDial.addListener(this);
    gainDial.setRange(0, 1);
    gainDial.setValue(0.5);
    
    //Position slider
    //posSlider.setLookAndFeel(&customSlider);
    addAndMakeVisible(posSlider);
    posSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);     //Remove text box from position slider
    posSlider.addListener(this);
    posSlider.setRange(0, 1);
    posSlider.setValue(0);
    
    //Speed slider
    speedSlider.setLookAndFeel(&customDial);
    addAndMakeVisible(speedSlider);
    speedSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    speedSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    speedSlider.addListener(this);
    speedSlider.setRange(0, 2);
    speedSlider.setValue(1);
    
    addAndMakeVisible(speedLabel);
    speedLabel.setColour(speedLabel.textColourId, juce::Colours::black);
    speedLabel.attachToComponent(&speedSlider, false);
    
    //Waveform
    addAndMakeVisible(waveformDisplay);
}

DeckGUI::~DeckGUI()
{
    
}

void DeckGUI::paint (juce::Graphics& g)
{
    posSlider.setColour(posSlider.backgroundColourId, juce::Colour(250, 150, 40));
    
    float cornerSize = 10.0f;
    auto innerBounds = getLocalBounds().toFloat().reduced(10.0f);
    auto outerBounds = getLocalBounds().toFloat().reduced(7.0f);
    
    juce::Path path;
    path.addRoundedRectangle(outerBounds, cornerSize);
    
    juce::DropShadow shadow (juce::Colours::black.withAlpha(0.5f), 15, juce::Point<int> (0, 0));
    shadow.drawForPath (g, path);
    
    g.setColour(juce::Colours::lightgrey);
    g.fillRoundedRectangle(innerBounds, cornerSize);
    
    g.setColour(juce::Colours::lightgrey.withAlpha(0.5f));
    g.drawRoundedRectangle(innerBounds, cornerSize, 1.0f);
}

void DeckGUI::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    auto rowH = getHeight() / 6;
    auto columnW = getWidth()/5;
    
    //loadButton.setBounds(0, 0, getWidth()/5, rowH/2);    //Load button
    playPauseButton.setBounds(columnW, getHeight()/1.3, columnW, 1.15*rowH); //Play Button
    stopButton.setBounds(2*columnW, getHeight()/1.3, columnW, 1.15*rowH); //Stop Button
    gainDial.setBounds(getWidth()/1.2, rowH*4.4, getWidth()/6, 1.6*rowH);  //Gain Slider
    speedSlider.setBounds(5, rowH*4.4, getWidth()/6, 1.6*rowH);   //Speed slider
    posSlider.setBounds(15, 2*rowH, getWidth()/1.05, rowH);  //Position Slider
    waveformDisplay.setBounds(15, 1.4*rowH, getWidth()/1.05, rowH); //Waveform
}

void DeckGUI::buttonClicked(juce::Button * button){
    
    if (&playPauseButton == button) {
        DBG("Maincomponent::buttonClicked: Play button clicked");
        if(!djAudioPlayer.playing){
            DBG("audio playing");
            djAudioPlayer.playing = true;
            djAudioPlayer.play();
        } else if(djAudioPlayer.playing){
            DBG("audio paused");
            djAudioPlayer.playing = false;
            djAudioPlayer.stop();
        }
        
    } else if (&stopButton == button) {
        DBG("Maincomponent::buttonClicked: Stop button clicked audio restarted");
        djAudioPlayer.playing = false;
        djAudioPlayer.stop();
        djAudioPlayer.setPosition(0);
    }
    
}

void DeckGUI::sliderValueChanged(juce::Slider * slider){
    
    if(&gainDial == slider) {
        DBG("Maincomponent::sliderValueChanged: gain dial value changed" << gainDial.getValue());
        djAudioPlayer.setGain(slider->getValue());
    } else if(&posSlider == slider) {
        DBG("Maincomponent::sliderValueChanged: position slider value changed" << posSlider.getValue());
        djAudioPlayer.setPositionRelative(slider->getValue());
    } else if(&speedSlider == slider) {
        DBG("Maincomponent::sliderValueChanged: speed slider value changed" << speedSlider.getValue());
        djAudioPlayer.setSpeed(slider->getValue());
    }
}

void DeckGUI::loadWaveform()
{
    if(djAudioPlayer.isLoaded)
    {
        auto url = djAudioPlayer.lurl;
        waveformDisplay.loadURL(url);
    }
}
