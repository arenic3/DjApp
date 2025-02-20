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
DeckGUI::DeckGUI(DJAudioPlayer& player)
: djAudioPlayer(player)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    //Play / Pause button
    addAndMakeVisible(playPauseButton);
    playPauseButton.setButtonText("Play");
    playPauseButton.addListener(this);
    
    //Stop button
    addAndMakeVisible(stopButton);
    stopButton.setButtonText("Stop");
    stopButton.addListener(this);
    
    //Load file button
    addAndMakeVisible(loadButton);
    loadButton.setButtonText("Load file");
    loadButton.addListener(this);
    
    //Gain dial
    addAndMakeVisible(gainDial);
    gainDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);  //Change slider into Dial
    gainDial.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);    //Remove gain slider value box
    gainDial.addListener(this);
    gainDial.setRange(0, 1);
    gainDial.setValue(0.5);
    
    //Position slider
    addAndMakeVisible(posSlider);
    posSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);     //Remove text box from position slider
    posSlider.addListener(this);
    posSlider.setRange(0, 1);
    posSlider.setValue(0);
    
    //Speed slider
    addAndMakeVisible(speedSlider);
    speedSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    speedSlider.addListener(this);
    speedSlider.setRange(0, 2);
    speedSlider.setValue(1);
}

DeckGUI::~DeckGUI()
{
}

void DeckGUI::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
}

void DeckGUI::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    auto rowH = getHeight() / 9;
    
    loadButton.setBounds(0, 0*rowH, getWidth()/6, rowH);    //Load button
    playPauseButton.setBounds(getWidth()/6, 0*rowH, getWidth()/6, rowH); //Play Button
    stopButton.setBounds(getWidth()/3, 0*rowH, getWidth()/6, rowH); //Stop Button
    gainDial.setBounds(getWidth()/1.2, 0.05*rowH, getWidth()/6, rowH);  //Gain Slider
    speedSlider.setBounds(getWidth()/2, 0*rowH, getWidth()/2.8, rowH);   //Speed slider
    posSlider.setBounds(0, 1*rowH, getWidth(), rowH);  //Position Slider
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
    
    if(&loadButton == button) {
        FileChooser chooser{"choose a file to play..."};
        if(chooser.browseForFileToOpen()){
            auto url = URL(chooser.getResult());
            djAudioPlayer.loadURL(url);
        }
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
        DBG("Maincomponent::sliderValueChanged: speed slider value changed" << posSlider.getValue());
        djAudioPlayer.setSpeed(slider->getValue());
    }
}
