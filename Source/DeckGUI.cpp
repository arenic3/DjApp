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
DeckGUI::DeckGUI(DJAudioPlayer& player, juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse, ScrollingWaveform* wave1, juce::Colour theme)
: waveformDisplay(formatManagerToUse, cacheToUse, player, theme), scrollWave(wave1), djAudioPlayer(player), col(theme)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setBufferedToImage(true);
    
    //Play / Pause button
    playPauseButton.setLookAndFeel(&playButton);
    addAndMakeVisible(playPauseButton);
    //playPauseButton.setButtonText("Play/Pause");
    playPauseButton.addListener(this);
    
    //Stop button
    stopButton.setLookAndFeel(&customStopButton);
    addAndMakeVisible(stopButton);
    //stopButton.setButtonText("Stop");
    stopButton.addListener(this);
    
    //Gain dial
    gainDial.setLookAndFeel(&customDial);
    addAndMakeVisible(gainDial);
    gainDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);  //Change slider into Dial
    gainDial.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);    //Remove gain slider value box
    gainDial.addListener(this);
    gainDial.setRange(0, 1);
    gainDial.setValue(0.5);
    
    addAndMakeVisible(gainLabel);
    gainLabel.setText("GAIN", juce::dontSendNotification);
    gainLabel.setColour(gainLabel.textColourId, juce::Colours::white);
    gainLabel.attachToComponent(&gainDial, false);
    
    //Position slider
    posSlider.setLookAndFeel(&customPosSlider);
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
    speedLabel.setText("SPEED", juce::dontSendNotification);
    speedLabel.setColour(speedLabel.textColourId, juce::Colours::white);
    speedLabel.attachToComponent(&speedSlider, true);
    
    //Cutoff dial
    cutoffDial.setLookAndFeel(&customDial);
    addAndMakeVisible(cutoffDial);
    cutoffDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);  //Change slider into Dial
    cutoffDial.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);    //Remove gain slider value box
    cutoffDial.addListener(this);
    cutoffDial.setRange(0, 1);
    cutoffDial.setValue(1);
    cutoffDial.onValueChange = [this](){    //Lambda
        float v = cutoffDial.getValue();
        djAudioPlayer.setCutoff(v*v*v*0.5);
    };
    
    addAndMakeVisible(cutoffLabel);
    cutoffLabel.setText("CUTOFF", juce::dontSendNotification);
    cutoffLabel.setColour(cutoffLabel.textColourId, juce::Colours::white);
    cutoffLabel.attachToComponent(&cutoffDial, false);
    
    //Waveform
    addAndMakeVisible(waveformDisplay);
    waveformDisplay.addMouseListener(this, false);
    
    //Deck Label
    addAndMakeVisible(deck);
    deck.setFont(juce::FontOptions(20.f, juce::Font::bold));
    
    if(col == juce::Colours::orange){
        deck.setText("DECK 1", juce::dontSendNotification);
    } else {
        deck.setText("DECK 2", juce::dontSendNotification);
    }
    
    deck.setColour(speedLabel.textColourId, juce::Colours::darkorange.withAlpha(0.7f));
    
    //Timer
    startTimer(30);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    g.setColour(juce::Colour(40, 40, 40));
    g.fillRect(waveformDisplay.getBounds().reduced(1.5f));
    
    g.setColour(juce::Colours::orange);
    g.drawRect(waveformDisplay.getBounds());
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
    
    
    playPauseButton.setBounds(15, getHeight()/1.7, columnW/1.5, 1.01*rowH); //Play Button
    stopButton.setBounds(15, getHeight()/1.3, columnW/1.5, 1.01*rowH); //Stop Button
    gainDial.setBounds(getWidth()/1.98, rowH*4, 1.5*rowH, 1.5*rowH);  //Gain Dial
    gainLabel.setBounds(getWidth()/1.91,rowH*3, 2*rowH, 1.5*rowH);  //Gain Label
    cutoffDial.setBounds(getWidth()/3.18, rowH*4, 1.5*rowH, 1.5*rowH);  //Cutoff Dial
    cutoffLabel.setBounds(getWidth()/3.2,rowH*3, 2*rowH, 1.5*rowH);  //Cutoff Label
    speedSlider.setBounds(getWidth()/1.15, rowH*3.25-2, getWidth()/10, getHeight()/1.65);   //Speed slider
    posSlider.setBounds(10, rowH/2-15, getWidth()-20, rowH);  //Position Slider
    waveformDisplay.setBoundsRelative(0.013f, 0.2f, 0.975f, 0.33f); //Waveform
    deck.setBounds(getWidth()/1.45, getHeight()/1.8, getWidth()/6, rowH);
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
        scrollWave->setPositionRelative(0);
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
        scrollWave->loadURL(url);
    }
}

void DeckGUI::timerCallback(){
    waveformDisplay.setPositionRelative(djAudioPlayer.getPositionRelative());
    scrollWave->setPositionRelative(djAudioPlayer.getPositionRelative());
    
    posSlider.setValue(djAudioPlayer.getPositionRelative());
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
