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
    speedSlider.setLookAndFeel(&customSlider);
    addAndMakeVisible(speedSlider);
    speedSlider.setSliderStyle(juce::Slider::LinearVertical);
    speedSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    speedSlider.addListener(this);
    speedSlider.setRange(0, 2);
    speedSlider.setValue(1);
    
    addAndMakeVisible(speedLabel);
    speedLabel.setColour(speedLabel.textColourId, juce::Colours::black);
    speedLabel.attachToComponent(&speedSlider, false);
    
    //Waveform
    addAndMakeVisible(waveformDisplay);
    waveformDisplay.addMouseListener(this, false);
    
    //Timer
    startTimer(20);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    float cornerSize = 10.0f;
    auto innerBounds = getLocalBounds().toFloat().reduced(10.0f);
    auto outerBounds = getLocalBounds().toFloat().reduced(7.0f);
    
    path.addRoundedRectangle(outerBounds, cornerSize);
    
    shadow = juce::DropShadow(juce::Colours::black.withAlpha(0.5f), 15, juce::Point<int> (0, 0));
    //shadow.drawForPath (g, path);
    
    g.setColour(juce::Colour(60, 60, 60));
    g.fillRoundedRectangle(innerBounds, cornerSize);
    
    g.setColour(juce::Colour(60, 60, 60).withAlpha(0.5f));
    g.drawRoundedRectangle(innerBounds, cornerSize, 1.0f);
}


void DeckGUI::paintOverChildren(juce::Graphics& g){
    if(djAudioPlayer.isLoaded){
        auto loopRegion = djAudioPlayer.loopRegion.load();
        if(loopRegion.proper()){
            g.setColour(juce::Colours::white);
            g.setOpacity(0.25);
            
            auto start = loopRegion.start();
            auto length = loopRegion.length();
            auto b = waveformDisplay.getBoundsInParent();
            b.setBounds(start* b.getWidth() + b.getX(), b.getY(), length * b.getWidth(), b.getHeight());
            g.fillRect(b);
        }
    }
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
    gainDial.setBounds(getWidth()/1.28, rowH*4, getWidth()/7, 1.2*rowH);  //Gain Dial
    speedSlider.setBounds(getWidth()/1.12, rowH*2.9, getWidth()/12, getHeight()/1.5);   //Speed slider
    posSlider.setBounds(0, rowH/2, getWidth(), rowH);  //Position Slider
    waveformDisplay.setBoundsRelative(0.021f, 0.2f, 0.961f, 0.33f); //Waveform
}

void DeckGUI::buttonClicked(juce::Button * button){
    
    if (&playPauseButton == button) {
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
        djAudioPlayer.playing = false;
        djAudioPlayer.stop();
        djAudioPlayer.setPosition(0);
    }
    
}

void DeckGUI::sliderValueChanged(juce::Slider * slider){
    
    if(&gainDial == slider) {
        djAudioPlayer.setGain(slider->getValue());
    } else if(&posSlider == slider) {
        djAudioPlayer.setPositionRelative(slider->getValue());
    } else if(&speedSlider == slider) {
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

void DeckGUI::timerCallback(){
    waveformDisplay.setPositionRelative(djAudioPlayer.getPositionRelative());
}
 
void DeckGUI::mouseDown(const juce::MouseEvent& event){
    if(&waveformDisplay == event.eventComponent){
        mouseDrag(event);
    }
}

void DeckGUI::mouseDrag(const juce::MouseEvent& event){
    
    if(&waveformDisplay == event.eventComponent){
        float invw = 1./waveformDisplay.getWidth();
        auto start = event.getMouseDownX() * invw;
        auto end = event.getPosition().getX() * invw;
        djAudioPlayer.loopRegion.load().start(start).end(end);
        djAudioPlayer.loopRegion.store(djAudioPlayer.loopRegion.load().start(start).end(end));
        waveformDisplay.repaint();
    }
}
