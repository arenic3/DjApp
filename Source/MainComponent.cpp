#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    
    //Deck1
    addAndMakeVisible(deck1);
    
    /*Play Button
    addAndMakeVisible(playButton);
    playButton.setButtonText("Play");
    playButton.addListener(this);
    
    
    //Stop Button
    addAndMakeVisible(stopButton);
    stopButton.setButtonText("Stop");
    stopButton.addListener(this);
    
    //Load Button
    addAndMakeVisible(loadButton);
    loadButton.setButtonText("Load file");
    loadButton.addListener(this);
    
    //Gain Slider
    gainDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    addAndMakeVisible(gainDial);
    gainDial.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);    //Remove gain slider value box
    gainDial.addListener(this);
    gainDial.setRange(0, 1);
    gainDial.setValue(0.5);
    
    //Playback position slider
    addAndMakeVisible(posSlider);
    posSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);     //Remove text box from position slider
    posSlider.addListener(this);
    posSlider.setRange(0, 1);
    posSlider.setValue(0);
    */
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    //
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    /*
    if(!playing){
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    
    
    int numChannels = bufferToFill.buffer->getNumChannels();
    auto * leftChannel = numChannels>0 ? bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample) : nullptr;
    auto * rightChannel = numChannels>0 ? bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample) : nullptr;
    
    for(int i=0; i<bufferToFill.numSamples; ++i){
        auto sample = std::fmod(phase, 1.f)*2. -1.;
        sample *= 0.1 * gain;
        phase += std::fmod(freq, 0.01f);
        freq += 0.0000005f;
        if(leftChannel) leftChannel[i] = sample;
        if(rightChannel) rightChannel[i] = sample;
    }//*/
    
    player1.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    //
    player1.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::grey);
    //Drawing code:
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    
    deck1.setBounds(0, 0, getWidth()/2., getHeight());
}

void MainComponent::buttonClicked(juce::Button * button)
{
    //Called when buttons are clicked
    
    if (&playButton == button) {
        DBG("Maincomponent::buttonClicked: Play button clicked");
        if(!playing){
            DBG("audio playing");
            playing = true;
            player1.play();
        } else if(playing){
            DBG("audio paused");
            playing = false;
            player1.stop();
        }
        
    } else if (&stopButton == button) {
        DBG("Maincomponent::buttonClicked: Stop button clicked audio restarted");
        playing = false;
        player1.stop();
        player1.setPosition(0);
    }
    
    if(&loadButton == button) {
        FileChooser chooser{"choose a file to play..."};
        if(chooser.browseForFileToOpen()){
            auto url = URL(chooser.getResult());
            player1.loadURL(url);
        }
    }
}

void MainComponent::sliderValueChanged(juce::Slider * slider)
{
    //Called when sliders are changed
    
    if(&gainDial == slider) {
        DBG("Maincomponent::sliderValueChanged: slider changed" << gainDial.getValue());
        gain = gainDial.getValue();
        player1.setGain(gain);
    }
    
    if(&posSlider == slider) {
        DBG("Maincomponent::sliderValueChanged: slider changed" << posSlider.getValue());
        posi = posSlider.getValue();
        player1.setPositionRelative(posi);
    }
}
